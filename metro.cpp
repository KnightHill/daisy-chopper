#include <math.h>
#include "metro.h"
#include "Utility/dsp.h"

using namespace bytebeat;

void Metro16::Init(float freq, float sample_rate)
{
  freq_ = freq;
  phase_ = 0.0f;
  sample_rate_ = sample_rate;
  phase_inc_ = (TWOPI_F * freq_) / sample_rate_;
  quadrant_index_ = 0.0f;
}

bool Metro16::Process(MetroCallback callback)
{
  float quadrant_index = floorf(phase_ / HALFPI_F);

  if (quadrant_index != quadrant_index_) {
    if (callback)
      callback(quadrant_index);

    quadrant_index_ = quadrant_index;
    return true;
  }

  phase_ += phase_inc_;
  if (phase_ >= TWOPI_F) {
    phase_ -= TWOPI_F;
    // return 1;
  }

  return false;
}

void Metro16::SetFreq(float freq)
{
  freq_ = freq;
  phase_inc_ = (TWOPI_F * freq_) / sample_rate_;
}
