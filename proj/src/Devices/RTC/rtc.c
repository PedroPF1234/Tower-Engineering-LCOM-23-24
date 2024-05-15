#include <lcom/lcf.h>
#include <stdint.h>

#include "rtc.h"

static int rtc_hook_id = RTC_IRQ;

extern RTC_Time* rtc_time;

static uint8_t (bcd_to_binary)(uint8_t bcd) {
  return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/*
static uint8_t (binary_to_bcd)(uint8_t binary) {
  return ((binary / 10) << 4) | (binary % 10);
}
*/

static int (rtc_get_seconds)(uint8_t *seconds) {

  if (rtc_read(RTC_ADDRESS_SECONDS, seconds)) return 1;

  *seconds = bcd_to_binary(*seconds);

  return 0;
}

static int (rtc_get_minutes)(uint8_t *minutes) {
  if (rtc_read(RTC_ADDRESS_MINUTES, minutes)) return 1;

  *minutes = bcd_to_binary(*minutes);

  return 0;
}

static int (rtc_get_hour)(uint8_t *hour) {
  if (rtc_read(RTC_ADDRESS_HOUR, hour)) return 1;

  *hour = bcd_to_binary(*hour);

  return 0;
}

/*
static int (rtc_get_week_day)(uint8_t *day) {
  if (rtc_read(RTC_ADDRESS_DAY_WEEK, day)) return 1;

  *day = bcd_to_binary(*day);

  return 0;
}
*/

static int (rtc_get_day)(uint8_t *day) {
  if (rtc_read(RTC_ADDRESS_DAY_MONTH, day)) return 1;

  *day = bcd_to_binary(*day);

  return 0;
}

static int (rtc_get_month)(uint8_t *month) {
  if (rtc_read(RTC_ADDRESS_MONTH, month)) return 1;

  *month = bcd_to_binary(*month);

  return 0;
}

static int (rtc_get_year)(uint8_t *year) {
  if (rtc_read(RTC_ADDRESS_YEAR, year)) return 1;

  *year = bcd_to_binary(*year);

  return 0;
}

static int (rtc_await_valid_state)() {
  uint8_t reg_a = 0;

  while (1) {
    if (rtc_read(RTC_REGISTER_A, &reg_a)) return 1;

    if (!(reg_a & RTC_REGISTER_A_UIP)) break;

    tickdelay(micros_to_ticks(RTC_DELAY_MICROSECONDS));
  }

  return 0;
}

static int (rtc_get_current_time)() {

  uint8_t seconds, minutes, hour, day, month, year;

  if (rtc_await_valid_state()) return 1;
  if (rtc_get_seconds(&seconds)) return 1;
  if (rtc_get_minutes(&minutes)) return 1;
  if (rtc_get_hour(&hour)) return 1;
  if (rtc_get_day(&day)) return 1;
  if (rtc_get_month(&month)) return 1;
  if (rtc_get_year(&year)) return 1;

  rtc_time->second = seconds;
  rtc_time->minute = minutes;
  rtc_time->hour = hour;
  rtc_time->day = day;
  rtc_time->month = month;
  rtc_time->year = RTC_YEAR_ADD + (uint32_t)year;

  return 0;
}

int (rtc_read)(uint8_t reg, uint8_t *byte) {
  if (byte == NULL) {
    return 1;
  } 

  if (sys_outb(RTC_ADDRESS_REGISTER, reg)) return 1;

  if (util_sys_inb(RTC_DATA_REGISTER, byte)) return 1;
  return 0;
}

int (rtc_write)(uint8_t reg, uint8_t byte) {
  if (sys_outb(RTC_ADDRESS_REGISTER, reg)) return 1;

  if (sys_outb(RTC_DATA_REGISTER, byte)) return 1;

  return 0;
}

int (rtc_subscribe_int)(uint8_t *bit_no) {

  // Clearing interrupt flags
  uint8_t reg_c = 0;
  if (rtc_read(RTC_REGISTER_C, &reg_c)) return 1;

  if (bit_no == NULL) return 1;
  *bit_no = rtc_hook_id;

  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id)) return 1;

  return 0;
}

