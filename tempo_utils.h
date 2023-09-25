
#pragma once
#ifndef TEMPO_UTILS_H
#define TEMPO_UTILS_H
#ifdef __cplusplus

namespace bytebeat
{

class TempoUtils
{
public:
  static float tempo_to_freq(uint8_t tempo) { return tempo / 60.0f; }
  static uint8_t freq_to_tempo(float freq) { return freq * 60.0f; }
  static float bpm_to_freq(uint32_t tempo) { return tempo / 60.0f; }
  static uint32_t ms_to_bpm(uint32_t ms) { return 60000 / ms; }
};

} // namespace bytebeat
#endif
#endif
