#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "Mouse/mouse.h"
#include "KeyBoard/kbc.h"
#include "Graphics/graphics.h"
#include "Timer/timer.h"
 
// Temp includes
#include "../ImageAssets/MouseCursor.xpm"
// End of temp includes

uint8_t timer_bit_no;
uint8_t kbc_bit_no;
uint8_t mouse_bit_no;

struct packet pp;
extern int counter;
extern uint8_t scancode[2];

extern unsigned h_res;
extern unsigned v_res;

extern unsigned bits_per_pixel;

int16_t mouse_position[2] = {0, 0};

int default_video_mode = 0x11B;

// Temp variables
xpm_image_t mouse_cursor_xpm;
// End of temp variables

int boot_devices(uint32_t freq) {

  if (timer_initiate_and_subscribe(&timer_bit_no, freq)) return 1;

  if (mouse_initiate(&mouse_bit_no)) return 1;

  if (kbc_subscribe_int(&kbc_bit_no)) return 1;

  if (vg_init(default_video_mode) == NULL) return 1;

  // Temp section
  if (xpm_load((xpm_map_t) MouseCursor, XPM_8_8_8, &mouse_cursor_xpm) == NULL) return 1;
  mouse_position[0] = h_res / 2;
  mouse_position[1] = v_res / 2;
  // End of temp section

  return 0;
}

int stop_devices() {

  if (kbc_unsubscribe_int()) return 1;

  if (mouse_terminate()) return 1;
     
  if (timer_unsubscribe_int()) return 1;

  if (vg_free()) return 1;
  if (vg_exit()) return 1;

  return 0;
}

int interrupt_handler(uint32_t interrupt_mask) {
  if (interrupt_mask & BIT(mouse_bit_no)) {
    mouse_ih();
    if (mouse_get_info(&pp)) {

      mouse_position[0] += pp.delta_x;
      mouse_position[1] -= pp.delta_y;

      if (mouse_position[0] < 0) mouse_position[0] = 0;
      if (mouse_position[0] >= (int16_t) h_res) mouse_position[0] = h_res - 1;
      if (mouse_position[1] < 0) mouse_position[1] = 0;
      if (mouse_position[1] >= (int16_t) v_res) mouse_position[1] = v_res - 1;
    }
  }
  
  if (interrupt_mask & BIT(timer_bit_no)) {
    timer_int_handler();
    if (vg_draw_xpm(mouse_position[0], mouse_position[1], 
                    mouse_cursor_xpm, (bits_per_pixel + 7)/8)) return 1;
    if (vg_replace_buffer()) return 1;
  }

  if (interrupt_mask & BIT(kbc_bit_no)) {
    kbc_ih();
    if (scancode[1] == 0x81) {
      return 1;
    }
  }

  return 0;
}
