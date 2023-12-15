#include "daisysp.h"
#include "chopper.h"

using namespace daisysp;
using namespace bytebeat;

constexpr float TWO_PI_RECIP = 1.0f / TWOPI_F;

Pattern Chopper::Patterns[PATTERNS_MAX] = {
    {16, {{1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}}},

    {16, {{1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}}},
    {8, {{1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}}},

    {16, {{1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}}},
    {4, {{1, D4}, {1, D4}, {1, D4}, {1, D4}}},

    {16, {{1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}}},
    {16, {{1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}}},

    {12, {{1, D8}, {1, D16}, {1, D16}, {1, D8}, {1, D16}, {1, D16}, {1, D8}, {1, D16}, {1, D16}, {1, D8}, {1, D16}, {1, D16}}},
    {12, {{1, D4}, {1, D8}, {1, D8}, {1, D4}, {1, D8}, {1, D8}, {1, D4}, {1, D8}, {1, D8}, {1, D4}, {1, D8}, {1, D8}}},

    {12, {{1, D16}, {1, D8}, {1, D8}, {1, D16}, {1, D8}, {1, D8}, {1, D16}, {1, D8}, {1, D8}, {1, D16}, {1, D8}, {1, D8}}},
    {12, {{1, D8}, {1, D4}, {1, D4}, {1, D8}, {1, D4}, {1, D4}, {1, D8}, {1, D4}, {1, D4}, {1, D8}, {1, D4}, {1, D4}}},
    {14, {{1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D8}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D8}}},

    {14, {{1, D8}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D8}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}}},
    {14, {{1, D8}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D8}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}}},
    {14, {{1, D8}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D8}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}}},
    {14, {{1, D8}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D8}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}}}

};

// 3/4 - doesn't work:    {8, {{1, D4}, {1, D8}, {1, D4}, {1, D8}, {1, D4}, {1, D8}, {1, D4}, {1, D8}}}

void Chopper::Init(float sample_rate)
{
  sr_ = sample_rate;
  sr_recip_ = 1.0f / sample_rate;
  freq_ = 100.0f;
  amp_ = 0.5f;
  pw_ = 0.5f;
  pw_rad_ = pw_ * TWOPI_F;
  phase_ = 0.0f;
  phase_inc_ = CalcPhaseInc(freq_);
  eoc_ = true;
  eor_ = true;
  current_pattern_ = 0;
  pattern_step_ = 0;
  old_quadrant_index_ = -1;

  // Init ADSR
  env_.Init(sample_rate);
  env_.SetTime(ADSR_SEG_ATTACK, .1);
  env_.SetTime(ADSR_SEG_DECAY, .1);
  env_.SetTime(ADSR_SEG_RELEASE, .01);
  env_.SetSustainLevel(.9);
}

void Chopper::Reset(float _phase)
{
  phase_ = _phase;
  pattern_step_ = 0;
  old_quadrant_index_ = -1;
}

void Chopper::IncPatternStep(uint8_t length)
{
  if (++pattern_step_ >= length)
    pattern_step_ = 0;
}

void Chopper::NextPattern(bool reset)
{
  current_pattern_++;
  if (current_pattern_ >= PATTERNS_MAX)
    current_pattern_ = 0;
  if (reset)
    Reset();
}

void Chopper::PrevPattern(bool reset)
{
  current_pattern_--;
  if (current_pattern_ < 0)
    current_pattern_ = PATTERNS_MAX - 1;
  if (reset)
    Reset();
}

float Chopper::CalcPhaseInc(float f) { return (TWOPI_F * f) * sr_recip_; }

uint16_t Chopper::GetQuadrant(float numQuadrants)
{
  float phase = phase_;
  if (phase > TWOPI_F)
    phase = TWOPI_F;

  return static_cast<uint16_t>(phase * numQuadrants / TWOPI_F);
}

float Chopper::Process()
{
  float gate = ProcessGate();
  float out = env_.Process(gate > 0);
  return out * amp_;
}

/**
 * Works only with 4/4 patterns
 * No support for tied notes over measures
 * No support for triplets
 */
float Chopper::ProcessGate()
{
  float out;
  float quadrant = floorf(phase_ / HALFPI_F);
  int16_t quadrant_index = static_cast<int16_t>(quadrant);

  if (quadrant_index != old_quadrant_index_) {

    // Increment the pattern step based on the nore duration
    note_ = Patterns[current_pattern_].notes[pattern_step_];
    switch (note_.duration) {
    case D16:
      IncPatternStep(Patterns[current_pattern_].length);
      break;
    case D8:
      if (quadrant_index == 1 || quadrant_index == 3)
        IncPatternStep(Patterns[current_pattern_].length);
      break;
    case D4:
      if (quadrant_index == 3)
        IncPatternStep(Patterns[current_pattern_].length);
      break;
    }

    old_quadrant_index_ = quadrant_index;
  }

  // Set the gate according to note duration, phase and duty
  if (note_.duration == D16) {
    if (phase_ - (HALFPI_F * quadrant_index) < pw_rad_ / 4.0f)
      out = note_.active ? 1.0f : 0.0f;
    else
      out = 0;
  } else if (note_.duration == D8) {
    quadrant_index /= 2.0f;
    if (phase_ - (PI_F * quadrant_index) < pw_rad_ / 2.0f)
      out = note_.active ? 1.0f : 0.0f;
    else
      out = 0;
  } else {
    quadrant_index /= 4.0f;
    if (phase_ < pw_rad_)
      out = note_.active ? 1.0f : 0.0f;
    else
      out = 0;
  }

  phase_ += phase_inc_;

  if (phase_ > TWOPI_F) {
    phase_ -= TWOPI_F;
    eoc_ = true;
  } else {
    eoc_ = false;
  }
  eor_ = (phase_ - phase_inc_ < PI_F && phase_ >= PI_F);

  return out;
}
