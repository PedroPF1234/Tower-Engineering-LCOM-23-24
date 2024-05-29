#include <lcom/lcf.h>
#include <stdint.h>

#include "player.h"

#include "../../../ImageAssets/Bicho.xpm"

extern ScreenInfo screen;

Player* initializePlayer(float x, float y, int16_t ox, int16_t oy, int16_t hp) {
  Player* new_player = (Player*)malloc(sizeof(Player));
  new_player->sprites = newSpriteArray(9);

  char** playerSprites[] = {BichoUp, BichoDown, BichoLeft, BichoRight, BichoUpperLeft, BichoUpperRight, BichoLowerLeft, BichoLowerRight, BichoStationary};

  for (uint32_t i = 0; i < 9; i++) {
    Sprite* sprite;
    if (i == 8) {
      sprite = create_sprite((xpm_map_t)playerSprites[i], x, y, false, false);
    } else {
      sprite = create_sprite((xpm_map_t)playerSprites[i], x, y, false, true);
    }
    if (sprite == NULL) {
      destroyPlayer(new_player);
      return NULL;
    }
    pushSpriteArray(&new_player->sprites, sprite);
  }

  new_player->player = create_gameobject_from_sprite(getSpriteArray(&new_player->sprites, STATIONARY), x, y, ox, oy, y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);

  new_player->x = x;
  new_player->y = y;
  new_player->speed[0] = 0.0f;
  new_player->speed[1] = 0.0f;
  new_player->origin_offset_x = ox;
  new_player->origin_offset_y = oy;
  new_player->hit_points = hp;
  new_player->max_hit_points = hp;

  return new_player;
}

void destroyPlayer(Player* player) {
  destroySpriteArray(&player->sprites);
  player->player->sprite = NULL;
  destroy_gameobject(player->player);
  free(player);
}

void updatePlayerPosition(Player* player) {

  int16_t old_y = (int16_t) player->y;

  player->x += player->speed[0] / 30;
  player->y += player->speed[1] / 30;

  if (player->x < 0) player->x = 0;
  if (player->y < 0) player->y = 0;
  if (player->x > screen.xres) player->x = screen.xres;
  if (player->y > screen.yres) player->y = screen.yres;

  int16_t new_x = (int16_t) player->x;
  int16_t new_y = (int16_t) player->y;

  player->player->x = new_x;
  player->player->y = new_y;

  if (old_y != new_y) {
    if (new_y < 0) new_y = 0;
    updateGameObjectZIndex(player->player, new_y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);
  }
}

void updatePlayerSpriteBasedOnPosition(Player* player) {

  float x = player->speed[0];
  float y = player->speed[1];

  if (x!=0.0f && y!=0.0f) {
    if (x > 0.0f) {
      if (y > 0.0f) {
        updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, DOWN_RIGHT));
      } else if (y < 0.0f) {
        updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, UP_RIGHT));
      }
    } else {
      if (y > 0.0f) {
        updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, DOWN_LEFT));
      } else if (y < 0.0f) {
        updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, UP_LEFT));
      }
    }
  } else {
    if (x > 0.0f) {
      updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, RIGHT));
    } else if (x < 0.0f) {
      updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, LEFT));
    } else if (y > 0.0f) {
      updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, DOWN));
    } else if (y < 0.0f) {
      updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, UP));
    } else {
      updateGameObjectSprite(player->player, getSpriteArray(&player->sprites, STATIONARY));
    }
  }

  player->player->origin_offset_x = -(player->player->sprite->width / 2);
  player->player->origin_offset_y = -(player->player->sprite->height / 2);

}

