#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint8_t second; // Seconds
  uint8_t minute; // Minutes
  uint8_t hour; // Hour
  uint8_t day; // Day
  uint8_t month; // Month
  uint32_t year; // Year
  bool just_updated; // Just Updated
} RTC_Time;
