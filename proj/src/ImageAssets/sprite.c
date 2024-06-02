#include <lcom/lcf.h>

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "sprite.h"

#include "../Devices/Graphics/graphics.h"

#define DEFAULT_ANGLE_INCREMENT 1.0

static uint32_t createdSprites = 0;

// AnimatedSpriteArray functions
AnimatedSpriteArray newAnimatedSpriteArray(uint32_t capacity) {
  AnimatedSpriteArray array;
  array.length = 0;

  if (capacity) {
    array.sprites = (AnimatedSprite**)malloc(capacity * sizeof(AnimatedSprite*));
    array.capacity = capacity;
  } else {
    array.sprites = (AnimatedSprite**)malloc(sizeof(AnimatedSprite*) * 10);
    array.capacity = 10;
  }
  return array;
}

void pushAnimatedSpriteArray(AnimatedSpriteArray* array, AnimatedSprite* sprite) {

  if (array->capacity != array->length) {
    array->sprites[array->length] = sprite;
  } else {
    uint32_t newCapacity = array->capacity * 2;
    AnimatedSprite** oldPointer = array->sprites;
    AnimatedSprite** newPointer = (AnimatedSprite**)malloc(newCapacity * sizeof(AnimatedSprite*));
    array->sprites = newPointer;
    for (uint32_t i = 0; i < array->length; i++) {
      newPointer[i] = oldPointer[i];
    }
    free(oldPointer);
    array->sprites[array->length] = sprite;
  }
  array->length++;
}

AnimatedSprite* getAnimatedSpriteArray(AnimatedSpriteArray* array, uint32_t index) {
  if (index < array->length) {
    return array->sprites[index];
  } else {
    return NULL;
  }
}

void removeAnimatedSpriteArray(AnimatedSpriteArray* array, uint32_t index) {
  if (index < array->length) {
    AnimatedSprite* sprite = getAnimatedSpriteArray(array, index);
    destroy_animated_sprite(sprite);
    for (uint32_t i = index; i < array->length - 1; i++) {
      array->sprites[i] = array->sprites[i + 1];
    }
    array->length--;
  }
}

void destroyAnimatedSpriteArray(AnimatedSpriteArray* array) {
  for (uint32_t i = 0; i < array->length; i++) {
    destroy_animated_sprite(array->sprites[i]);
  }
  free(array->sprites);
}

AnimatedSpriteArray newAnimatedSpriteArrayFromSprites(char*** sprites, uint8_t sprites_per_animated, uint8_t num_of_sprites, uint16_t cooldown) {
  AnimatedSpriteArray array = newAnimatedSpriteArray(0);

  for (uint32_t i = 0; i < num_of_sprites*sprites_per_animated; i += sprites_per_animated) {
    AnimatedSprite* as = create_animated_sprite(sprites + i, sprites_per_animated, cooldown);
    if (as == NULL) {
      destroyAnimatedSpriteArray(&array);
      return array;
    }
    pushAnimatedSpriteArray(&array, as);
  }
  
  return array;
}

// AnimatedSprite functions
AnimatedSprite* create_animated_sprite(char*** sprite, uint8_t num_of_sprites, uint16_t cooldown) {
  AnimatedSprite* as = (AnimatedSprite*)malloc(sizeof(AnimatedSprite));
  if (as == NULL) return NULL;

  as->sprites = newSpriteArray(0);
  as->current_sprite = 0;
  as->animation_cooldown = cooldown;
  as->cooldown_counter = 0;

  for (uint32_t i = 0; i < num_of_sprites; i++) {
    Sprite* sp = create_sprite((xpm_map_t)sprite[i], 0, 0, false, true);
    if (sp == NULL) {
      destroy_animated_sprite(as);
      return NULL;
    }
    pushSpriteArray(&as->sprites, sp);
  }

  if (as->sprites.length != num_of_sprites) {
    destroy_animated_sprite(as);
    return NULL;
  }

  return as;
}

void destroy_animated_sprite(AnimatedSprite* as) {
  for (uint32_t i = 0; i < as->sprites.length; i++) {
    destroy_sprite(getSpriteArray(&as->sprites, i));
  }
  destroySpriteArray(&as->sprites);
  free(as);
}

