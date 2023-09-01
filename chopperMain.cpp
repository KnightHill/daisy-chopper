#include "daisy_pod.h"
#include "daisysp.h"
#include "util.h"
#include "chopper.h"

using namespace daisysp;
using namespace daisy;
using namespace bytebeat;

static DaisyPod  pod;
static Chopper   chopper;
static Utilities util;
static Parameter chopperFreq;
static Parameter chopperPw;

static bool  active;
static float fChopperFreq, fChopperPw;
static float oldk1, oldk2;

// prototypes
bool ConditionalParameter(float  oldVal,
                          float  newVal,
                          float &param,
                          float  update);
void UpdateKnobs(void);
void UpdateLEDs(void);
void UpdateButtons(void);
void InitSynth(void);

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float inl, inr;

    pod.ProcessAnalogControls();
    pod.ProcessDigitalControls();

    UpdateButtons();
    //UpdateKnobs();
    UpdateLEDs();

    //audio
    for(size_t i = 0; i < size; i += 2)
    {
        inl = in[i];
        inr = in[i + 1];

        float gate = 1.0f;

        if(active)
        {
            gate = chopper.Process();
        }

        // left out
        out[i] = inl * gate;

        // right out
        out[i + 1] = inr * gate;
    }
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
        chopper.NextPattern();
    }
}

void UpdateLEDs(void)
{
    pod.led1.Set(active, 0, 0);
    switch(chopper.GetCurrentPattern())
    {
        case 0: pod.led2.Set(1, 0, 0); break;
        case 1: pod.led2.Set(0, 1, 0); break;
        case 2: pod.led2.Set(0, 0, 1); break;
        case 3: pod.led2.Set(1, 1, 0); break;
        case 4: pod.led2.Set(1, 0, 1); break;
        case 5: pod.led2.Set(0, 1, 1); break;
        default: pod.led2.Set(0, 0, 0); break;
    }
    pod.UpdateLeds();
}

void UpdateKnobs(void)
{
    float k1 = pod.knob1.Process();
    float k2 = pod.knob2.Process();

    if(ConditionalParameter(oldk1, k1, fChopperFreq, chopperFreq.Process()))
    {
        chopper.SetFreq(fChopperFreq);
    }

    if(ConditionalParameter(oldk2, k2, fChopperPw, chopperPw.Process()))
    {
        chopper.SetPw(fChopperPw);
    }

    oldk1 = k1;
    oldk2 = k2;
}

void InitSynth(void)
{
    active = false;
    oldk1 = oldk2 = 0;
    fChopperFreq  = 2.0f; // 2 Hz = 120 BPM?
    fChopperPw    = 0.5f;

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
    chopper.SetFreq(fChopperFreq);
    chopper.SetAmp(1.0f);
    chopper.SetPw(fChopperPw);

    chopperFreq.Init(pod.knob1, 0.2, 6.0, chopperFreq.LINEAR);
    chopperPw.Init(pod.knob2, 0.0, 1.0, chopperPw.LINEAR);

    // initialize the logger
    // Debug on Linux:
    // cu -l /dev/ttyACM0
    // screen /dev/ttyACM0
    pod.seed.StartLog(false);
    // System::Delay(250);

    util.BlinkLED({1, 1, 1});
}

int main(void)
{
    InitSynth();

    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    while(true) {}
}