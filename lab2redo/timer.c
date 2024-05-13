#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = TIMER0_IRQ;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  if (freq > TIMER_FREQ || freq < 19 || timer < 0 || timer > 3) {
    return 1;
  }

  uint8_t st;
  if (timer_get_conf(timer, &st)) {
    return 1;
  }

  uint8_t configuration = (0x0F & st) | TIMER_LSB_MSB;

  switch (timer)
  {
  case 0:
    break;

  case 1:
    configuration |= TIMER_SEL1;
    break;
  
  case 2:
    configuration |= TIMER_SEL2;
    break;
  
  default:
    break;
  }

  if (sys_outb(TIMER_CTRL, configuration)) {
    return 1;
  }

  uint16_t new_freq = TIMER_FREQ / freq;

  uint8_t lsb, msb;

  if (util_get_LSB(new_freq, &lsb)) {
    return 1;
  }

  if (util_get_MSB(new_freq, &msb)) {
    return 1;
  }

  switch (timer)
  {
  case 0:
    if (sys_outb(TIMER_0, lsb)) {
      return 1;
    }
    if (sys_outb(TIMER_0, msb)) {
      return 1;
    }
    break;

  case 1:
    if (sys_outb(TIMER_1, lsb)) {
      return 1;
    }
    if (sys_outb(TIMER_1, msb)) {
      return 1;
    }
    break;
  
  case 2:
    if (sys_outb(TIMER_2, lsb)) {
      return 1;
    }
    if (sys_outb(TIMER_2, msb)) {
      return 1;
    }
    break;

  default:
    break;
  }

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  
  if (bit_no == NULL) {
    return 1;
  }

  *bit_no = hook_id;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) {
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {
  
  if (sys_irqrmpolicy(&hook_id)) {
    return 1;
  }

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (st == NULL) {
    return 1;
  }

  uint8_t control_word = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, control_word)) {
    return 1;
  }

  switch (timer) {
    case 0:
      if (util_sys_inb(TIMER_0, st)) {
        return 1;
      }
      break;

    case 1:
      if (util_sys_inb(TIMER_1, st)) {
        return 1;
      }
      break;
    
    case 2:
      if (util_sys_inb(TIMER_2, st)) {
        return 1;
      }
      break;
    
    default:
      return 1;
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
  union timer_status_field_val val;
  uint8_t count_mode = 0;
  uint8_t initial = 0;

  switch (field) {
    case tsf_all:
      val.byte = st;
      break;
    
    case tsf_base:
      val.bcd = st & BIT(0);
      break;

    case tsf_mode:

      count_mode = (st >> 1) & 0x07;

      if (count_mode == 6) count_mode = 2;
      else if (count_mode == 7) count_mode = 3;

      val.count_mode = count_mode;
      break;
    
    case tsf_initial:

      initial = (st >> 4) & 0x03;

      switch (initial) {
        case 1:
          val.in_mode = LSB_only;
          break;
        
        case 2:
          val.in_mode = MSB_only;
          break;
        
        case 3:
          val.in_mode = MSB_after_LSB;
          break;
        
        default:
          val.in_mode = INVAL_val;
          break;
      }
      break;
    
    default:
      break;
  }

  if (timer_print_config(timer, field, val)) {
    return 1;
  }

  return 0;
}
