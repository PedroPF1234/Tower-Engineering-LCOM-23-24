#include <lcom/lcf.h>
#include <stdint.h>

#include "gameobject.h"

#include "../ImageAssets/MouseCursor.xpm"

typedef struct RenderArrayList {
  GameObject** gameObjects;
  uint32_t capacity;
  uint32_t length;
} RenderArrayList;

typedef struct AnimatedGameObjectArray {
  AnimatedGameObject** animatedGameObjects;
  uint32_t capacity;
  uint32_t length;
} AnimatedGameObjectArray;

static AnimatedGameObjectArray animatedSprites;
static RenderArrayList renderPipeline;

GameObject* mouse;

static RenderArrayList newRenderArray(uint32_t capacity) {
  RenderArrayList renderArray;
  renderArray.length = 0;

  if (capacity != 0) {
    renderArray.capacity = capacity;
    renderArray.gameObjects = (GameObject**) malloc(capacity * sizeof(GameObject*));
  } else {
    renderArray.capacity = 10;
    renderArray.gameObjects = (GameObject**) malloc(10 * sizeof(GameObject*));
  }

  return renderArray;
}

static void insertRenderPipeline(RenderArrayList *array, GameObject *newGameObject) {
  if (array->length == 0) {
    array->gameObjects[0] = newGameObject;
    array->length++;
  }

  if (array->length >= array->capacity) {
    uint32_t newCapacity = array->capacity * 2;
    GameObject** oldPointer = array->gameObjects;
    GameObject** newPointer = (GameObject**)malloc(newCapacity * sizeof(GameObject*));
    array->gameObjects = newPointer;
    for (uint32_t i = 0; i < array->length; i++) {
      newPointer[i] = oldPointer[i];
    }
    free(oldPointer);
  }

  for (uint32_t i = 0; i < array->length; i++) {
    if (array->gameObjects[i]->z_index >= newGameObject->z_index) {
      array->length++;
      for (uint32_t j = array->length - 1; j > i; j--) {

        array->gameObjects[j] = array->gameObjects[j - 1];

      }

      array->gameObjects[i] = newGameObject;
      break;
    }
    if (i == array->length - 1) {
      array->gameObjects[i] = newGameObject;
      array->length++;
      break;
    }
  }
}

static void removeRenderPipeline(RenderArrayList *array, GameObject *gameObject) {
  
  if (array->length == 0) {
    printf("Array is already empty!\n");
    return;
  }

  for (uint32_t i = 0; i < array->length; i++) {
    if (array->gameObjects[i] == gameObject) {
      for (uint32_t j = i; j < array->length - 1; j++) {
        array->gameObjects[j] = array->gameObjects[j + 1]; 
      }
      array->gameObjects[array->length - 1] = (GameObject*)0;
      array->length--;
      break;
    }
  }
}

static AnimatedGameObjectArray newAnimatedGameObjectArray(uint32_t capacity) {
  AnimatedGameObjectArray animatedGameObjectArray;
  animatedGameObjectArray.length = 0;

  if (capacity != 0) {
    animatedGameObjectArray.capacity = capacity;
    animatedGameObjectArray.animatedGameObjects = (AnimatedGameObject**) malloc(capacity * sizeof(AnimatedGameObject*));
  } else {
    animatedGameObjectArray.capacity = 10;
    animatedGameObjectArray.animatedGameObjects = (AnimatedGameObject**) malloc(10 * sizeof(AnimatedGameObject*));
  }

  return animatedGameObjectArray;
}

static void insertAnimatedObjectArray(AnimatedGameObjectArray *array, AnimatedGameObject *newAnimatedGameObject) {
  if (array->capacity != array->length) {
    array->animatedGameObjects[array->length] = newAnimatedGameObject;
  } else {
    uint32_t newCapacity = array->capacity * 2;
    AnimatedGameObject** oldPointer = array->animatedGameObjects;
    AnimatedGameObject** newPointer = (AnimatedGameObject**)malloc(newCapacity * sizeof(AnimatedGameObject*));
    array->animatedGameObjects = newPointer;
    for (uint32_t i = 0; i < array->length; i++) {
      newPointer[i] = oldPointer[i];
    }
    free(oldPointer);
  }

  array->length++;
}

static AnimatedGameObject* getAnimatedObjectArray(AnimatedGameObjectArray *array, uint32_t index) {
  if (index < array->length) {
    return array->animatedGameObjects[index];
  } else {
    return NULL;
  }
}

static void removeAnimatedObjectArray(AnimatedGameObjectArray *array, AnimatedGameObject *animatedGameObject) {
  if (array->length == 0) {
    printf("Array is already empty!\n");
    return;
  }

  for (uint32_t i = 0; i < array->length; i++) {
    if (array->animatedGameObjects[i] == animatedGameObject) {
      for (uint32_t j = i; j < array->length - 1; j++) {
        array->animatedGameObjects[j] = array->animatedGameObjects[j + 1];
      }
      array->animatedGameObjects[array->length - 1] = (AnimatedGameObject*)0;
      array->length--;
      break;
    }
  }
}

static void destroyAnimatedGameObjectArray(AnimatedGameObjectArray *array) {
  for (int32_t i = 0; i < (int32_t)array->length; i++) {
    AnimatedGameObject* animatedGameObject = getAnimatedObjectArray(array, i);
    removeAnimatedObjectArray(array, animatedGameObject);
    free(animatedGameObject);
  }
}

