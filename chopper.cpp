#include "chopper.h"

using namespace bytebeat;

constexpr float TWO_PI_RECIP = 1.0f / TWOPI_F;

float Chopper::Process()
{
    float out = phase_ < pw_rad_ ? (1.0f) : -1.0f;

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
