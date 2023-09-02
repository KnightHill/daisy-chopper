#pragma once
#ifndef UTIL_H
#define UTIL_H
#ifdef __cplusplus

namespace bytebeat
{

class Utilities
{
  public:
    Utilities() {}
    void Init(daisy::DaisyPod *);
    void BlinkLED(float         r      = 1.0f,
                  float         g      = 1.0f,
                  float         b      = 1.0f,
                  unsigned long delay  = 140,
                  size_t        number = 3);

  private:
    daisy::DaisyPod *pod_;

    void BlinkLEDColor(float r, float g, float b, unsigned long delay);
};


} // namespace bytebeat

#endif
#endif