#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

#include "../ImageAssets/sprite.h"

typedef struct Mouse {
  int16_t x_position;
  int16_t y_position;
  bool left_button;
  bool right_button;
  bool middle_button;
  Sprite* sprite;
} Mouse;


int boot_devices(uint32_t freq, uint16_t mode);

int stop_devices();

int interrupt_handler(uint32_t interrupt_mask);
