#pragma once

#include <stdint.h>
#include "../ImageAssets/sprite.h"

#define BIT_MASK(n) ((1 << n) - 1)
#define Z_INDEX_PER_LAYER 3
#define LOW_PRIORITY_Z_INDEX 1
#define MEDIUM_PRIORITY_Z_INDEX 2
#define HIGH_PRIORITY_Z_INDEX 3
#define MAX_PRIORITY_Z_INDEX 4

typedef struct GameObject {
  Sprite* sprite;
  int16_t x, y;
  int16_t origin_offset_x, origin_offset_y;
  uint16_t z_index;
} GameObject;

int create_gameobjects();
int destroy_gameobjects();
void renderGameObjects();

GameObject* create_gameobject(xpm_map_t pic, uint16_t x, uint16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index, bool square_shape, bool visible);
GameObject* create_gameobject_from_sprite(Sprite* sprite, uint16_t x, uint16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index);
GameObject* create_spriteless_gameobject(uint16_t x, uint16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index);
void add_sprite_to_spriteless_gameobject(GameObject* gameObject, Sprite* sprite);
void updateGameObjectZIndex(GameObject* gameObject, uint16_t z_index);
void destroy_gameobject(GameObject* gameObject);
void destroy_gameobject_after_sprite_destroyed(GameObject* gameObject);
void updateGameObjectSprite(GameObject* gameObject, Sprite* sprite);
