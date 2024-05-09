#include <lcom/lcf.h>

#include <stdint.h>

#include "sprite.h"

#include "../Devices/Graphics/graphics.h"

static uint32_t createdSprites = 0;

Sprite* create_sprite(xpm_map_t pic, uint16_t x, uint16_t y, uint16_t z_index, bool square_shape, bool is_visible) {
  
  Sprite* sp = (Sprite*) malloc(sizeof(Sprite));
  if (sp == NULL) return NULL;

  xpm_image_t img;
  sp->map = xpm_load(pic, XPM_8_8_8, &img);
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->z_index = z_index;
  sp->width = img.width;
  sp->height = img.height;
  sp->x = x;
  sp->y = y;
  sp->is_visible = is_visible;
  sp->square_shape = square_shape;

  createdSprites++;

  return sp;
}

void destroy_sprite(Sprite *sp) {
  if (sp == NULL) return;
  free(sp->map);
  free(sp);
  createdSprites--;
}

void update_sprite_depth(Sprite *sp, uint16_t z_index) {
  if (sp == NULL) return;
  sp->z_index = z_index;
}

void update_sprite_visibility(Sprite *sp, bool is_visible) {
  if (sp == NULL) return;
  sp->is_visible = is_visible;
}

int draw_sprite(Sprite *sp) {
  if (sp == NULL) return 1;
  if (sp->is_visible) {
    if (vg_draw_xpm(sp->x, sp->y, sp->width, sp->height, sp->map, sp->square_shape)) return 1;
  }
  return 0;
}
