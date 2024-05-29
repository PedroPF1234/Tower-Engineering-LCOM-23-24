#pragma once

#include "../../../Devices/device_controller.h"

typedef struct Arena {
  Sprite* background;
  uint8_t num_targets;
  int16_t* targert_coordinates;
  int16_t* tower_coordinates;
  int16_t spawn_x, spawn_y;
  int16_t store_x, store_y;
} Arena;

Arena* initializeArenas();
void destroyArenas(Arena* arena);
