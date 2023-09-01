#include "daisy_pod.h"
#include "util.h"

using namespace daisy;
using namespace bytebeat;

void Utilities::Init(DaisyPod *pod)
{
    pod_ = pod;
}

void Utilities::BlinkLEDColor(RGB rgb, unsigned long delay)
{
    pod_->led1.Set(rgb.R, rgb.G, rgb.B);
    pod_->UpdateLeds();
    System::Delay(delay);
}

void Utilities::BlinkLED(RGB rgb, unsigned long delay, size_t number)
{
    for(size_t i = 0; i < number; i++)
    {
        BlinkLEDColor(rgb, delay);
        BlinkLEDColor({0, 0, 0}, delay);
    }
}