#pragma once
#ifndef TIME_PERIOD_H
#define TIME_PERIOD_H

#ifdef __cplusplus

namespace bytebeat
{

class TimePeriod
{
public:
  TimePeriod() {}
  ~TimePeriod() {}

  /** initialize and sets duration in milliseconds */
  void Init(uint32_t duration);

  /** returns true if the current time is beyond period's duration */
  bool Elapsed();

  /** returns the time period duration in milliseconds */
  inline uint32_t GetDuration() { return m_duration; }

private:
  uint32_t m_start;
  uint32_t m_duration;
};

} // namespace bytebeat

#endif
#endif
