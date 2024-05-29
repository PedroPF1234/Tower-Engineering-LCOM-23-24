#include <lcom/lcf.h>
#include <stdint.h>

#include "gameobject.h"

#include "../ImageAssets/MouseCursor.xpm"

typedef struct RenderArrayList {
    GameObject** gameObjects;
    uint32_t capacity;
    uint32_t length;
} RenderArrayList;

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

void init_render_pipeline() {
  renderPipeline = newRenderArray(200);
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

  return 0;
}


