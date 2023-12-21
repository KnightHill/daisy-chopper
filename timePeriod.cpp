#include "daisy_pod.h"
#include "timePeriod.h"

using namespace bytebeat;
using namespace daisy;

void TimePeriod::Init(uint32_t duration)
{
  start_ = System::GetNow();
  duration_ = duration;
}

bool TimePeriod::Elapsed() { return System::GetNow() > start_ + duration_; }
