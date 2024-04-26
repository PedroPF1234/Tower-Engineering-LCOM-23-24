#pragma once

#include <stdint.h>
#include "../ImageAssets/sprite.h"

#define BIT_MASK(n) ((1 << n) - 1)

typedef struct GameObject {
  Sprite* sprite;
  int16_t x, y;
  uint16_t origin_offset_x, origin_offset_y;
} GameObject;

typedef struct Node {
    GameObject *gameObject;
    struct Node *next;
} Node;

int create_gameobjects();
int destroy_gameobjects();
void renderGameObjects();

GameObject* create_gameobject(xpm_map_t pic, uint16_t x, uint16_t y, uint16_t origin_offset_x, uint16_t origin_offset_y, uint16_t z_index, bool square_shape);
void destroy_gameobject(GameObject* gameObject);
