#pragma once

#include <stdint.h>
#include "../ImageAssets/sprite.h"

typedef struct GameObject {
  Sprite* sprite;
  uint16_t x, y;
  uint16_t origin_offset_x, origin_offset_y;
} GameObject_t;

int draw_gameObject(GameObject_t* gameObject);
int create_gameobjects();
int destroy_gameobjects();
