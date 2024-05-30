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

typedef struct SpriteArray {
  Sprite** sprites;
  uint32_t capacity;
  uint32_t length;
} SpriteArray;

typedef struct AnimatedSprite {
  SpriteArray sprites;
  uint8_t current_sprite;
  uint16_t animation_cooldown;
  uint16_t cooldown_counter;
} AnimatedSprite;

typedef struct AnimatedSpriteArray {
  AnimatedSprite** sprites;
  uint32_t capacity;
  uint32_t length;
} AnimatedSpriteArray;

// AnimatedSpriteArray functions
AnimatedSpriteArray newAnimatedSpriteArray(uint32_t capacity);
void pushAnimatedSpriteArray(AnimatedSpriteArray* array, AnimatedSprite* sprite);
AnimatedSprite* getAnimatedSpriteArray(AnimatedSpriteArray* array, uint32_t index);
void removeAnimatedSpriteArray(AnimatedSpriteArray* array, uint32_t index);
void destroyAnimatedSpriteArray(AnimatedSpriteArray* array);
AnimatedSpriteArray newAnimatedSpriteArrayFromSprites(char*** sprites, uint8_t sprites_per_animated, uint8_t num_of_sprites, uint16_t cooldown);

// AnimatedSprite functions
AnimatedSprite* create_animated_sprite(char*** sprites, uint8_t num_of_sprites, uint16_t cooldown);
void destroy_animated_sprite(AnimatedSprite* sprite);
void update_animated_sprite(AnimatedSprite* sprite);

// SpriteArray functions
SpriteArray newSpriteArray(uint32_t capacity);
void pushSpriteArray(SpriteArray* array, Sprite* sprite);
Sprite* getSpriteArray(SpriteArray* array, uint32_t index);
void removeSpriteArray(SpriteArray* array, uint32_t index);
void destroySpriteArray(SpriteArray* array);
void hideSprites(SpriteArray* array);
void showSprites(SpriteArray* array);

// Individual sprite functions
Sprite *create_sprite(xpm_map_t pic, int16_t x, int16_t y, bool square_shape, bool is_visible);
Sprite* create_rotation_abled_sprite(xpm_map_t pic, int16_t x, int16_t y, bool square_shape, 
  bool is_visible, int* num_sprites);
Sprite* create_sprite_from_sprite(Sprite* sprite, int16_t x, int16_t y, bool is_visible, int rotations);
void destroy_sprite(Sprite *sp);
void update_sprite_visibility(Sprite *sp, bool is_visible);
int draw_sprite(Sprite *sp);
