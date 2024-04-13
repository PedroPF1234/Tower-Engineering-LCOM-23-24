#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "Mouse/mouse.h"
#include "KeyBoard/kbc.h"
#include "Graphics/graphics.h"
#include "Timer/timer.h"
#include "device_controller.h"
 
// Device Interrupt Bit Masks
static uint8_t timer_bit_no;
static uint8_t kbc_bit_no;
static uint8_t mouse_bit_no;

static struct packet pp;
extern int counter;
extern uint8_t scancode[2];

extern unsigned h_res;
extern unsigned v_res;

extern uint8_t bytes_per_pixel;

extern Mouse mouse; 

static int default_video_mode = 0x11B;

int boot_devices(uint32_t freq, uint16_t mode) {

  if (timer_initiate_and_subscribe(&timer_bit_no, freq)) return 1;

  if (mouse_initiate(&mouse_bit_no)) return 1;

  if (kbc_subscribe_int(&kbc_bit_no)) return 1;

  if (mode) {
    if (vg_init(mode) == NULL) return 1;
  } else {
    if (vg_init(default_video_mode) == NULL) return 1;
  }

  mouse.x_position = h_res / 2;
  mouse.y_position = v_res / 2;

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

      mouse.x_position += pp.delta_x;
      mouse.y_position -= pp.delta_y;

      if (mouse.x_position < 0) mouse.x_position = 0;
      if (mouse.x_position >= (int16_t) h_res) mouse.x_position = h_res - 1;
      if (mouse.y_position < 0) mouse.y_position = 0;
      if (mouse.y_position >= (int16_t) v_res) mouse.y_position = v_res - 1;
    }
  }
  
  if (interrupt_mask & BIT(timer_bit_no)) {
    timer_int_handler();
    if (vg_draw_xpm(mouse.x_position, mouse.y_position, 
                    mouse.sprite->width, mouse.sprite->height, mouse.sprite->map)) return 1;
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

int update_timer_freq(uint32_t freq) {
  if (timer_unsubscribe_int()) return 1;
  if (timer_initiate_and_subscribe(&timer_bit_no, freq)) return 1;
  return 0;
}
