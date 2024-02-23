#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st;
  if (timer_get_conf(timer, &st)) return 1;

  uint8_t control_word = TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN;

  switch (timer)
  {
  case 0:
    control_word |= TIMER_SEL0;
    break;

  case 1:
    control_word |= TIMER_SEL1;
    break;

  case 2:
    control_word |= TIMER_SEL2;
    break;
  
  default:
    return 1;
    break;
  }

  if (sys_outb(TIMER_CTRL, control_word)) return 1;

  uint16_t div = TIMER_FREQ / freq;

  uint8_t lsb, msb;
  if (util_get_LSB(div, &lsb)) return 1;
  if (util_get_MSB(div, &msb)) return 1;

  if (sys_outb(TIMER_0 + timer, lsb)) return 1;
  if (sys_outb(TIMER_0 + timer, msb)) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return 1;
  
  
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)) return 1;
  return 1;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t read_back_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, read_back_command)) return 1;
  
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
  union timer_status_field_val conf;

  switch (field)
  {
  case tsf_all:
    conf.byte = st;
    break;

  case tsf_initial:
    conf.in_mode = (st & (BIT(5) | BIT(4))) >> 4;
    break;

  case tsf_mode:
    conf.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
    break;

  case tsf_base:
    conf.bcd = st & BIT(0);
    break;
  
  default:
    return 1;
    break;
  }

  timer_print_config(timer, field, conf);

  return 0;
}
