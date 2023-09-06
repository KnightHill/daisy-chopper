#pragma once
#ifndef DSY_METRO_H
#define DSY_METRO_H
#include <stdint.h>
#ifdef __cplusplus

namespace bytebeat
{

typedef void (*MetroCallback)(uint8_t quadrant);

/** Creates a clock signal at a specific frequency.
 */
class Metro16
{
public:
  Metro16() {}
  ~Metro16() {}
  void Init(float freq, float sample_rate);
  bool Process(MetroCallback callback = NULL);

  /** resets phase to 0
   */
  inline void Reset() { quadrant_index_ = phase_ = 0.0f; }
  void SetFreq(float freq);
  inline float GetFreq() { return freq_; }

private:
  float freq_;
  float phase_, sample_rate_, phase_inc_;
  float quadrant_index_;
};
} // namespace bytebeat
#endif
#endif
