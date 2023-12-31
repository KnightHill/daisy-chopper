#pragma once
#ifndef EVENT_H
#define EVENT_H
#ifdef __cplusplus

namespace bytebeat
{

enum EventType { Sync };

struct __attribute__((__packed__)) Event {
  EventType eventType;
  uint32_t timestamp; // timestamp in milliseconds
};

} // namespace bytebeat

#endif
#endif