void update_animated_sprite(AnimatedSprite* as) {
  if (as->animation_cooldown == 0) return;

  if (as->cooldown_counter < as->animation_cooldown) {
    as->cooldown_counter++;
  } else {
    as->cooldown_counter = 0;
    as->current_sprite++;
    if (as->current_sprite >= as->sprites.length) {
      as->current_sprite = 0;
    }
  }
}

// Sprite array functions
SpriteArray newSpriteArray(uint32_t capacity) {
  SpriteArray array;
  array.length = 0;

  if (capacity) {
    array.sprites = (Sprite**)malloc(capacity * sizeof(Sprite*));
    array.capacity = capacity;
  } else {
    array.sprites = (Sprite**)malloc(sizeof(Sprite*) * 10);
    array.capacity = 10;
  }
  return array;
}

SpriteArray newApriteFromCharArray(char*** sprites, uint32_t sprite_count) {
  SpriteArray array;
  array.length = 0;
  array.capacity = sprite_count;

  array.sprites = (Sprite**)malloc(sprite_count * sizeof(Sprite*));

  for (uint32_t i = 0; i < sprite_count; i++) {
    Sprite* temp = create_sprite((xpm_map_t)sprites[i], 0, 0, false, true);
    pushSpriteArray(&array, temp);
  }

  return array;
}

void pushSpriteArray(SpriteArray* array, Sprite* sprite) {

  if (array->capacity != array->length) {
      array->sprites[array->length] = sprite;
  } else {
    array->capacity = array->capacity * 2;
    Sprite** oldPointer = array->sprites;
    Sprite** newPointer = (Sprite**)malloc(array->capacity * sizeof(Sprite*));
    array->sprites = newPointer;
    for (uint32_t i = 0; i < array->length; i++) {
      newPointer[i] = oldPointer[i];
    }
    free(oldPointer);
    array->sprites[array->length] = sprite;
  }
  array->length++;
}

Sprite* getSpriteArray(SpriteArray* array, uint32_t index) {
  if (index < array->length) {
    return array->sprites[index];
  } else {
    return NULL;
  }
}

void removeSpriteArray(SpriteArray* array, uint32_t index) {
  if (index < array->length) {
    Sprite* sprite = getSpriteArray(array, index);
    destroy_sprite(sprite);
    for (uint32_t i = index; i < array->length - 1; i++) {
      array->sprites[i] = array->sprites[i + 1];
    }
    array->length--;
  }
}

void destroySpriteArray(SpriteArray* array) {
  
  
  for (uint32_t i = 0; i < array->length; i++) {
    destroy_sprite(array->sprites[i]);
  }
  free(array->sprites);
}

void cleanSpriteArray(SpriteArray* array) {
  array->length = 0;
}

void hideSprites(SpriteArray* array) {
  for (uint32_t i = 0; i < array->length; i++) {
    array->sprites[i]->is_visible = false;
  }
}

void showSprites(SpriteArray* array) {
  for (uint32_t i = 0; i < array->length; i++) {
    array->sprites[i]->is_visible = true;
  }
}

// Individual sprite functions
Sprite* create_sprite(xpm_map_t pic, int16_t x, int16_t y, bool square_shape, bool is_visible) {
  
  Sprite* sp = (Sprite*) malloc(sizeof(Sprite));
  if (sp == NULL) return NULL;

  xpm_image_t img;
  sp->map = xpm_load(pic, XPM_8_8_8, &img);
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->width = img.width;
  sp->height = img.height;
  sp->x = x;
  sp->y = y;
  sp->is_visible = is_visible;
  sp->square_shape = square_shape;

  createdSprites++;

  return sp;
}

