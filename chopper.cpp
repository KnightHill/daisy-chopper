#include "chopper.h"

using namespace bytebeat;

constexpr float TWO_PI_RECIP = 1.0f / TWOPI_F;

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
    //tick_            = 0;
}

void Chopper::Reset(float _phase)
{
    phase_        = _phase;
    pattern_step_ = 0;
    //tick_         = 0;
}

void Chopper::IncPatternStep()
{
    if(++pattern_step_ >= PATTERN_STEPS_MAX)
        pattern_step_ = 0;
}

float Chopper::Process4()
{
    float out;

    if(phase_ < pw_rad_)
        out = Patterns[current_pattern_][pattern_step_] ? 1.0f : 0.0f;
    else
        out = 0;

    phase_ += phase_inc_;

    if(phase_ > TWOPI_F)
    {
        phase_ -= TWOPI_F;
        eoc_ = true;
        IncPatternStep();
    }
    else
    {
        eoc_ = false;
    }
    eor_ = (phase_ - phase_inc_ < PI_F && phase_ >= PI_F);

    return out * amp_;
}

float Chopper::Process8()
{
    float out;

    if(phase_ < PI_F)
    {
        if(phase_ < pw_rad_ / 2.0f)
            out = Patterns[current_pattern_][pattern_step_] ? 1.0f : 0.0f;
        else
            out = 0;
    }
    else
    {
        if(phase_ - PI_F < pw_rad_ / 2.0f)
            out = Patterns[current_pattern_][pattern_step_ + 1] ? 1.0f : 0.0f;
        else
            out = 0;
    }

    phase_ += phase_inc_;

    if(phase_ > TWOPI_F)
    {
        phase_ -= TWOPI_F;
        eoc_ = true;
        IncPatternStep();
        IncPatternStep();
    }
    else
    {
        eoc_ = false;
    }
    eor_ = (phase_ - phase_inc_ < PI_F && phase_ >= PI_F);

    return out * amp_;
}

float Chopper::Process16()
{
    float out;

    if(phase_ < HALFPI_F)
    {
        if(phase_ < pw_rad_ / 4.0f)
            out = Patterns[current_pattern_][pattern_step_] ? 1.0f : 0.0f;
        else
            out = 0;
    }
    else if(phase_ >= HALFPI_F && phase_ < PI_F)
    {
        if(phase_ - HALFPI_F < pw_rad_ / 4.0f)
            out = Patterns[current_pattern_][pattern_step_ + 1] ? 1.0f : 0.0f;
        else
            out = 0;
    }
    else if(phase_ >= PI_F && phase_ < PI_F + HALFPI_F)
    {
        if(phase_ - PI_F < pw_rad_ / 4.0f)
            out = Patterns[current_pattern_][pattern_step_ + 2] ? 1.0f : 0.0f;
        else
            out = 0;
    }
    else
    {
        if(phase_ - (PI_F + HALFPI_F) < pw_rad_ / 4.0f)
            out = Patterns[current_pattern_][pattern_step_ + 3] ? 1.0f : 0.0f;
        else
            out = 0;
    }

    phase_ += phase_inc_;

    if(phase_ > TWOPI_F)
    {
        phase_ -= TWOPI_F;
        eoc_ = true;
        IncPatternStep();
        IncPatternStep();
        IncPatternStep();
        IncPatternStep();
    }
    else
    {
        eoc_ = false;
    }
    eor_ = (phase_ - phase_inc_ < PI_F && phase_ >= PI_F);

    return out * amp_;
}

float Chopper::Process()
{
    float out;

    if(phase_ < pw_rad_)
        out = Patterns[current_pattern_][pattern_step_] ? 1.0f : 0.0f;
    else
        out = 0;

    /*
    // 1/4
    if(mode_ == Quarter)
    {
        float pphase = fmodf(phase_, TWOPI_F);
        if(pphase + phase_inc_ > TWOPI_F)
            IncPatternStep();
    }
    else if(mode_ == Eight)
    {
        // 1/8
        float pphase = fmodf(phase_, PI_F);
        if(pphase + phase_inc_ > PI_F)
            IncPatternStep();
    }
    else
    {
        // 1/16
        float pphase = fmodf(phase_, HALFPI_F);
        if(pphase + phase_inc_ > HALFPI_F)
            IncPatternStep();
    }
*/

    phase_ += phase_inc_;

    if(phase_ > TWOPI_F)
    {
        phase_ -= TWOPI_F;
        eoc_ = true;
        IncPatternStep();
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
