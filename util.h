#pragma once
#ifndef UTIL_H
#define UTIL_H
#ifdef __cplusplus

namespace bytebeat
{

struct RGB
{
    float R, G, B;
};

class Utilities
{
  public:
    Utilities() {}
    void Init(daisy::DaisyPod *);
    void BlinkLED(RGB rgb, unsigned long delay = 140, size_t number = 3);

  private:
    daisy::DaisyPod *pod_;

    void BlinkLEDColor(RGB rgb, unsigned long delay);
};


} // namespace bytebeat

#endif
#endif