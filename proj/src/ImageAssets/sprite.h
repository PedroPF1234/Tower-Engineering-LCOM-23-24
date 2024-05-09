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
  uint16_t z_index;
} Sprite;

Sprite *create_sprite(xpm_map_t pic, uint16_t x, uint16_t y,
  uint16_t z_index, bool square_shape, bool is_visible);
void destroy_sprite(Sprite *sp);
void update_sprite_depth(Sprite *sp, uint16_t z_index);
void update_sprite_visibility(Sprite *sp, bool is_visible);
int draw_sprite(Sprite *sp);
