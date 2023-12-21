#pragma once
#ifndef TIME_PERIOD_H
#define TIME_PERIOD_H

namespace bytebeat
{

class TimePeriod
{
public:
  TimePeriod() {}
  ~TimePeriod() {}

  bool Process();
  void Init(uint32_t duration);
  inline uint32_t GetDuration() { return duration_; }

private:
  uint32_t start_;
  uint32_t duration_;
};

} // namespace bytebeat

#endif
