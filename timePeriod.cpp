#include "daisy_pod.h"
#include "timePeriod.h"

using namespace bytebeat;
using namespace daisy;

void TimePeriod::Init(uint32_t duration)
{
  m_start = System::GetNow();
  m_duration = duration;
}

bool TimePeriod::Elapsed() { return System::GetNow() > m_start + m_duration; }
