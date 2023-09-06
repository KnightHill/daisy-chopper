#include "chopper.h"

using namespace bytebeat;

constexpr float TWO_PI_RECIP = 1.0f / TWOPI_F;
/*
Pattern Chopper::Patterns[PATTERNS_MAX] = {
    {16, {{1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}}},
    {16, {{1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}}},
    {16, {{1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}}},
    {16, {{0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}}},
    {16, {{1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}}},
    {16, {{1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}}},
    //{16, {{1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}}},
    //{16, {{0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}}}
    {4, {{1, D4}, {1, D4}, {1, D4}, {1, D4}}},
    {8, {{1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}}}

};
*/

Pattern Chopper::Patterns[PATTERNS_MAX] = {
    {16, {{1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}, {1, D16}}},

    {16, {{1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}, {1, D16}, {0, D16}}},
    {8, {{1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}, {1, D8}}},

    {16, {{1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}, {1, D16}, {0, D16}, {0, D16}, {0, D16}}},
    {4, {{1, D4}, {1, D4}, {1, D4}, {1, D4}}},

    {16, {{0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}, {0, D16}}},
    {16, {{1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}, {1, D16}, {1, D16}, {0, D16}, {0, D16}}},
    {16, {{1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}, {1, D16}, {1, D16}, {1, D16}, {0, D16}}}

};

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
}

void Chopper::Reset(float _phase)
{
  phase_ = _phase;
  pattern_step_ = 0;
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
    pattern_step_ = 0;
}

void Chopper::PrevPattern(bool reset)
{
  current_pattern_--;
  if (current_pattern_ < 0)
    current_pattern_ = PATTERNS_MAX - 1;
  if (reset)
    pattern_step_ = 0;
}

uint16_t Chopper::GetStepIncrement(NoteDuration duration)
{
  switch (duration) {
  case D4:
    return 1;
  case D8:
    return 2;
  case D16:
  default:
    return 4;
  }
}

float Chopper::Process()
{
  float out;
  float quadrant_index = floorf(phase_ / HALFPI_F);
  Note note = Patterns[current_pattern_].notes[pattern_step_ + (uint16_t)quadrant_index];

  if (quadrant_index == 0)
    first_note_duration_ = note.duration;

  if (first_note_duration_ == D16) {
    if (phase_ - (HALFPI_F * quadrant_index) < pw_rad_ / 4.0f)
      out = note.active ? 1.0f : 0.0f;
    else
      out = 0;
  } else if (first_note_duration_ == D8) {
    quadrant_index /= 2.0f;
    Note note8 = Patterns[current_pattern_].notes[pattern_step_ + (uint16_t)quadrant_index];
    if (phase_ - (PI_F * quadrant_index) < pw_rad_ / 2.0f)
      out = note8.active ? 1.0f : 0.0f;
    else
      out = 0;
  } else {
    quadrant_index /= 4.0f;
    Note note4 = Patterns[current_pattern_].notes[pattern_step_ + (uint16_t)quadrant_index];
    if (phase_ < pw_rad_)
      out = note4.active ? 1.0f : 0.0f;
    else
      out = 0;
  }

  phase_ += phase_inc_;

  if (phase_ > TWOPI_F) {
    uint16_t step_increment = GetStepIncrement(first_note_duration_);
    phase_ -= TWOPI_F;
    eoc_ = true;
    for (uint16_t i = 0; i < step_increment; i++)
      IncPatternStep(Patterns[current_pattern_].length);
  } else {
    eoc_ = false;
  }
  eor_ = (phase_ - phase_inc_ < PI_F && phase_ >= PI_F);

  return out * amp_;
}

float Chopper::CalcPhaseInc(float f) { return (TWOPI_F * f) * sr_recip_; }
