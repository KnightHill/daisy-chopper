/*
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
*/
