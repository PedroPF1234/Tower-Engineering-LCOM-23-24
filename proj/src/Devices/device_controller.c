#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "Mouse/mouse.h"
#include "KeyBoard/kbc.h"
#include "Graphics/graphics.h"
#include "Timer/timer.h"
#include "device_controller.h"

#include "../GameObjects/gameobject.h"

// Mouse Game Object
extern GameObject_t* mouse;
 
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

static int default_video_mode = 0x11B;

int boot_devices(uint32_t freq, uint16_t mode) {

  if (create_gameobjects()) return 1;

  if (timer_initiate_and_subscribe(&timer_bit_no, freq)) return 1;

  if (mouse_initiate(&mouse_bit_no)) return 1;

  if (kbc_subscribe_int(&kbc_bit_no)) return 1;

  if (mode) {
    if (vg_init(mode) == NULL) return 1;
  } else {
    if (vg_init(default_video_mode) == NULL) return 1;
  }

  mouse->x = h_res / 2;
  mouse->y = v_res / 2;

  return 0;
}

int stop_devices() {

  if (destroy_gameobjects()) return 1;

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

      mouse->x += pp.delta_x;
      mouse->y -= pp.delta_y;

      if (mouse->x < 0) mouse->x = 0;
      if (mouse->x >= (int16_t) h_res) mouse->x = h_res - 1;
      if (mouse->y < 0) mouse->y = 0;
      if (mouse->y >= (int16_t) v_res) mouse->y = v_res - 1;
    }
  }
  
  if (interrupt_mask & BIT(timer_bit_no)) {
    timer_int_handler();
    if (draw_gameObject(mouse)) return 1;
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
