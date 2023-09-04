#pragma once
#ifndef DSY_METRO_H
#define DSY_METRO_H
#include <stdint.h>
#ifdef __cplusplus

namespace bytebeat
{
/** Creates a clock signal at a specific frequency.
 */
class Metro16
{
public:
  Metro16() {}
  ~Metro16() {}
  void Init(float freq, float sample_rate);
  uint8_t Process();

  /** resets phase to 0
   */
  inline void Reset() { quadrant_index_ = phs_ = 0.0f; }
  void SetFreq(float freq);
  inline float GetFreq() { return freq_; }

private:
  float freq_;
  float phs_, sample_rate_, phs_inc_;
  float quadrant_index_;
};
} // namespace bytebeat
#endif
#endif
