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

typedef struct AnimatedGameObject {
  GameObject* gameObject;
  AnimatedSprite* animatedSprite;
} AnimatedGameObject;

typedef struct GameObjectArray {
  GameObject** gameObjects;
  uint32_t capacity;
  uint32_t length;
} GameObjectArray;

void init_render_pipeline();

int create_gameobjects();
int destroy_gameobjects();
void renderGameObjects();

AnimatedGameObject* create_animated_gameobject(AnimatedSprite* animatedSprite, int16_t x, int16_t y, uint16_t z_index);
void destroy_animated_gameobject(AnimatedGameObject* animatedGameObject);
void updateAnimatedGameObjects();
void switchAnimatedSpriteOfAnimatedGameObject(AnimatedGameObject* animatedGameObject, AnimatedSprite* animatedSprite);
void hideAnimatedGameObject(AnimatedGameObject* animatedGameObject);
void showAnimatedGameObject(AnimatedGameObject* animatedGameObject);

GameObject* create_gameobject(xpm_map_t pic, int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index, bool square_shape, bool visible);
GameObject* create_gameobject_from_sprite(Sprite* sprite, int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index);
GameObject* create_spriteless_gameobject(int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index);
void add_sprite_to_spriteless_gameobject(GameObject* gameObject, Sprite* sprite);
void remove_sprite_from_spriteless_gameobject(GameObject* gameObject);
void updateGameObjectZIndex(GameObject* gameObject, uint16_t z_index);
void destroy_gameobject(GameObject* gameObject);
void destroy_gameobject_safe_sprite(GameObject* gameObject);
void updateGameObjectSprite(GameObject* gameObject, Sprite* sprite);

GameObjectArray newGameObjectArray(uint32_t capacity);
void insertGameObjectArray(GameObjectArray *array, GameObject *newGameObject);
GameObject* getGameObjectArray(GameObjectArray *array, uint32_t index);
void removeGameObjectArray(GameObjectArray *array, GameObject *gameObject);
void destroyGameObjectArray(GameObjectArray *array);
void showGameObjects(GameObjectArray* gameObjects);
void hideGameObjects(GameObjectArray* gameObjects);
