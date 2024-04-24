#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

#include "MouseCursor.xpm"

typedef struct Sprite {
  uint16_t x, y;
  uint16_t width, height;
  uint8_t *map;
  bool is_visible;
  uint16_t z_index;
} Sprite;

Sprite *create_sprite(xpm_map_t pic, uint16_t x, uint16_t y, uint16_t z_index);
void destroy_sprite(Sprite *sp);
void update_sprite_depth(Sprite *sp, uint16_t z_index);
int draw_sprite(Sprite *sp);
