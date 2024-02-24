#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

extern uint8_t scancode[2];
extern gid_t ctr;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  
  int ipc_status;
  message msg;
  uint8_t r;
  uint8_t bit_no;
  uint8_t esc_found = 1;

  if (kbc_subscribe_int(&bit_no)) return 1;

  while (esc_found) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(bit_no)) {
            kbc_ih();
            if (scancode[1] == ESC_MAKE) {
              esc_found = 0;
              break;
            }
            if (scancode[1] == SPECIAL_KEY) {
              scancode[0] = scancode[1];
              break;
            }

            else {
              if (scancode[0]) {
                if (scancode[1] & 0x80)
                  kbd_print_scancode(BREAK, 2, &scancode[0]);
                else
                  kbd_print_scancode(MAKE, 2, &scancode[0]);
                scancode[0] = 0;
              }
              else {
                if (scancode[1] & 0x80)
                  kbd_print_scancode(BREAK, 1, &scancode[1]);
                else
                  kbd_print_scancode(MAKE, 1, &scancode[1]);
              }
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbc_unsubscribe_int()) return 1;

  return 0;
}

int(kbd_test_poll)() {
  uint8_t esc_found = 1;

  while (esc_found) {
    kbc_ih();
    if (scancode[1] == ESC_MAKE) {
      esc_found = 0;
      break;
    }
    if (scancode[1] == SPECIAL_KEY) {
      scancode[0] = scancode[1];
      continue;
    }

    else {
      if (scancode[0]) {
        if (scancode[1] & 0x80)
          kbd_print_scancode(BREAK, 2, &scancode[0]);
        else
          kbd_print_scancode(MAKE, 2, &scancode[0]);
        scancode[0] = 0;
      }
      else {
        if (scancode[1] & 0x80)
          kbd_print_scancode(BREAK, 1, &scancode[1]);
        else
          kbd_print_scancode(MAKE, 1, &scancode[1]);
      }
    }
  }

  if (kbd_reenable_interrupts()) return 1;

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status;
  message msg;
  uint8_t r;
  uint8_t kbc_bit_no;
  uint8_t timer_bit_no;
  uint8_t esc_found = 1;
  uint8_t idle = n;

  if (kbc_subscribe_int(&kbc_bit_no)) return 1;
  if (timer_subscribe_int(&timer_bit_no)) return 1;

  while (esc_found) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(kbc_bit_no)) {
            idle = n;
            kbc_ih();
            if (scancode[1] == ESC_MAKE) {
              esc_found = 0;
              break;
            }
            if (scancode[1] == SPECIAL_KEY) {
              scancode[0] = scancode[1];
              break;
            }

            else {
              if (scancode[0]) {
                if (scancode[1] & 0x80)
                  kbd_print_scancode(BREAK, 2, &scancode[0]);
                else
                  kbd_print_scancode(MAKE, 2, &scancode[0]);
                scancode[0] = 0;
              }
              else {
                if (scancode[1] & 0x80)
                  kbd_print_scancode(BREAK, 1, &scancode[1]);
                else
                  kbd_print_scancode(MAKE, 1, &scancode[1]);
              }
            }
          }

          if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
            timer_int_handler();
            if (counter == 60) {
              counter = 0;
              idle--;
              //timer_print_elapsed_time();
            }
            if (idle == 0) {
              esc_found = 0;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbc_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;

  return 0;
}
