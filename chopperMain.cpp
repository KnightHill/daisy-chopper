#include "daisy_pod.h"
#include "daisysp.h"

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

static DaisyPod pod;
static Chopper chopper;
static Utilities util;
static Parameter chopperPw;

// http://bradthemad.org/guitar/tempo_explanation.php
// Freq(Hz) = BPM / 60
// 120 BPM = 2 Hz

static uint8_t tempo;
static bool active;
static float fChopperPw;
static float oldk1;

// tap tempo vars
static uint32_t prev_ms;
static uint16_t tt_count;

// prototypes
bool ConditionalParameter(float oldVal, float newVal, float &param, float update);
void UpdateKnobs(void);
void UpdateLEDs(void);
void UpdateButtons(void);
void UpdateEncoder(void);
void Controls(void);
void InitSynth(void);
void HandleSystemRealTime(uint8_t srt_type);

void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
  pod.ProcessAllControls();
  Controls();

  for (size_t i = 0; i < size; i += 2) {
    const float gate = active ? chopper.Process() : 1.0f;
    out[i] = gate * in[i];
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

  if (pod.button3.RisingEdge())
    chopper.Reset();
  /*
    if (pod.button2.RisingEdge()) {
      if (++tempo > TEMPO_MAX)
        tempo = TEMPO_MAX;

      chopper.SetFreq(tempo_to_freq(tempo));
    }

    if (pod.button3.RisingEdge()) {
      if (--tempo < TEMPO_MIN)
        tempo = TEMPO_MIN;

      chopper.SetFreq(tempo_to_freq(tempo));
    }
  */
}

void UpdateLEDs(void)
{
  pod.seed.SetLed(active);

  switch (chopper.GetCurrentPattern()) {
  case 0:
    pod.led2.Set(RED);
    break;
  case 1:
  case 8:
    pod.led2.Set(GREEN);
    break;
  case 2:
  case 9:
    pod.led2.Set(BLUE);
    break;
  case 3:
  case 10:
    pod.led2.Set(MAGENTA);
    break;
  case 4:
  case 11:
    pod.led2.Set(CYAN);
    break;
  case 5:
    pod.led2.Set(BLACK);
    break;
  case 6:
    pod.led2.Set(GOLD);
    break;
  case 7:
    pod.led2.Set(WHITE);
    break;
  }
  pod.UpdateLeds();
}

void UpdateKnobs(void)
{
  float k1 = pod.knob1.Process();

  if (ConditionalParameter(oldk1, k1, fChopperPw, chopperPw.Process()))
    chopper.SetPw(fChopperPw);

  oldk1 = k1;
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
  fChopperPw = 0.3f;

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
