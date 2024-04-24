#include <lcom/lcf.h>
#include <stdint.h>

#include "gameobject.h"

#include "../ImageAssets/MouseCursor.xpm"


GameObject_t* mouse;

static GameObject_t* create_gameobject(xpm_map_t pic, uint16_t x, uint16_t y, uint16_t origin_offset_x, uint16_t origin_offset_y) {

  GameObject_t* gameObject = (GameObject_t*) malloc(sizeof(GameObject_t));
  Sprite* sprite = create_sprite(pic, x, y, 0);

  gameObject->sprite = sprite;
  gameObject->x = x;
  gameObject->y = y;
  gameObject->origin_offset_x = origin_offset_x;
  gameObject->origin_offset_y = origin_offset_y;

  return gameObject;
}

static void destroy_gameobject(GameObject_t* gameObject) {
  destroy_sprite(gameObject->sprite);
  free(gameObject);
}

int draw_gameObject(GameObject_t* gameObject) {
  gameObject->sprite->x = gameObject->x + gameObject->origin_offset_x;
  gameObject->sprite->y = gameObject->y + gameObject->origin_offset_y;

  return draw_sprite(gameObject->sprite);
}

int create_gameobjects() {
  mouse = create_gameobject((xpm_map_t) MouseCursor, 0, 0, 0, 0);

  return 0;
}

int destroy_gameobjects() {
  destroy_gameobject(mouse);

  return 0;
}

