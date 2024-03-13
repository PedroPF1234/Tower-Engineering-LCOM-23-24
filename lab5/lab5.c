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
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

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
  uint8_t timer_bit_no;
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ax = 0x4F02;
  reg86.bx = BIT(14) | mode;
  if (sys_int86(&reg86)) return 1;

  if (timer_subscribe_int(&timer_bit_no)) return 1;

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
          if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
            timer_int_handler();
            if (counter >= delay * 60) running = false;
          }
          break;
        default:
          break;
      }
    }
  }

  if (timer_unsubscribe_int()) return 1;
  if (vg_exit()) return 1;

  return 1;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  
  uint8_t kbc_bit_no;

  if (vg_init(mode) == NULL) return 1;
  if (kbc_subscribe_int(&kbc_bit_no)) return 1;

  if (vg_draw_rectangle(x, y, width, height, color)) return 1;

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

  return 1;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  
  uint8_t kbc_bit_no;

  if (vg_init(mode) == NULL) return 1;
  if (kbc_subscribe_int(&kbc_bit_no)) return 1;

  uint16_t h_size = h_res / no_rectangles;
  uint16_t v_size = v_res / no_rectangles;

  uint32_t color = 0;

  if (current_mode == MODE_INDEXED) {
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

      }
    }
  }

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

  return 1;

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
