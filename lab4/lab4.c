// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"

extern struct packet pp;
extern int counter;

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
  int r;
  uint8_t mouse_bit_no;

  if (mouse_write_cmdb(KBC_MOUSE_ENABLE_STREAM_MODE_REPORTING)) return 1;
  if (mouse_subscribe_int(&mouse_bit_no)) return 1;

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

  if (mouse_unsubscribe_int()) return 1;
  if (mouse_write_cmdb(KBC_MOUSE_DISABLE_STREAM_MODE_REPORTING)) return 1;

  return 0;
}


int (mouse_test_async)(uint8_t idle_time) {
  uint8_t mouse_bit_no;
  uint8_t timer_bit_no;
  uint8_t idle = idle_time;
  int32_t interrupt_freq = sys_hz();

  if (mouse_write_cmdb(KBC_MOUSE_ENABLE_STREAM_MODE_REPORTING)) return 1;
  if (mouse_subscribe_int(&mouse_bit_no)) return 1;
  if (timer_subscribe_int(&timer_bit_no)) return 1;

  int ipc_status;
  message msg;
  int r;

  while (idle) {
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
            }
            idle = idle_time;
          }
          if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
            timer_int_handler();
            if (counter % interrupt_freq == 0) {
              idle--;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (mouse_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;
  if (mouse_write_cmdb(KBC_MOUSE_DISABLE_STREAM_MODE_REPORTING)) return 1;

  return 0;
}


int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  message msg;
  int r;
  uint8_t mouse_bit_no;
  bool finished_gesture = false;

  if (mouse_write_cmdb(KBC_MOUSE_ENABLE_STREAM_MODE_REPORTING)) return 1;
  if (mouse_subscribe_int(&mouse_bit_no)) return 1;

  while (!finished_gesture) {
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
              finished_gesture = mouse_gesture_event(&pp, x_len, tolerance);
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (mouse_unsubscribe_int()) return 1;
  if (mouse_write_cmdb(KBC_MOUSE_DISABLE_STREAM_MODE_REPORTING)) return 1;

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  uint8_t mouse_bit_no;
  uint8_t timer_bit_no;
  int32_t interrupt_freq = sys_hz();
  int ipc_status;
  message msg;
  int r;
  int delay = period;

  if (mouse_write_cmdb(KBC_MOUSE_SET_REMOTE_MODE)) return 1;

  if (mouse_subscribe_int(&mouse_bit_no)) return 1;
  if (timer_subscribe_int(&timer_bit_no)) return 1;


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
            }
          }
          if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
            timer_int_handler();
            if (counter == interrupt_freq) {
              counter = 0;
              delay--;
            }
            if (delay == 0) {
              delay = period;
              cnt--;
              if (mouse_write_cmdb(KBC_MOUSE_READ_DATA)) return 1;
              uint8_t data;
              if (mouse_read_cmdb(&data)) return 1;
              mouse_print_packet(&pp);
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (mouse_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;
  if (mouse_write_cmdb(KBC_MOUSE_SET_STREAM_MODE)) return 1;

  minix_get_dflt_kbc_cmd_byte();

  return 0;
}

