#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

#include "MouseCursor.xpm"

/**
 * @brief Struct that represents a sprite.
 * 
 * Struct that contains the x and y position, the width and height, the map of pixels of the sprite, the visibility and the shape of the sprite.
 * 
 * @param x The x position of the sprite.
 * @param y The y position of the sprite.
 * @param width The width of the sprite.
 * @param height The height of the sprite.
 * @param map The map of pixels of the sprite.
 * @param is_visible A boolean that represents if the sprite is visible.
 * @param square_shape A boolean that represents if the sprite is squared.
*/
typedef struct Sprite {
  int16_t x, y;
  uint16_t width, height;
  uint8_t *map;
  bool is_visible;
  bool square_shape;
} Sprite;

/**
 * @brief Struct that represents an array of sprites.
 * 
 * Struct that contains an array of sprites, the capacity and the length of the array.
 * 
 * @param sprites The array of sprites.
 * @param capacity The capacity of the array.
 * @param length The length of the array.
*/
typedef struct SpriteArray {
  Sprite** sprites;
  uint32_t capacity;
  uint32_t length;
} SpriteArray;

/**
 * @brief Struct that represents an animated sprite.
 * 
 * Struct that contains an array of sprites, the current sprite, the animation cooldown and the cooldown counter.
 * 
 * @param sprites The array of sprites.
 * @param current_sprite The index of the current sprite.
 * @param animation_cooldown The ammount of time between each sprite change (in milliseconds, ms).
 * @param cooldown_counter The counter of the cooldown.
*/
typedef struct AnimatedSprite {
  SpriteArray sprites;
  uint8_t current_sprite;
  uint16_t animation_cooldown;
  uint16_t cooldown_counter;
} AnimatedSprite;

/**
 * @brief Struct that represents an array of animated sprites.
 * 
 * Struct that contains an array of animated sprites, the capacity and the length of the array.
 * 
 * @param sprites The array of animated sprites.
 * @param capacity The capacity of the array.
 * @param length The length of the array.
*/
typedef struct AnimatedSpriteArray {
  AnimatedSprite** sprites;
  uint32_t capacity;
  uint32_t length;
} AnimatedSpriteArray;


// AnimatedSpriteArray functions

/**
 * @brief Initializes a new array of animated sprites.
 * 
 * Initializes a new array of animated sprites with the given capacity.
 * 
 * @param capacity The capacity of the array.
 * @return A new AnimatedSpriteArray object.
*/
AnimatedSpriteArray newAnimatedSpriteArray(uint32_t capacity);

/**
 * @brief Pushes a animated sprite to a given animated sprite array.
 * 
 * @param array The array of animated sprites.
 * @param sprite The animated sprite to be pushed to the array.
*/
void pushAnimatedSpriteArray(AnimatedSpriteArray* array, AnimatedSprite* sprite);

/**
 * @brief Gets a animated sprite from a given animated sprite array.
 * 
 * @param array The array of animated sprites.
 * @param index The index of the animated sprite to be retrieved.
*/
AnimatedSprite* getAnimatedSpriteArray(AnimatedSpriteArray* array, uint32_t index);

/**
 * @brief Removes a animated sprite from a given animated sprite array.
 * 
 * @param array The array of animated sprites.
 * @param index The index of the animated sprite to be removed.
*/
void removeAnimatedSpriteArray(AnimatedSpriteArray* array, uint32_t index);

/**
 * @brief Destroys a animated sprite array and frees associated memory.
 * 
 * @param array The array of animated sprites to be destroyed.
*/
void destroyAnimatedSpriteArray(AnimatedSpriteArray* array);

/**
 * @brief Initializes a new array of animated sprites from a given array of animated sprites.
 * 
 * Initializes a new array of animated sprites from a given array of animated sprites.
 * 
 * @param sprites The array of animated sprites.
 * @param sprites_per_animated The number of sprites per animated sprite.
 * @param num_of_sprites The number of animated sprites.
 * @param cooldown The ammount of time between each sprite change (in milliseconds, ms).
 * @return A new AnimatedSpriteArray object.
*/
AnimatedSpriteArray newAnimatedSpriteArrayFromSprites(char*** sprites, uint8_t sprites_per_animated, uint8_t num_of_sprites, uint16_t cooldown);


// AnimatedSprite functions

/**
 * @brief Initializes a new animated sprite.
 * 
 * Initializes a new animated sprite with the given array of sprites and the animation cooldown.
 * 
 * @param sprites The array of sprites.
 * @param num_of_sprites The number of sprites.
 * @param cooldown The ammount of time between each sprite change (in milliseconds, ms).
 * @return A new AnimatedSprite object.
*/
AnimatedSprite* create_animated_sprite(char*** sprites, uint8_t num_of_sprites, uint16_t cooldown);

/**
 * @brief Destroys a animated sprite and frees associated memory.
 * 
 * @param sprite The animated sprite to be destroyed.
*/
void destroy_animated_sprite(AnimatedSprite* sprite);

/**
 * @brief Updates a animated sprite.
 * 
 * Updates a animated sprite by changing the current sprite based on the cooldown.
 * 
 * @param sprite The animated sprite to be updated.
*/
void update_animated_sprite(AnimatedSprite* sprite);


// SpriteArray functions

/**
 * @brief Initializes a new array of sprites.
 * 
 * Initializes a new array of sprites with the given capacity.
 * 
 * @param capacity The capacity of the array.
 * @return A new SpriteArray object.
*/
SpriteArray newSpriteArray(uint32_t capacity);

