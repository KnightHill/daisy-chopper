#include "chopper.h"

using namespace bytebeat;

constexpr float TWO_PI_RECIP = 1.0f / TWOPI_F;
constexpr float HALF_PI      = TWOPI_F / 4.0f;

// 1/4 notes
uint8_t Chopper::Patterns[PATTERNS_MAX][PATTERN_STEPS_MAX] = {

    {1, 1, 1, 1},
    {1, 0, 1, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {1, 1, 1, 0},
    {1, 0, 0, 1},
    {0, 1, 1, 0}

};

// 1/8 Notes

void Chopper::Init(float sample_rate)
{
    sr_              = sample_rate;
    sr_recip_        = 1.0f / sample_rate;
    freq_            = 100.0f;
    amp_             = 0.5f;
    pw_              = 0.5f;
    pw_rad_          = pw_ * TWOPI_F;
    phase_           = 0.0f;
    phase_inc_       = CalcPhaseInc(freq_);
    eoc_             = true;
    eor_             = true;
    current_pattern_ = 0;
    pattern_step_    = 0;
    mode_            = Quarter;
    tick_            = 0;
}

void Chopper::Reset(float _phase)
{
    phase_        = _phase;
    pattern_step_ = 0;
    tick_         = 0;
}

void Chopper::IncPatternStep()
{
    if(++pattern_step_ >= PATTERN_STEPS_MAX)
        pattern_step_ = 0;
}

float Chopper::Process()
{
    float out;

    if(phase_ < pw_rad_)
    {
        out = Patterns[current_pattern_][pattern_step_] ? 1.0f : 0.0f;
    }
    else
    {
        out = 0;
    }

    if(fmod(phase_, HALF_PI) + phase_inc_ > HALF_PI)
    {
        // ticks every 16th note
        tick_++;
    }

    switch(mode_)
    {
        case Quarter:
            if(tick_ % 4 == 0)
            {
                IncPatternStep();
            }
            break;
        case Eight:
            if(tick_ % 2 == 0)
            {
                IncPatternStep();
            }
            break;
        case Sixteen: IncPatternStep(); break;
    }

    phase_ += phase_inc_;

    if(phase_ > TWOPI_F)
    {
        phase_ -= TWOPI_F;
        eoc_ = true;
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
