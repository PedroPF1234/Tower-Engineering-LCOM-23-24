#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "../Game/game.h"

#include "Mouse/mouse.h"
#include "KeyBoard/kbc.h"
#include "Graphics/graphics.h"
#include "Timer/timer.h"
#include "device_controller.h"

// Temp
#include "../Game/Player/player.h"

// Mouse Game Object
extern GameObject* mouse;

// Global device interfaces
MouseDevice* mouse_device;
KeyboardDevice* keyboard_device;
bool last_pressed_was_mouse = true;
 
// Device Interrupt Bit Masks
static uint8_t timer_bit_no;
static uint8_t kbc_bit_no;
static uint8_t mouse_bit_no;

static struct packet pp;
extern uint64_t counter;
extern uint8_t scancode[2];

static int default_video_mode = 0x11B;

static bool was_left_button_pressed = false;
static bool was_right_button_pressed = false;
static bool was_middle_button_pressed = false;

uint16_t FPS = 0;
ScreenInfo screen;
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

  screen = getScreenInfo();

  mouse->x = screen.xres / 2;
  mouse->y = screen.yres / 2;

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

static int kbc_interrupt() {
  uint8_t kbc_status;

  for (int j = 0; j < 5; j++) {

    if(kbc_read_status(&kbc_status)) {
      return 1;
    }

    if (!(kbc_status & BIT(0))) {
      continue;
    }
    
    if (kbc_status & (BIT(6) | BIT(7))) {
      return 1;
    } else {
      if (kbc_status & BIT(5)) {
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
          if (mouse->x >= (int16_t) screen.xres) mouse->x = screen.xres - 1;
          if (mouse->y < 0) mouse->y = 0;
          if (mouse->y >= (int16_t) screen.xres) mouse->y = screen.yres - 1;
        }
        last_pressed_was_mouse = true;
        return 0;
      } else {
        kbc_ih();
        if (scancode[1] == 0xE0) {
          scancode[0] = 0xE0;
          scancode[1] = 0;
        } else {
          addKeyPressAtEnd(&keyboard_device->keyPresses, scancode[1], scancode[0] ? true : false);
          scancode[0] = 0;
          scancode[1] = 0;
        }
        last_pressed_was_mouse = false;
        return 0;
      }
    }
  }

  return 1;
}

int interrupt_handler(uint32_t interrupt_mask) {

  if (interrupt_mask & (BIT(mouse_bit_no) | BIT(kbc_bit_no))) {

    int i = 1;

    if (interrupt_mask & 0x110) {
      i = 2;
    }

    for (; i > 0; i--) {
      if (kbc_interrupt()) return 1;
    }
  }
  
  if (interrupt_mask & BIT(timer_bit_no)) {
    timer_int_handler();

    if (game_main_loop()) {
      return 1;
    }

    if (counter % (frequency / FPS) == 0) {
      renderGameObjects();
      testRotate();
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
