#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;

const float TT_MAX_FREQ = 35;    // 30ms period
const float TT_MIN_FREQ = 0.333; // 3s period

int tt_count, TT_MAX_COUNT,
    TT_MIN_COUNT;        // count between taps. MAX and MIN value
bool  tapping   = false; // True when user is tapping
bool  averaging = false; // True after 2nd tap
float TT_BPS;            // Audio blocks per second
float freq_tt;           // Frequency read from tap tempo

Parameter freq_k;          // Frequency read from knob
float     prev_k1 = -1.0f; // Previous knob position. Used to detect change
bool      use_tt  = false; // use tap tempo. Knob values arent read when true

float      lfo_out;
Oscillator lfo;

void tap_tempo();
void UpdateLeds();
void UpdateKnobs();

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    hw.ProcessAllControls();
    tap_tempo();
    UpdateKnobs();
    UpdateLeds();

    for(size_t i = 0; i < size; i++)
    {
        lfo_out   = lfo.Process();
        out[0][i] = lfo_out * in[0][i];
        out[1][i] = lfo_out * in[1][i];
    }
}

void tap_tempo()
{
    if(tapping)
    {
        tt_count++;

        if(hw.button2.RisingEdge() && tt_count > TT_MIN_COUNT)
        {
            if(averaging) // 3rd plus tap
                freq_tt = 0.6 * (freq_tt)
                          + 0.4 * (TT_BPS / tt_count); // Weighted Averaging
            else
            {
                // 2nd tap
                freq_tt   = TT_BPS / tt_count; // frequency = TT_BPS/tt_count
                averaging = true;
            }

            use_tt = true;
            lfo.SetFreq(freq_tt);
            tt_count = 0;
        }
        else if(tt_count == TT_MAX_COUNT)
        { // After 1/TT_MIN_FREQ seconds no tap, reset values
            tt_count  = 0;
            tapping   = false;
            averaging = false;
        }
    }
    else if(hw.button2.RisingEdge()) // 1st tap
    {
        tapping = true;
    }
}

void UpdateKnobs()
{
    float new_k1 = hw.knob1.Process();

    if(!use_tt)
    {
        lfo.SetFreq(freq_k.Process());
        prev_k1 = new_k1;
    }
    else if(
        new_k1 > prev_k1 + 0.05
        || new_k1
               < prev_k1
                     - 0.05) // When the knob value is changed set use_tt to false
        use_tt = false;
}

void UpdateLeds()
{
    if(tapping)
        hw.led1.Set(0, 0, 1); // blue
    else
        hw.led1.Set(0, 0, 0); // off

    hw.led2.Set(lfo_out, lfo_out, lfo_out);

    hw.UpdateLeds();
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(4); // number of samples handled per callback
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    TT_BPS
        = hw.AudioSampleRate()
          / hw.AudioBlockSize(); // Blocks per second = sample_rate/block_size
    TT_MAX_COUNT = round(TT_BPS / TT_MIN_FREQ); // tt_count = TT_BPS/frequency
    TT_MIN_COUNT = round(TT_BPS / TT_MAX_FREQ);

    freq_k.Init(
        hw.knob1,
        TT_MIN_FREQ,
        TT_MAX_FREQ,
        freq_k.LOGARITHMIC); // Mapping knob 1 values to freq_k logarithmically

    // LFO Settings
    lfo.Init(hw.AudioSampleRate());
    lfo.SetWaveform(lfo.WAVE_SIN);
    lfo.SetPw(0.25);
    lfo.SetFreq(1.0f);
    lfo.SetAmp(1.0f);

    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    while(1) {}
}