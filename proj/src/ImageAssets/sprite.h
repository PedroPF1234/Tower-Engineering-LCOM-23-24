#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

#include "MouseCursor.xpm"

typedef struct Sprite {
  int16_t x, y;
  uint16_t width, height;
  uint8_t *map;
  bool is_visible;
  bool square_shape;
} Sprite;

Sprite *create_sprite(xpm_map_t pic, int16_t x, int16_t y, bool square_shape, bool is_visible);
Sprite* create_rotation_abled_sprite(xpm_map_t pic, int16_t x, int16_t y, bool square_shape, 
  bool is_visible, int* num_sprites);
Sprite* create_sprite_from_sprite(Sprite* sprite, int16_t x, int16_t y, bool is_visible, int rotations);
void destroy_sprite(Sprite *sp);
void update_sprite_visibility(Sprite *sp, bool is_visible);
int draw_sprite(Sprite *sp);
