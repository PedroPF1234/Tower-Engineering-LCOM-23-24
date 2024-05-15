#pragma once

#include <lcom/lcf.h>

#define RTC_IRQ 8 // RTC IRQ line

// RTC Ports
#define RTC_ADDRESS_REGISTER 0x70 // RTC Address Register
#define RTC_DATA_REGISTER 0x71 // RTC Data Register

// RTC Constants
#define RTC_DELAY_MICROSECONDS 20000 // RTC Delay in microseconds
#define RTC_IGNORE_VALUE BIT(7) | BIT(6) // RTC Ignore Value
#define RTC_YEAR_ADD 2000 // RTC Year Addition

// RTC Addresses
#define RTC_ADDRESS_SECONDS 0 // RTC Address Seconds
#define RTC_ADDRESS_SEC_INTERRUPT 1 // RTC Address Seconds Interrupt
#define RTC_ADDRESS_MINUTES 2 // RTC Address Minutes
#define RTC_ADDRESS_MIN_INTERRUPT 3 // RTC Address Minutes Interrupt
#define RTC_ADDRESS_HOUR 4 // RTC Address Hour
#define RTC_ADDRESS_HOUR_INTERRUPT 5 // RTC Address Hour Interrupt
#define RTC_ADDRESS_DAY_WEEK 6 // RTC Address Day of Week 
#define RTC_ADDRESS_DAY_MONTH 7 // RTC Address Day of Month
#define RTC_ADDRESS_MONTH 8 // RTC Address Month
#define RTC_ADDRESS_YEAR 9 // RTC Address Year

// RTC Registers
#define RTC_REGISTER_A 10 // RTC Register A
#define RTC_REGISTER_B 11 // RTC Register B
#define RTC_REGISTER_C 12 // RTC Register C
#define RTC_REGISTER_D 13 // RTC Register D

// Register A bits
#define RTC_REGISTER_A_UIP BIT(7) // RTC Register A Update In Progress
#define RTC_RS3 BIT(3) // RTC Register A Rate Select 3
#define RTC_RS2 BIT(2) // RTC Register A Rate Select 2
#define RTC_RS1 BIT(1) // RTC Register A Rate Select 1
#define RTC_RS0 BIT(0) // RTC Register A Rate Select 0

// Register B bits
#define RTC_REGISTER_B_INHIBIT_UPDATES BIT(7) // RTC Register B Inhibit Updates
#define RTC_REGISTER_B_SET_PERIODIC BIT(6) // RTC Register B Set Periodic Interruptions
#define RTC_REGISTER_B_SET_ALARM BIT(5) // RTC Register B Set Alarm Interruptions
#define RTC_REGISTER_B_SET_UPDATE BIT(4) // RTC Register B Set Update Ended Interruptions

// Register C bits
#define RTC_REGISTER_C_GENERAL_INTERRUPT BIT(7) // RTC Register C General Interrupt Flag
#define RTC_REGISTER_C_PERIODIC_INTERRUPT BIT(6) // RTC Register C Periodic Interrupt
#define RTC_REGISTER_C_ALARM_INTERRUPT BIT(5) // RTC Register C Alarm Interrupt
#define RTC_REGISTER_C_UPDATE_INTERRUPT BIT(4) // RTC Register C Update Ended Interrupt

// Periodic Interrupts Values
#define RTC_RATE_NONE 0                                 // RTC Periodic Interrupt Rate None
#define RTC_RATE_122US (RTC_RS1 | RTC_RS0)             // RTC Periodic Interrupt Rate 122us
#define RTC_RATE_244US RTC_RS2                         // RTC Periodic Interrupt Rate 244us
#define RTC_RATE_488US (RTC_RS2 | RTC_RS0)             // RTC Periodic Interrupt Rate 244us
#define RTC_RATE_976US (RTC_RS2 | RTC_RS1)             // RTC Periodic Interrupt Rate 976us
#define RTC_RATE_1_95MS (RTC_RS2 | RTC_RS1 | RTC_RS0) // RTC Periodic Interrupt Rate 1.95ms
#define RTC_RATE_3_9MS (RTC_RS3)                       // RTC Periodic Interrupt Rate 3.9ms
#define RTC_RATE_7_8MS (RTC_RS3 | RTC_RS0)             // RTC Periodic Interrupt Rate 7.8ms
#define RTC_RATE_15_6MS (RTC_RS3 | RTC_RS1)           // RTC Periodic Interrupt Rate 15.6ms
#define RTC_RATE_31_2MS (RTC_RS3 | RTC_RS1 | RTC_RS0) // RTC Periodic Interrupt Rate 31.2ms
#define RTC_RATE_62_5MS (RTC_RS3 | RTC_RS2)           // RTC Periodic Interrupt Rate 62.5ms
#define RTC_RATE_125MS (RTC_RS3 | RTC_RS2 | RTC_RS0)   // RTC Periodic Interrupt Rate 125ms
#define RTC_RATE_250MS (RTC_RS3 | RTC_RS2 | RTC_RS1)   // RTC Periodic Interrupt Rate 250ms
#define RTC_RATE_500MS (RTC_RS3 | RTC_RS2 | RTC_RS1 | RTC_RS0) // RTC Periodic Interrupt Rate 500ms


typedef struct {
  uint8_t second; // Seconds
  uint8_t minute; // Minutes
  uint8_t hour; // Hour
  uint8_t day; // Day
  uint8_t month; // Month
  uint32_t year; // Year
  bool just_updated; // Just Updated
} RTC_Time;

int (rtc_read)(uint8_t reg, uint8_t *byte);
int (rtc_write)(uint8_t reg, uint8_t byte);
int (rtc_subscribe_int)(uint8_t *bit_no);
int (rtc_unsubscribe_int)();
int (rtc_toggle_periodic_int)(bool enable, uint8_t rate);
int (rtc_toggle_alarm_int)(bool enable);
int (rtc_toggle_update_int)(bool enable);
int (rtc_disable_all_ints)();

void(rtc_set_alarm_every_second)();

int rtc_initialize_and_subscribe(uint8_t *bit_no);
int rtc_disable_and_unsubscribe();
void rtc_ih();
