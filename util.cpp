#include "daisy_pod.h"
#include "basic_exp.h"
#include "util.h"

using namespace daisy;
using namespace bytebeat;

void Utilities::Init(BasicExp *pod) { pod_ = pod; }

void Utilities::BlinkLEDColor(float r, float g, float b, unsigned long delay)
{
  pod_->led1.Set(r, g, b);
  pod_->UpdateLeds();
  System::Delay(delay);
}

void Utilities::BlinkLED(float r, float g, float b, unsigned long delay, size_t number)
{
  for (size_t i = 0; i < number; i++) {
    BlinkLEDColor(r, g, b, delay);
    BlinkLEDColor(0, 0, 0, delay);
  }
}