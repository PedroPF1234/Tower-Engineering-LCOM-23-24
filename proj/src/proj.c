// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "devices/i8042.h"
#include "devices/i8254.h"
#include "devices/mouse.h"
#include "devices/kbc.h"
#include "devices/vbe_interface.h"
#include "devices/graphics.h"

extern struct packet pp;
extern int counter;
extern uint8_t scancode[2];

uint8_t packet_to_read = 0;
uint8_t packet[3] = {0, 0, 0};
extern uint8_t mouse_byte;

extern unsigned h_res;
extern unsigned v_res;

extern unsigned bits_per_pixel;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char **argv) {
  
  uint8_t timer_bit_no;
  uint8_t kbc_bit_no;
  uint8_t mouse_bit_no;

  if (timer_set_frequency(0, 240)) return 1;
  if (timer_subscribe_int(&timer_bit_no)) return 1;
  if (mouse_write_cmdb(KBC_MOUSE_ENABLE_STREAM_MODE_REPORTING)) return 1;
  if (kbc_subscribe_int(&kbc_bit_no)) return 1;
  if (mouse_subscribe_int(&mouse_bit_no)) return 1;
  
  int ipc_status;
  message msg;
  int r;

  if (vg_init(0x11B) == NULL) return 1;

  xpm_image_t mouse_cursor_xpm;
  if (xpm_load((xpm_map_t) temp_cursor, XPM_8_8_8, &mouse_cursor_xpm) == NULL) return 1;

  int16_t mouse_position[2] = {h_res/2,v_res/2};

  bool running = true;

  while (running) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
            mouse_ih();
            if (packet_to_read == 0) {
              memset(&pp, 0, sizeof(pp));
              if (mouse_byte & BIT(3)) {
                packet[0] = mouse_byte;
                packet_to_read++;
              }
            } else if (packet_to_read == 1) {
              packet[1] = mouse_byte;
              packet_to_read++;
            } else {
              packet[2] = mouse_byte;
              packet_to_read = 0;
              pp.bytes[0] = packet[0];
              pp.bytes[1] = packet[1];
              pp.bytes[2] = packet[2];

              pp.lb = packet[0] & BIT(0);
              pp.rb = packet[0] & BIT(1);
              pp.mb = packet[0] & BIT(2);
              pp.x_ov = packet[0] & BIT(6);
              pp.y_ov = packet[0] & BIT(7);

              if (packet[0] & BIT(4)) {
                pp.delta_x = packet[1] - 256;
              } else {
                pp.delta_x = packet[1];
              }

              if (packet[0] & BIT(5)) {
                pp.delta_y = packet[2] - 256;
              } else {
                pp.delta_y = packet[2];
              }
              memset(&packet, 0, sizeof(packet));
              mouse_position[0] += pp.delta_x;
              mouse_position[1] -= pp.delta_y;
              if (mouse_position[0] < 0) mouse_position[0] = 0;
              if (mouse_position[0] >= (int16_t) h_res) mouse_position[0] = h_res - 1;
              if (mouse_position[1] < 0) mouse_position[1] = 0;
              if (mouse_position[1] >= (int16_t) v_res) mouse_position[1] = v_res - 1;
            }
          }
          
          if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
            timer_int_handler();
            if (vg_draw_xpm(mouse_position[0], mouse_position[1], 
                            mouse_cursor_xpm, (bits_per_pixel + 7)/8)) return 1;
            if (vg_replace_buffer()) return 1;
          }

          if (msg.m_notify.interrupts & BIT(kbc_bit_no)) {
            kbc_ih();
            if (scancode[1] == ESC_BREAK) {
              running = false;
            }
          }

          break;
        default:
          break;
      }
    }
  }

  if (mouse_unsubscribe_int()) return 1;
  if (kbc_unsubscribe_int()) return 1;
  if (mouse_write_cmdb(KBC_MOUSE_DISABLE_STREAM_MODE_REPORTING)) return 1;
  if (timer_unsubscribe_int()) return 1;
  if (vg_free()) return 1;
  if (vg_exit()) return 1;

  return 0;
}
