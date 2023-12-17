#include "daisy_pod.h"
#include "daisysp.h"

#include "basic_exp.h"
#include "chopper.h"
#include "colors.h"
#include "util.h"
#include "tempo_utils.h"

using namespace daisysp;
using namespace daisy;
using namespace bytebeat;

#define TEMPO_MIN 30
#define TEMPO_DEFAUT 120
#define TEMPO_MAX 240

static BasicExp hw;
static Chopper chopper;
static Utilities util;
static Parameter chopperPw;
static Parameter dryWetMix;
static Parameter attack;

// http://bradthemad.org/guitar/tempo_explanation.php
// Freq(Hz) = BPM / 60
// 120 BPM = 2 Hz

static uint8_t tempo;
static bool active;
static float fChopperPw;
static float fDryWetMix;
static float fAttack;
static float oldk1, oldk2, oldk3;

static bool poSync;

// tap tempo variables
static uint32_t prev_ms;
static uint16_t tt_count;

// prototypes
bool ConditionalParameter(float oldVal, float newVal, float &param, float update);
void UpdateKnobs(void);
void UpdateLED(RgbLed &led, uint8_t value);
void UpdateLEDs(void);
void UpdateButtons(void);
void UpdateEncoder(void);
void Controls(void);
void InitSynth(void);
void HandleSystemRealTime(uint8_t srt_type);
void InitExpansionControls();

// TODO: Add PO sync
// https://modwiggler.com/forum/viewtopic.php?t=189932
void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
  Controls();

  for (size_t i = 0; i < size; i += 2) {
    float right;
    const float cout = chopper.Process();
    const float gate = active ? cout : 1.0f;
    hw.seed.SetLed(cout != 0.0f && active);

    float left = (0.5f * gate * fDryWetMix * in[i]) + (0.5f * (1.0f - fDryWetMix) * in[i]);
    if (poSync) {
      // right channel carries the PO sync signal
      right = left;
    } else {
      right = (0.5f * gate * fDryWetMix * in[i + 1]) + (0.5f * (1.0f - fDryWetMix) * in[i + 1]);
    }

    out[i] = left;
    out[i + 1] = right;
  }
}

void Controls(void)
{
  hw.ProcessAllControls();

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
  if (hw.button1.RisingEdge()) {
    active = !active;
    if (active)
      chopper.Reset();
  }

  if (hw.button2.RisingEdge()) {
    uint32_t ms = System::GetNow();
    uint32_t diff = ms - prev_ms;
    uint32_t bpm = TempoUtils::ms_to_bpm(diff);
    if (bpm >= TEMPO_MIN && bpm <= TEMPO_MAX) {
      tempo = bpm;
      chopper.SetFreq(TempoUtils::tempo_to_freq(tempo));
    }

    prev_ms = ms;
  }

  if (hw.button3.RisingEdge())
    chopper.Reset();

  if (hw.button4.RisingEdge())
    poSync = !poSync;
}

void UpdateLED(RgbLed &led, uint8_t value)
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
  //  hw.seed.SetLed(active);
  uint8_t led1 = chopper.GetCurrentPattern() / 7;
  uint8_t led2 = chopper.GetCurrentPattern() % 7;
  UpdateLED(hw.led1, led1);
  UpdateLED(hw.led2, led2);
  hw.UpdateLeds();
}

void UpdateKnobs(void)
{
  float k1 = hw.knob1.Process();
  float k2 = dryWetMix.Process();
  float k3 = attack.Process();

  if (ConditionalParameter(oldk1, k1, fChopperPw, chopperPw.Process()))
    chopper.SetPw(fChopperPw);

  if (ConditionalParameter(oldk2, k2, fDryWetMix, k2))
    fDryWetMix = k2;

  if (ConditionalParameter(oldk3, k3, fAttack, attack.Process()))
    chopper.SetAttack(fAttack);

  oldk1 = k1;
  oldk2 = k2;
  oldk3 = k3;
}

void UpdateEncoder(void)
{
  if (hw.encoder.RisingEdge()) {
    tempo = TEMPO_DEFAUT;
    chopper.SetFreq(TempoUtils::tempo_to_freq(tempo));
  }

  int32_t inc = hw.encoder.Increment();
  if (inc == 1)
    chopper.NextPattern();
  else if (inc == -1)
    chopper.PrevPattern();
}

void HandleSystemRealTime(uint8_t srt_type)
{
  // MIDI Clock -  24 clicks per quarter note
  if (srt_type == TimingClock && poSync == false) {
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
  oldk1 = oldk2 = oldk3 = 0;

  fChopperPw = 0.3f;
  fDryWetMix = 0.5f;
  fAttack = 0.1f;

  prev_ms = 0;
  tt_count = 0;

  poSync = false;

  hw.Init();
  hw.SetAudioBlockSize(4);

  util.Init(&hw);

  const float sample_rate = hw.AudioSampleRate();
  chopper.Init(sample_rate);
  chopper.SetFreq(TempoUtils::tempo_to_freq(tempo));
  chopper.SetAmp(1.0f);
  chopper.SetPw(fChopperPw);
  chopper.SetAttack(fAttack);

  chopperPw.Init(hw.knob1, 0.1f, 0.9f, chopperPw.LINEAR);
  dryWetMix.Init(hw.knob2, 0.2f, 1.0f, dryWetMix.LINEAR);
  attack.Init(hw.knob3, 0.02f, 0.2f, attack.LINEAR);

  // initialize the logger
  hw.seed.StartLog(false);

  util.BlinkLED(WHITE);
}

int main(void)
{
  InitSynth();

  hw.StartAdc();
  hw.StartAudio(AudioCallback);
  hw.midi.StartReceive();

  while (true) {
    hw.midi.Listen();
    while (hw.midi.HasEvents()) {
      MidiEvent m = hw.midi.PopEvent();
      if (m.type == SystemRealTime) {
        HandleSystemRealTime(m.srt_type);
      }
    }
  }
}
