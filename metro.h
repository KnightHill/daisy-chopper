#pragma once
#ifndef DSY_METRO16_H
#define DSY_METRO16_H
#include <stdint.h>
#ifdef __cplusplus

namespace bytebeat
{

typedef void (*MetroCallback)(float phase, uint8_t quadrant);

/** Creates a clock signal at a specific frequency.
 */
class Metro16
{
public:
  Metro16() {}
  ~Metro16() {}
  void Init(float freq, float sample_rate, MetroCallback callback = NULL);
  bool Process();

  inline void Reset() { quadrant_index_ = phase_ = 0.0f; }
  void SetFreq(float freq);
  inline float GetFreq() { return freq_; }

private:
  float freq_;
  float phase_, sample_rate_, phase_inc_;
  float quadrant_index_;
  MetroCallback callback_;
};

} // namespace bytebeat

#endif
#endif
