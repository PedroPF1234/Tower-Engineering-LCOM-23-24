#include <lcom/lcf.h>
#include <stdint.h>

#include "player.h"

#include "../../../ImageAssets/Player.xpm"

extern ScreenInfo screen;

Player* initializePlayer(float x, float y, int16_t ox, int16_t oy, int16_t hp) {
  Player* new_player = (Player*)malloc(sizeof(Player));

  char** playerSprites[] = {PlayerUp1, PlayerUp2, PlayerDown1, PlayerDown2, PlayerLeft1, PlayerLeft2, PlayerRight1, PlayerRight2, PlayerUpLeft1, PlayerUpLeft2, PlayerUpRight1, PlayerUpRight2, PlayerDownLeft1, PlayerDownLeft2, PlayerDownRight1, PlayerDownRight2};

  new_player->sprites = newAnimatedSpriteArrayFromSprites(playerSprites, 2, 8, 166);

  new_player->player = create_animated_gameobject(getAnimatedSpriteArray(&new_player->sprites, DOWN), x, y, 0);

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
  destroyAnimatedSpriteArray(&player->sprites);
  destroy_animated_gameobject(player->player);
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

  player->player->gameObject->x = new_x;
  player->player->gameObject->y = new_y;

  if (old_y != new_y) {
    if (new_y < 0) new_y = 0;
    updateGameObjectZIndex(player->player->gameObject, new_y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);
  }
}

void updatePlayerSpriteBasedOnPosition(Player* player) {

  float x = player->speed[0];
  float y = player->speed[1];

  if (x!=0.0f && y!=0.0f) {
    if (x > 0.0f) {
      if (y > 0.0f) {
        switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, DOWN_RIGHT));
      } else if (y < 0.0f) {
        switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, UP_RIGHT));
      }
    } else {
      if (y > 0.0f) {
        switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, DOWN_LEFT));
      } else if (y < 0.0f) {
        switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, UP_LEFT));
      }
    }
  } else {
    if (x > 0.0f) {
      switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, RIGHT));
    } else if (x < 0.0f) {
      switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, LEFT));
    } else if (y > 0.0f) {
      switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, DOWN));
    } else if (y < 0.0f) {
      switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, UP));
    }
  }

  player->player->gameObject->origin_offset_x = -(player->player->gameObject->sprite->width / 2);
  player->player->gameObject->origin_offset_y = -(player->player->gameObject->sprite->height / 2);

}

