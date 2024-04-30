#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "../Game/game.h"

#include "Mouse/mouse.h"
#include "KeyBoard/kbc.h"
#include "Graphics/graphics.h"
#include "Timer/timer.h"
#include "device_controller.h"

// Mouse Game Object
extern GameObject* mouse;

// Global device interfaces
MouseDevice* mouse_device;
KeyboardDevice* keyboard_device;
 
// Device Interrupt Bit Masks
static uint8_t timer_bit_no;
static uint8_t kbc_bit_no;
static uint8_t mouse_bit_no;

static struct packet pp;
extern uint64_t counter;
extern uint8_t scancode[2];

extern unsigned h_res;
extern unsigned v_res;

static int default_video_mode = 0x11B;

static bool was_left_button_pressed = false;
static bool was_right_button_pressed = false;
static bool was_middle_button_pressed = false;

uint16_t FPS = 0;
static uint32_t frequency = 0;

int boot_devices(uint32_t freq, uint16_t framespersecond, uint16_t mode) {

  FPS = framespersecond;
  frequency = freq;

  if (create_gameobjects()) return 1;

  mouse_device = (MouseDevice*) malloc(sizeof(MouseDevice));
  memset(mouse_device, 0, sizeof(MouseDevice));
  mouse_device->mouse = mouse;

  keyboard_device = (KeyboardDevice*) malloc(sizeof(KeyboardDevice));
  memset(keyboard_device, 0, sizeof(KeyboardDevice));

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

  if ((interrupt_mask & (BIT(mouse_bit_no) | BIT(kbc_bit_no))) == 0x110) {

  } else if (interrupt_mask & BIT(mouse_bit_no)) {
    mouse_ih();
    if (mouse_get_info(&pp)) {

      if (!was_left_button_pressed && pp.lb) {
        mouse_device->left_button_is_pressed = true;
        mouse_device->left_button_released = false;
        was_left_button_pressed = true;
      } else if (was_left_button_pressed && !pp.lb) {
        mouse_device->left_button_released = true;
        mouse_device->left_button_is_pressed = false;
        was_left_button_pressed = false;
      }

      if (!was_right_button_pressed && pp.rb) {
        mouse_device->right_button_is_pressed = true;
        mouse_device->right_button_released = false;
        was_right_button_pressed = true;
      } else if (was_right_button_pressed && !pp.rb) {
        mouse_device->right_button_released = true;
        mouse_device->right_button_is_pressed = false;
        was_right_button_pressed = false;
      }

      if (!was_middle_button_pressed && pp.mb) {
        mouse_device->middle_button_is_pressed = true;
        mouse_device->middle_button_released = false;
        was_middle_button_pressed = true;
      } else if (was_middle_button_pressed && !pp.mb) {
        mouse_device->middle_button_released = true;
        mouse_device->middle_button_is_pressed = false;
        was_middle_button_pressed = false;
      }

      mouse->x += pp.delta_x;
      mouse->y -= pp.delta_y;

      if (mouse->x < 0) mouse->x = 0;
      if (mouse->x >= (int16_t) h_res) mouse->x = h_res - 1;
      if (mouse->y < 0) mouse->y = 0;
      if (mouse->y >= (int16_t) v_res) mouse->y = v_res - 1;
    }
  } else if (interrupt_mask & BIT(kbc_bit_no)) {
    kbc_ih();
    if (scancode[1] == 0x81) {
      keyboard_device->escape_key_pressed = true;
    }
  }
  
  if (interrupt_mask & BIT(timer_bit_no)) {
    timer_int_handler();

    if (game_main_loop()) {
      return 1;
    }

    if (counter % (frequency / FPS) == 0) {
      renderGameObjects();
      if (vg_replace_buffer()) return 1;
    }
  }

  return 0;
}

int update_timer_freq(uint32_t freq) {
  if (timer_unsubscribe_int()) return 1;
  if (timer_initiate_and_subscribe(&timer_bit_no, freq)) return 1;
  return 0;
}

ScreenInfo getScreenInfo() {
  ScreenInfo info;

  uint16_t* vg_info = getInfo();

  info.xres = *vg_info;
  info.yres = *(vg_info+1);

  free(vg_info);
  
  return info;
}
