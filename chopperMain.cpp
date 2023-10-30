#include "daisy_pod.h"
#include "daisysp.h"

#include "chopper.h"
#include "colors.h"
#include "util.h"
#include "tempo_utils.h"

using namespace daisysp;
using namespace daisy;
using namespace bytebeat;

#define BASIC_EXP 1

#define TEMPO_MIN 30
#define TEMPO_DEFAUT 120
#define TEMPO_MAX 240

static DaisyPod pod;
static Chopper chopper;
static Utilities util;
static Parameter chopperPw;
static Parameter dryWetMix;

// http://bradthemad.org/guitar/tempo_explanation.php
// Freq(Hz) = BPM / 60
// 120 BPM = 2 Hz

static uint8_t tempo;
static bool active;
static float fChopperPw;
static float fDryWetMix;
static float oldk1,oldk2;

// tap tempo variables
static uint32_t prev_ms;
static uint16_t tt_count;

// prototypes
bool ConditionalParameter(float oldVal, float newVal, float &param, float update);
void UpdateKnobs(void);
void UpdateLED(RgbLed& led, uint8_t value);
void UpdateLEDs(void);
void UpdateButtons(void);
void UpdateEncoder(void);
void Controls(void);
void InitSynth(void);
void HandleSystemRealTime(uint8_t srt_type);

void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
  Controls();

  for (size_t i = 0; i < size; i += 2) {
    const float cout = chopper.Process();
    const float gate = active ? cout : 1.0f;
    pod.seed.SetLed(cout != 0.0f && active);

    out[i] = (0.5f * gate * fDryWetMix * in[i]) + (0.5f * (1.0f - fDryWetMix) * in[i]);
    out[i+1] = (0.5f * gate * fDryWetMix * in[i+1]) + (0.5f * (1.0f - fDryWetMix) * in[i+1]);
  }
}

void Controls(void)
{
  pod.ProcessAllControls();

  UpdateButtons();
  UpdateEncoder();
  UpdateKnobs();
  UpdateLEDs();
}

// Updates values if knob had changed
bool ConditionalParameter(float oldVal, float newVal, float &param, float update)
{
  if (abs(oldVal - newVal) > 0.00005) {
    param = update;
    return true;
  }

  return false;
}

void UpdateButtons(void)
{
  if (pod.button1.RisingEdge()) {
    active = !active;
    if (active)
      chopper.Reset();
  }

  if (pod.button2.RisingEdge()) {
    uint32_t ms = System::GetNow();
    uint32_t diff = ms - prev_ms;
    uint32_t bpm = TempoUtils::ms_to_bpm(diff);
    if (bpm >= TEMPO_MIN && bpm <= TEMPO_MAX) {
      tempo = bpm;
      chopper.SetFreq(TempoUtils::tempo_to_freq(tempo));
    }

    prev_ms = ms;
  }

#ifdef BASIC_EXP
  if (pod.button3.RisingEdge())
    chopper.Reset();
#endif
}

void UpdateLED(RgbLed& led, uint8_t value)
{
  switch (value) {
    case 0:
      led.Set(RED);
      break;
    case 1:
      led.Set(GREEN);
      break;
    case 2:
      led.Set(BLUE);
      break;
    case 3:
      led.Set(MAGENTA);
      break;
    case 4:
      led.Set(CYAN);
      break;
    case 5:
      led.Set(GOLD);
      break;
    case 6:
      led.Set(WHITE);
      break;
  }
}

void UpdateLEDs(void)
{
//  pod.seed.SetLed(active);
  uint8_t led1 = chopper.GetCurrentPattern() / 7;
  uint8_t led2 = chopper.GetCurrentPattern() % 7;
  UpdateLED(pod.led1, led1);
  UpdateLED(pod.led2, led2);
  pod.UpdateLeds();
}

void UpdateKnobs(void)
{
  float k1 = pod.knob1.Process();
  float k2 = dryWetMix.Process();

  if (ConditionalParameter(oldk1, k1, fChopperPw, chopperPw.Process()))
    chopper.SetPw(fChopperPw);

  if (ConditionalParameter(oldk2, k2, fDryWetMix, k2))
    fDryWetMix = k2;

  oldk1 = k1;
  oldk2 = k2;
}

void UpdateEncoder(void)
{
  if (pod.encoder.RisingEdge()) {
    tempo = TEMPO_DEFAUT;
    chopper.SetFreq(TempoUtils::tempo_to_freq(tempo));
  }

  int32_t inc = pod.encoder.Increment();
  if (inc == 1)
    chopper.NextPattern();
  else if (inc == -1)
    chopper.PrevPattern();
}

void HandleSystemRealTime(uint8_t srt_type)
{
  // MIDI Clock -  24 clicks per quarter note
  if (srt_type == TimingClock) {
    tt_count++;
    if (tt_count == 24) {
      uint32_t ms = System::GetNow();
      uint32_t diff = ms - prev_ms;
      uint32_t bpm = TempoUtils::ms_to_bpm(diff);
      if (bpm >= TEMPO_MIN && bpm <= TEMPO_MAX) {
        tempo = bpm;
        chopper.SetFreq(TempoUtils::tempo_to_freq(tempo));
      }

      prev_ms = ms;
      tt_count = 0;
    }
  }
}

void InitSynth(void)
{
  tempo = TEMPO_DEFAUT; // 120 BPM
  active = false;
  oldk1 = 0;
  oldk2 = 0;
  fChopperPw = 0.3f;
  fDryWetMix = 0.5f;

  prev_ms = 0;
  tt_count = 0;

  pod.Init();
  pod.SetAudioBlockSize(4);

  util.Init(&pod);

  float sample_rate = pod.AudioSampleRate();
  chopper.Init(sample_rate);
  chopper.SetFreq(TempoUtils::tempo_to_freq(tempo));
  chopper.SetAmp(1.0f);
  chopper.SetPw(fChopperPw);

  chopperPw.Init(pod.knob1, 0.1f, 0.9f, chopperPw.LINEAR);
  dryWetMix.Init(pod.knob2, 0.2f, 1.0f, dryWetMix.LINEAR);

  // initialize the logger
  pod.seed.StartLog(false);

  util.BlinkLED(WHITE);
}

int main(void)
{
  InitSynth();

  pod.StartAdc();
  pod.StartAudio(AudioCallback);
  pod.midi.StartReceive();

  while (true) {
    pod.midi.Listen();
    while (pod.midi.HasEvents()) {
      MidiEvent m = pod.midi.PopEvent();
      if (m.type == SystemRealTime) {
        HandleSystemRealTime(m.srt_type);
      }
    }
  }
}
