#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

#include "../Devices/KeyBoard/keyboardkeys.h"
#include "../Devices/RTC/rtc_time.h"

#include "../GameObjects/gameobject.h"
#include "../ImageAssets/sprite.h"

typedef struct MouseDevice {
  GameObject* mouse;
  bool left_button_is_pressed;
  bool left_button_released;
  bool right_button_is_pressed;
  bool right_button_released;
  bool middle_button_is_pressed;
  bool middle_button_released;
} MouseDevice;

typedef struct KeyboardDevice {
  KeyPresses* keyPresses;
} KeyboardDevice;

typedef struct ScreenInfo {
  uint16_t xres, yres;
} ScreenInfo;

int boot_devices(uint32_t freq, uint16_t framespersecond, uint16_t mode);

int stop_devices();

int interrupt_handler(uint32_t interrupt_mask);

int update_timer_freq(uint32_t freq);

ScreenInfo getScreenInfo();
