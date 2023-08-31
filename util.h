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
    void BlinkLED(RGB rgb, int delay = 140);

  private:
    daisy::DaisyPod *pod_;

    void BlinkLEDColor(RGB rgb, int delay);
};


} // namespace bytebeat

#endif
#endif