/**
 * @brief Initializes a new array of sprites from a given array of sprites.
 * 
 * @param sprites The array of sprites.
 * @param sprite_count The number of sprites.
 * @return A new SpriteArray object.
*/
SpriteArray newApriteFromCharArray(char*** sprites, uint32_t sprite_count);

/**
 * @brief Pushes a sprite to a given sprite array.
 * 
 * @param array The array of sprites.
 * @param sprite The sprite to be pushed to the array.
*/
void pushSpriteArray(SpriteArray* array, Sprite* sprite);

/**
 * @brief Gets a sprite from a given sprite array.
 * 
 * @param array The array of sprites.
 * @param index The index of the sprite to be retrieved.
 * @return The sprite at the given index.
*/
Sprite* getSpriteArray(SpriteArray* array, uint32_t index);

/**
 * @brief Removes a sprite from a given sprite array.
 * 
 * @param array The array of sprites.
 * @param index The index of the sprite to be removed.
*/
void removeSpriteArray(SpriteArray* array, uint32_t index);

/**
 * @brief Destroys a sprite array and frees associated memory.
 * 
 * @param array The array of sprites to be destroyed.
*/
void destroySpriteArray(SpriteArray* array);

/**
 * @brief Hides all sprites in a given sprite array.
 * 
 * @param array The array of sprites.
*/
void hideSprites(SpriteArray* array);

/**
 * @brief Shows all sprites in a given sprite array.
 * 
 * @param array The array of sprites.
*/
void showSprites(SpriteArray* array);

/**
 * @brief Cleans a sprite array.
 * 
 * Cleans a sprite array by setting its length to 0.
 * 
 * @param array The array of sprites to be cleaned.
*/
void cleanSpriteArray(SpriteArray* array);


// Individual sprite functions

/**
 * @brief Initializes a new sprite.
 * 
 * Initializes a new sprite with the given x and y position, the map of pixels, the shape and the visibility.
 * 
 * @param pic The map of pixels of the sprite.
 * @param x The x position of the sprite.
 * @param y The y position of the sprite.
 * @param square_shape A boolean that represents if the sprite is squared.
 * @param is_visible A boolean that represents if the sprite is visible.
 * @return A new Sprite object.
*/
Sprite *create_sprite(xpm_map_t pic, int16_t x, int16_t y, bool square_shape, bool is_visible);

/**
 * @brief Initializes a new sprite with the ability to be rotated.
 * 
 * Initializes a new rotating sprite with the given x and y position, the map of pixels, the shape and the visibility.
 * 
 * @param pic The map of pixels of the sprite.
 * @param x The x position of the sprite.
 * @param y The y position of the sprite.
 * @param square_shape A boolean that represents if the sprite is squared.
 * @param is_visible A boolean that represents if the sprite is visible.
 * @param num_sprites The number of different rotating sprites.
 * @return A new Sprite object.
*/
Sprite* create_rotation_abled_sprite(xpm_map_t pic, int16_t x, int16_t y, bool square_shape, 
  bool is_visible, int* num_sprites);

/**
 * @brief Initializes a new rotated sprite from a given sprite.
 * 
 * Initializes a new rotated sprite from a given sprite with the given x and y position, the visibility and the number of rotations.
 * 
 * @param sprite The sprite to be rotated.
 * @param x The x position of the sprite.
 * @param y The y position of the sprite.
 * @param is_visible A boolean that represents if the sprite is visible.
 * @param rotations The index corresponding to the intended rotation angle of the sprite.
 * @return A new Sprite object rotated to the intended rotation/angle.
 */
Sprite* create_sprite_from_sprite(Sprite* sprite, int16_t x, int16_t y, bool is_visible, int rotations);

/**
 * @brief Destroys a sprite and frees associated memory.
 * 
 * @param sp The sprite to be destroyed.
*/
void destroy_sprite(Sprite *sp);

/**
 * @brief Updates the visibility of a sprite.
 * 
 * Updates the visibility of a sprite based on the given boolean.
 * 
 * @param sp The sprite to be updated.
 * @param is_visible A boolean that represents if the sprite is visible.
*/
void update_sprite_visibility(Sprite *sp, bool is_visible);

/**
 * @brief Draws a sprite.
 * 
 * Draws a given sprite on the screen.
 * 
 * @param sp The sprite to be drawn.
*/
int draw_sprite(Sprite *sp);

/**
 * @brief Gets a sprite from a given sprite with the intended angle.
 * 
 * @param sprite The sprite to be rotated.
 * @param angle The intended angle of the sprite.
 * @return A Sprite object rotated to the intended angle.
*/
Sprite* getSpriteFromAngle(Sprite* sprite, int angle);

/**
 * @brief Calculates the angle between a line connecting two points and the x-axis.
 * 
 * @param x The x position of the first point.
 * @param y The y position of the first point.
 * @param x2 The x position of the second point.
 * @param y2 The y position of the second point.
*/
int calculate_angle(float x, float y, float x2, float y2);

/**
 * @brief Normalizes a vector defined by two points.
 * 
 * @param x The x position of the first point.
 * @param y The y position of the first point.
 * @param x2 The x position of the second point.
 * @param y2 The y position of the second point.
*/
float* normalize_vector(float x, float y, float x2, float y2);
