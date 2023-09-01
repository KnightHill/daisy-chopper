#include "chopper.h"

using namespace bytebeat;

constexpr float TWO_PI_RECIP = 1.0f / TWOPI_F;

uint8_t Chopper::Patterns[PATTERNS_MAX][PATTERN_STEPS_MAX]
    = {{0, 0, 0, 0, 0, 0, 0, 0},
       {1, 0, 0, 0, 0, 0, 0, 0},
       {1, 0, 0, 0, 1, 0, 0, 0},
       {1, 0, 1, 0, 1, 0, 1, 0},
       {1, 1, 1, 1, 1, 1, 1, 1},
       {1, 1, 0, 0, 1, 1, 0, 0}};

float Chopper::Process()
{
    float out;

    if(phase_ < pw_rad_)
    {
        out = Patterns[current_pattern_][pattern_step_] ? 1.0f : 0.0f;
    }
    else
    {
        out = Patterns[current_pattern_][pattern_step_] ? -1.0f : 0.0f;
    }

    phase_ += phase_inc_;
    if(phase_ > TWOPI_F)
    {
        phase_ -= TWOPI_F;
        eoc_ = true;
        pattern_step_++;
        if(pattern_step_ >= PATTERN_STEPS_MAX)
            pattern_step_ = 0;
    }
    else
    {
        eoc_ = false;
    }
    eor_ = (phase_ - phase_inc_ < PI_F && phase_ >= PI_F);

    return out * amp_;
}

float Chopper::CalcPhaseInc(float f)
{
    return (TWOPI_F * f) * sr_recip_;
}
