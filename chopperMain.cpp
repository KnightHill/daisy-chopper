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

bool active;


void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float inl, inr;

    pod.ProcessAnalogControls();
    pod.ProcessDigitalControls();

    if(pod.button1.RisingEdge())
    {
        active = !active;
    }

    pod.led1.Set(active, 0, 0);
    pod.UpdateLeds();

    //audio
    for(size_t i = 0; i < size; i += 2)
    {
        inl = in[i];
        inr = in[i + 1];

        float gate = 1.0f;

        if(active)
            gate = chopper.Process();

        // left out
        out[i] = inl * gate;

        // right out
        out[i + 1] = inr * gate;
    }
}

void InitSynth()
{
    active = false;

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
    chopper.SetFreq(2.5f);
    chopper.SetAmp(1.0f);
    chopper.SetPw(0.5f);

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