void init_render_pipeline() {
  renderPipeline = newRenderArray(200);
  animatedSprites = newAnimatedGameObjectArray(200);
}

AnimatedGameObject* create_animated_gameobject(AnimatedSprite* animatedSprite, int16_t x, int16_t y, uint16_t z_index) {
  AnimatedGameObject* animatedGameObject = (AnimatedGameObject*) malloc(sizeof(AnimatedGameObject));

  Sprite* sprite = getSpriteArray(&animatedSprite->sprites, 0);

  animatedGameObject->gameObject = create_gameobject_from_sprite(getSpriteArray(&animatedSprite->sprites, 0), x, y, -(sprite->width)/2, -(sprite->height), z_index);
  animatedGameObject->animatedSprite = animatedSprite;

  insertAnimatedObjectArray(&animatedSprites, animatedGameObject);

  return animatedGameObject;
}

void destroy_animated_gameobject(AnimatedGameObject* animatedGameObject) {
  destroy_gameobject(animatedGameObject->gameObject);
  removeAnimatedObjectArray(&animatedSprites, animatedGameObject);
  free(animatedGameObject);
}

void updateAnimatedGameObjects() {
  for (uint32_t i = 0; i < animatedSprites.length; i++) {
    AnimatedGameObject* animatedGameObject = getAnimatedObjectArray(&animatedSprites, i);
    AnimatedSprite* animatedSprite = animatedGameObject->animatedSprite;
    update_animated_sprite(animatedSprite);
    Sprite* sprite = getSpriteArray(&animatedSprite->sprites, animatedSprite->current_sprite);
    updateGameObjectSprite(animatedGameObject->gameObject, sprite);
  }
}

void switchAnimatedSpriteOfAnimatedGameObject(AnimatedGameObject* animatedGameObject, AnimatedSprite* animatedSprite) {
  animatedGameObject->animatedSprite = animatedSprite;
  animatedSprite->cooldown_counter = 0;
  animatedSprite->current_sprite = 0;
  updateGameObjectSprite(animatedGameObject->gameObject, getSpriteArray(&animatedSprite->sprites, animatedSprite->current_sprite));
}

GameObject* create_gameobject(xpm_map_t pic, int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index, bool square_shape, bool visible) {

  GameObject* gameObject = (GameObject*) malloc(sizeof(GameObject));
  Sprite* sprite = create_sprite(pic, x, y, square_shape, visible);

  gameObject->sprite = sprite;
  gameObject->x = x;
  gameObject->y = y;
  gameObject->origin_offset_x = origin_offset_x;
  gameObject->origin_offset_y = origin_offset_y;
  gameObject->z_index = z_index;

  insertRenderPipeline(&renderPipeline, gameObject);

  return gameObject;
}

GameObject* create_spriteless_gameobject(int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index) {
  GameObject* gameObject = (GameObject*) malloc(sizeof(GameObject));

  gameObject->sprite = NULL;
  gameObject->x = x;
  gameObject->y = y;
  gameObject->origin_offset_x = origin_offset_x;
  gameObject->origin_offset_y = origin_offset_y;
  gameObject->z_index = z_index;

  return gameObject;
}

void add_sprite_to_spriteless_gameobject(GameObject* gameObject, Sprite* sprite) {
  if (gameObject->sprite == NULL) {
    gameObject->sprite = sprite;
    insertRenderPipeline(&renderPipeline, gameObject);
  }
}

void remove_sprite_from_spriteless_gameobject(GameObject* gameObject) {
  if (gameObject->sprite != NULL) {
    removeRenderPipeline(&renderPipeline, gameObject);
    gameObject->sprite = NULL;
  }
}

GameObject* create_gameobject_from_sprite(Sprite* sprite, int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index) {
  GameObject* gameObject = (GameObject*) malloc(sizeof(GameObject));

  gameObject->sprite = sprite;
  gameObject->x = x;
  gameObject->y = y;
  gameObject->origin_offset_x = origin_offset_x;
  gameObject->origin_offset_y = origin_offset_y;
  gameObject->z_index = z_index;

  insertRenderPipeline(&renderPipeline, gameObject);

  return gameObject;
}

void updateGameObjectZIndex(GameObject* gameObject, uint16_t z_index) {
  gameObject->z_index = z_index;
  removeRenderPipeline(&renderPipeline, gameObject);
  insertRenderPipeline(&renderPipeline, gameObject);
}

void destroy_gameobject(GameObject* gameObject) {
  removeRenderPipeline(&renderPipeline, gameObject);
  if (gameObject->sprite != NULL) destroy_sprite(gameObject->sprite);
  free(gameObject);
}

static int draw_gameObject(GameObject* gameObject) {
  gameObject->sprite->x = gameObject->x + gameObject->origin_offset_x;
  gameObject->sprite->y = gameObject->y + gameObject->origin_offset_y;

  return draw_sprite(gameObject->sprite);
}

void updateGameObjectSprite(GameObject* gameObject, Sprite* sprite) {
  gameObject->sprite = sprite;
}

void renderGameObjects() {

  for (uint32_t i = 0; i < renderPipeline.length; i++) {
    draw_gameObject(renderPipeline.gameObjects[i]);
  }
}

int create_gameobjects() {
  mouse = create_gameobject((xpm_map_t) MouseCursor, 0, 0, 0, 0, BIT_MASK(16), false, true);

  return 0;
}

int destroy_gameobjects() {
  destroy_gameobject(mouse);
  destroyAnimatedGameObjectArray(&animatedSprites);
  return 0;
}
