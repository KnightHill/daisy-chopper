#include "daisy_pod.h"
#include "daisysp.h"
#include "util.h"
#include "chopper.h"
#include "colors.h"

using namespace daisysp;
using namespace daisy;
using namespace bytebeat;

#define TEMPO_MIN 60
#define TEMPO_MAX 180

static DaisyPod  pod;
static Chopper   chopper;
static Utilities util;
static Parameter chopperPw;

// http://bradthemad.org/guitar/tempo_explanation.php
// Freq(Hz) = BPM / 60
// 120 BPM = 2 Hz

static uint8_t tempo;
static bool    active;
static float   fChopperPw;
static float   oldk1;

// prototypes
bool  ConditionalParameter(float  oldVal,
                           float  newVal,
                           float &param,
                           float  update);
void  UpdateKnobs(void);
void  UpdateLEDs(void);
void  UpdateButtons(void);
void  UpdateEncoder(void);
void  Controls(void);
void  InitSynth(void);
float CalcTempFreq(uint8_t tempo);

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    pod.ProcessAnalogControls();
    pod.ProcessDigitalControls();
    Controls();

    for(size_t i = 0; i < size; i += 2)
    {
        const float gate = active ? chopper.Process() : 1.0f;

        out[i]     = gate * in[i];
        out[i + 1] = gate * in[i + 1];
    }
}

void Controls(void)
{
    UpdateButtons();
    UpdateEncoder();
    UpdateKnobs();
    UpdateLEDs();
}

// Updates values if knob had changed
bool ConditionalParameter(float  oldVal,
                          float  newVal,
                          float &param,
                          float  update)
{
    if(abs(oldVal - newVal) > 0.00005)
    {
        param = update;
        return true;
    }

    return false;
}

void UpdateButtons(void)
{
    if(pod.button1.RisingEdge())
    {
        active = !active;
        if(active)
            chopper.Reset();
    }

    if(pod.button2.RisingEdge())
    {
        if(++tempo >= TEMPO_MAX)
            tempo = TEMPO_MAX;

        chopper.SetFreq(CalcTempFreq(tempo));
    }

    if(pod.button3.RisingEdge())
    {
        if(--tempo >= TEMPO_MIN)
            tempo = TEMPO_MIN;

        chopper.SetFreq(CalcTempFreq(tempo));
    }

    if(pod.button4.RisingEdge())
        chopper.Reset();
}

void UpdateLEDs(void)
{
    pod.seed.SetLed(active);

    switch(chopper.GetCurrentPattern())
    {
        case 0: pod.led2.Set(RED); break;
        case 1: pod.led2.Set(GREEN); break;
        case 2: pod.led2.Set(BLUE); break;
        case 3: pod.led2.Set(BLACK); break;
        case 4: pod.led2.Set(MAGENTA); break;
        case 5: pod.led2.Set(CYAN); break;
        case 6: pod.led2.Set(GOLD); break;
        case 7: pod.led2.Set(WHITE); break;
    }
    pod.UpdateLeds();
}

void UpdateKnobs(void)
{
    float k1 = pod.knob1.Process();

    if(ConditionalParameter(oldk1, k1, fChopperPw, chopperPw.Process()))
        chopper.SetPw(fChopperPw);

    oldk1 = k1;
}

void UpdateEncoder(void)
{
    //if(pod.encoder.RisingEdge()) {}
    int32_t inc = pod.encoder.Increment();
    if(inc == 1)
        chopper.NextPattern();
    else if(inc == -1)
        chopper.PrevPattern();
}

void InitSynth(void)
{
    tempo      = 120; // 120 BPM
    active     = false;
    oldk1      = 0;
    fChopperPw = 0.3f;

    pod.Init();
    pod.SetAudioBlockSize(4);

    /*
    // Set sample rate to 8kHz
    SaiHandle::Config sai_config;
    sai_config.sr = SaiHandle::Config::SampleRate::SAI_8KHZ;
    pod.SetAudioSampleRate(sai_config.sr);
    */

    util.Init(&pod);

    float sample_rate = pod.AudioSampleRate();
    chopper.Init(sample_rate);
    chopper.SetFreq(CalcTempFreq(tempo));
    chopper.SetAmp(1.0f);
    chopper.SetPw(fChopperPw);

    chopperPw.Init(pod.knob1, 0.1f, 0.9f, chopperPw.LINEAR);

    // initialize the logger
    pod.seed.StartLog(false);

    util.BlinkLED(WHITE);
}

/** Calculates the tempo frequency in Hz for a given BPM 
  http://bradthemad.org/guitar/tempo_explanation.php
  Freq(Hz) = BPM / 60
  120 BPM = 2 Hz
*/
float CalcTempFreq(uint8_t tempo)
{
    return tempo / 60.0f;
}

int main(void)
{
    InitSynth();

    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    while(true) {}
}