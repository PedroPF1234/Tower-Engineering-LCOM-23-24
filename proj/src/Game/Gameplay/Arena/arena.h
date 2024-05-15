#pragma once

#include "../../../Devices/device_controller.h"

typedef struct Arena {
  Sprite* background;
  int16_t* coordinate_targets;
  uint8_t num_targets;
  int16_t spawn_x;
  int16_t spawn_y;
} Arena;

Arena* initializeArenas();
void destroyArenas(Arena* arena);