int (rtc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&rtc_hook_id)) return 1;
  return 0;
}

int (rtc_toggle_periodic_int)(bool enable, uint8_t rate) {
  uint8_t reg_a = 0;
  uint8_t reg_b = 0;

  if (rtc_read(RTC_REGISTER_A, &reg_a) != OK) {
      return !OK;
  }

  reg_a = (reg_a & 0xF0) | (rate & 0x0F);

  if (rtc_write(RTC_REGISTER_A, reg_a) != OK) {
      return !OK;
  }

  if (rtc_read(RTC_REGISTER_B, &reg_b)) return 1;

  if (enable) {
    reg_b |= RTC_REGISTER_B_SET_PERIODIC;
  } else {
    reg_b &= ~RTC_REGISTER_B_SET_PERIODIC;
  }

  if (rtc_write(RTC_REGISTER_B, reg_b)) return 1;

  return 0;

}

int (rtc_toggle_alarm_int)(bool enable) {
  uint8_t reg_b = 0;

  if (rtc_read(RTC_REGISTER_B, &reg_b)) return 1;

  if (enable) {
    reg_b |= RTC_REGISTER_B_SET_ALARM;
  } else {
    reg_b &= ~RTC_REGISTER_B_SET_ALARM;
  }

  if (rtc_write(RTC_REGISTER_B, reg_b)) return 1;

  return 0;
}

int (rtc_toggle_update_int)(bool enable) {
  uint8_t reg_b = 0;

  if (rtc_read(RTC_REGISTER_B, &reg_b)) return 1;

  if (enable) {
    reg_b |= RTC_REGISTER_B_SET_UPDATE;
  } else {
    reg_b &= ~RTC_REGISTER_B_SET_UPDATE;
  }

  if (rtc_write(RTC_REGISTER_B, reg_b)) return 1;

  return 0;
}

int (rtc_disable_all_ints)() {
  if (rtc_toggle_alarm_int(false)) return 1;
  if (rtc_toggle_periodic_int(false, 0)) return 1;
  if (rtc_toggle_update_int(false)) return 1;

  return 0;
}

void(rtc_set_alarm_every_second)() {
  rtc_await_valid_state();
  uint8_t byte = 0;
  rtc_read(RTC_REGISTER_B, &byte);
  byte |= RTC_REGISTER_B_INHIBIT_UPDATES;
  rtc_write(RTC_REGISTER_B, byte);
  rtc_write(RTC_ADDRESS_HOUR_INTERRUPT, RTC_IGNORE_VALUE);
  rtc_write(RTC_ADDRESS_MIN_INTERRUPT, RTC_IGNORE_VALUE);
  rtc_write(RTC_ADDRESS_SEC_INTERRUPT, RTC_IGNORE_VALUE);
  rtc_read(RTC_REGISTER_B, &byte);
  byte &= ~RTC_REGISTER_B_INHIBIT_UPDATES;
  rtc_write(RTC_REGISTER_B, byte);
}

int rtc_initialize_and_subscribe(uint8_t *bit_no) {
  if (rtc_subscribe_int(bit_no)) return 1;

  if (rtc_disable_all_ints()) return 1;

  if (rtc_toggle_periodic_int(true, RTC_RATE_500MS)) return 1;

  rtc_time = (RTC_Time*) malloc(sizeof(RTC_Time));

  return 0;
}

int rtc_disable_and_unsubscribe() {
  if (rtc_disable_all_ints()) return 1;

  if (rtc_unsubscribe_int()) return 1;

  free(rtc_time);

  return 0;
}

void rtc_ih() {
  uint8_t reg_c = 0;

  if (rtc_await_valid_state()) return;

  if (rtc_read(RTC_REGISTER_C, &reg_c)) return;

  if (reg_c & RTC_REGISTER_C_PERIODIC_INTERRUPT) {
    if (rtc_get_current_time()) return;
  }

  if (reg_c & RTC_REGISTER_C_ALARM_INTERRUPT) {
    return;
  }

  if (reg_c & RTC_REGISTER_C_UPDATE_INTERRUPT) {
    if (rtc_get_current_time()) return;
  }
}
