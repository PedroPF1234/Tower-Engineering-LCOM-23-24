// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "../lab3/i8042.h"
#include "mouse.h"

extern struct packet pp;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  message msg;
  uint8_t r;
  uint8_t mouse_bit_no;

  if (mouse_subscribe_int(&mouse_bit_no)) return 1;
  if (mouse_enable_data_reporting()) return 1;

  while (cnt) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
            mouse_ih();
            if (pp.bytes[0] & BIT(3)) {
              mouse_print_packet(&pp);
              cnt--;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (mouse_disable_data_reporting()) return 1;
  if (mouse_unsubscribe_int()) return 1;

  return 0;
}

/*
int (mouse_test_async)(uint8_t idle_time) {
    return 1;
}
*/

/*
int (mouse_test_gesture)() {
    return 1;
}
*/

/*
int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    return 1;
}
*/
