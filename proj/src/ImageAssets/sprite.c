#include <lcom/lcf.h>

#include <stdint.h>

#include "sprite.h"

Sprite* create_sprite(xpm_map_t pic, uint16_t origin_offset_x,
                      uint16_t origin_offset_y, uint16_t z_index) {
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
  sp->origin_offset_x = origin_offset_x;
  sp->origin_offset_y = origin_offset_y;

  return sp;
}

void destroy_sprite(Sprite *sp) {
  if (sp == NULL) return;
  free(sp->map);
  free(sp);
}

void update_sprite_depth(Sprite *sp, uint16_t z_index) {
  if (sp == NULL) return;
  sp->z_index = z_index;
}
