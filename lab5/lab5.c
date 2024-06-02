// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "vbe_interface.h"
#include "i8042.h"
#include "graphics.h"
#include "kbc.h"

extern int counter;
extern uint8_t scancode[2];

extern uint8_t red_pixel_mask;
extern uint8_t green_pixel_mask;
extern uint8_t blue_pixel_mask;

extern unsigned h_res;
extern unsigned v_res;

extern unsigned bits_per_pixel;

extern uint16_t current_mode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  // lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  if ((vg_init)(mode) == NULL) return 1;

  sleep(delay);

  if (vg_exit()) return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  
  uint8_t kbc_bit_no;

  if (vg_init(mode) == NULL) return 1;
  if (kbc_subscribe_int(&kbc_bit_no)) return 1;

  if (vg_draw_rectangle(x, y, width, height, color)) return 1;
  if (vg_replace_buffer()) return 1;

  int ipc_status;
  message msg;
  int r;

  bool running = true;

  while (running) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(kbc_bit_no)) {
            kbc_ih();
            if (scancode[1] == ESC_BREAK) {
              running = false;
            }
            if (scancode[1] == SPECIAL_KEY) {
              scancode[0] = scancode[1];
              break;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbc_unsubscribe_int()) return 1;
  if (vg_exit()) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  
  uint8_t kbc_bit_no;

  if (vg_init(mode) == NULL) return 1;
  if (kbc_subscribe_int(&kbc_bit_no)) return 1;

  uint16_t h_size = h_res / no_rectangles;
  uint16_t v_size = v_res / no_rectangles;

  uint32_t color = 0;

  if (current_mode == MODE_INDEXED_DEFAULT) {
    for (uint32_t i = 0; i < h_res; i+= h_size) {
      for (uint32_t j = 0; j < v_res; j += v_size) {

        color = (first + ((i / h_size) * no_rectangles + (j/v_size)) * step) % (1 << bits_per_pixel);
        if (vg_draw_rectangle(i, j, h_size, v_size, color));
      }
    }
  } else {
    for (uint32_t i = 0; i < h_res; i+= h_size) {
      for (uint32_t j = 0; j < v_res; j += v_size) {

        color |= ((uint8_t)first + (j/v_size) * step) % (1 << red_pixel_mask);
        color |= (((uint8_t)(first >> 8) + (i/h_size) * step) % (1 << green_pixel_mask)) << 8;
        color |= (((uint8_t)(first >> 16) + ((i/h_size) * (j/v_size)) * step) % (1 << blue_pixel_mask)) << 16;
        color |= 0xFF000000;

        if (vg_draw_rectangle(i, j, h_size, v_size, color));
      }
    }
  }

  if (vg_replace_buffer()) return 1;

  int ipc_status;
  message msg;
  int r;

  bool running = true;

  while (running) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(kbc_bit_no)) {
            kbc_ih();
            if (scancode[1] == ESC_BREAK) {
              running = false;
            }
            if (scancode[1] == SPECIAL_KEY) {
              scancode[0] = scancode[1];
              break;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbc_unsubscribe_int()) return 1;
  if (vg_exit()) return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
  uint8_t kbc_bit_no;

  xpm_image_t image;

  if (vg_init(MODE_INDEXED_DEFAULT) == NULL) return 1;
  if (kbc_subscribe_int(&kbc_bit_no)) return 1;

  if(xpm_load(xpm, XPM_INDEXED, &image) == NULL) return 1;

  if (vg_draw_xpm(x, y, image, (bits_per_pixel + 7)/8)) return 1;
  if (vg_replace_buffer()) return 1;

  int ipc_status;
  message msg;
  int r;

  bool running = true;

  while (running) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(kbc_bit_no)) {
            kbc_ih();
            if (scancode[1] == ESC_BREAK) {
              running = false;
            }
            if (scancode[1] == SPECIAL_KEY) {
              scancode[0] = scancode[1];
              break;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbc_unsubscribe_int()) return 1;
  if (vg_exit()) return 1;

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  
  uint8_t kbc_bit_no;
  uint8_t timer_bit_no;

  xpm_image_t image;

  uint16_t xp = xi;
  uint16_t yp = yi;
  int16_t xd, yd;
  uint16_t wait = 0;
  uint8_t rate_of_change = 60 / fr_rate;

  if (speed == 0) {
    panic("Speed is zero, sprite cannot move.\n");
    return 1;
  }

  if (xf > xi) xd = speed > 0 ? speed : 1;
  else if (xf < xi) xd = speed > 0 ? (0 - speed) : -1;
  else xd = 0;

  if (yf > yi) yd = speed > 0 ? speed : 1;
  else if (yf < yi) yd = speed > 0 ? (0 - speed) : -1;
  else yd = 0;

  wait = speed > 0 ? 0 : (0 - speed);

  if (vg_init(MODE_INDEXED_DEFAULT) == NULL) return 1;

  if(xpm_load(xpm, XPM_INDEXED, &image) == NULL) return 1;

  if (vg_draw_xpm(xi, yi, image, (bits_per_pixel + 7)/8)) return 1;
  if (vg_replace_buffer()) return 1;

  if (kbc_subscribe_int(&kbc_bit_no)) return 1;
  if (timer_subscribe_int(&timer_bit_no)) return 1;

  int ipc_status;
  message msg;
  int r;

  uint16_t tempwait = wait;

  bool running = true;

  while (running) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(kbc_bit_no)) {
            kbc_ih();
            if (scancode[1] == ESC_BREAK) {
              running = false;
            }
            if (scancode[1] == SPECIAL_KEY) {
              scancode[0] = scancode[1];
              break;
            }
          }

          if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
            timer_int_handler();
            if (counter % rate_of_change == 0) {
              if (tempwait) {
                tempwait--;
              } else {

                if (xd > xf - xp) xd = xf - xp;
                else xp += xd;
                if (yd > yf - yp) yd = yf - yp;
                else yp += yd;
                
                if (vg_draw_xpm(xp, yp, image, (bits_per_pixel + 7)/8)) return 1;
                if (vg_replace_buffer()) return 1;
                tempwait = wait;
              }
            }

          }
          break;
        default:
          break;
      }
    }
  }

  if (timer_unsubscribe_int()) return 1;
  if (kbc_unsubscribe_int()) return 1;
  if (vg_free()) return 1;
  if (vg_exit()) return 1;

  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
