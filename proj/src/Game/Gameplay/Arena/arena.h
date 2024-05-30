#pragma once

#include "../../../Devices/device_controller.h"

typedef struct Arena {
  Sprite* background;
  uint8_t num_targets;
  uint8_t num_towers;
  uint8_t num_decorations;
  int16_t* targert_coordinates;
  int16_t* tower_coordinates;
  int16_t* decoration_coordinates_and_ids;
  int16_t spawn_x, spawn_y;
  int16_t shop_x, shop_y;
  int16_t base_x, base_y;
} Arena;

Arena* initializeArenas();
void destroyArenas(Arena* arena);