static uint8_t* rotate_image(uint8_t* original_pixels, uint16_t width, uint16_t height, double angle, uint16_t* new_width, uint16_t* new_height) {

  double rad = angle * M_PI / 180.0;
  double cos_theta = cos(rad);
  double sin_theta = sin(rad);

  *new_width = (uint16_t)(width * fabs(cos_theta) + height * fabs(sin_theta));
  *new_height = (uint16_t)(width * fabs(sin_theta) + height * fabs(cos_theta));

  uint8_t *rotated_pixels = (uint8_t *)malloc(*new_width * *new_height * 3);
  uint16_t x_center = width / 2;
  uint16_t y_center = height / 2;
  uint16_t new_x_center = *new_width / 2;
  uint16_t new_y_center = *new_height / 2;

  for (int y = 0; y < *new_height; y++) {
    for (int x = 0; x < *new_width; x++) {
      int new_x = x - new_x_center;
      int new_y = y - new_y_center;

      int src_x = (int)(cos_theta * new_x + sin_theta * new_y) + x_center;
      int src_y = (int)(-sin_theta * new_x + cos_theta * new_y) + y_center;

      if (src_x >= 0 && src_x < width && src_y >= 0 && src_y < height) {
        for (int c = 0; c < 3; c++) {
          rotated_pixels[(y * *new_width + x) * 3 + c] = original_pixels[(src_y * width + src_x) * 3 + c];
        }
      } else {
        for (int c = 0; c < 3; c++) {
          rotated_pixels[(y * *new_width + x) * 3 + c] = 0x00;
        }
      }
    }
  }

  return rotated_pixels;
}

Sprite* create_rotation_abled_sprite(xpm_map_t pic, int16_t x, int16_t y, bool square_shape, bool is_visible, int* num_sprites) {

  xpm_image_t img;
  uint8_t* bytes = xpm_load(pic, XPM_8_8_8, &img);
  if (bytes == NULL) {
    return NULL;
  }

  *num_sprites = 360 / DEFAULT_ANGLE_INCREMENT;
  Sprite* sprites = (Sprite*)malloc(*num_sprites * sizeof(Sprite));

  for (int i = 0; i < *num_sprites; i++) {
    double angle = i * DEFAULT_ANGLE_INCREMENT;
    uint16_t new_width, new_height;
    uint8_t *rotated_pixels = rotate_image(bytes, img.width, img.height, angle,
                                           &new_width, &new_height);
    if (!rotated_pixels) {
        for (int j = 0; j < i; j++) {
            free(sprites[j].map);
        }
        free(sprites);
        return NULL;
    }

    sprites[i].x = x;
    sprites[i].y = y;
    sprites[i].width = new_width;
    sprites[i].height = new_height;
    sprites[i].map = rotated_pixels;
    sprites[i].is_visible = is_visible;
    sprites[i].square_shape = square_shape;
  }

  return sprites;
}

Sprite* create_sprite_from_sprite(Sprite* sprite, int16_t x, int16_t y, bool is_visible, int rotations) {
  if (sprite == NULL) return NULL;

  Sprite* sp = (Sprite*)malloc(sizeof(Sprite) * rotations);
  if (sp == NULL) return NULL;

  for (int i = 0; i < rotations; i++) {
    sp[i].map = (uint8_t*)malloc(sprite[i].width * sprite[i].height * 3);
    if (sp[i].map == NULL) {
      for (int j = 0; j < i; j++) {
        free(sp[j].map);
      }
      free(sp);
      return NULL;
    }

    memcpy(sp[i].map, sprite[i].map, sprite[i].width * sprite[i].height * 3);
    sp[i].width = sprite[i].width;
    sp[i].height = sprite[i].height;
    sp[i].x = x;
    sp[i].y = y;
    sp[i].is_visible = is_visible;
    sp[i].square_shape = sprite[i].square_shape;
  }

  createdSprites++;

  return sp;
}

void destroy_sprite(Sprite *sp) {

  // Memory leak if sprite is a series of rotated sprites, needs fixing
  if (sp == NULL) return;
  free(sp->map);
  free(sp);
  createdSprites--;
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

Sprite* getSpriteFromAngle(Sprite* sprite, int angle) {
  
  if (angle < 0) {
    angle = 360 - abs(angle);
  }

  return sprite + angle;
}

int calculate_angle(float x, float y, float x2, float y2) {
  if (x == x2 && y == y2) return 0;
  float angle = atan2(y2 - y, x2 - x) * 180 / M_PI;
  return (int)angle;
}

float* normalize_vector(float x, float y, float x2, float y2) {
  float* vector = (float*)malloc(2 * sizeof(float));
  if (vector == NULL) return NULL;

  float magnitude = sqrt(pow(x2 - x, 2) + pow(y2 - y, 2));
  vector[0] = (x2 - x) / magnitude;
  vector[1] = (y2 - y) / magnitude;

  return vector;
}
