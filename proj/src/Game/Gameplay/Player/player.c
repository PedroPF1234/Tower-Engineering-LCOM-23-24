#include <lcom/lcf.h>
#include <stdint.h>

#include "player.h"

#include "../../../ImageAssets/Bicho.xpm"

extern ScreenInfo screen;

Player* initializePlayer(float x, float y, int16_t ox, int16_t oy, int16_t hp) {
  Player* new_player = (Player*)malloc(sizeof(Player));

  new_player->up = create_sprite((xpm_map_t)BichoUp, x, y, false, true);
  new_player->down = create_sprite((xpm_map_t)BichoDown, x, y, false, true);
  new_player->left = create_sprite((xpm_map_t)BichoLeft, x, y, false, true);
  new_player->right = create_sprite((xpm_map_t)BichoRight, x, y, false, true);
  new_player->up_left = create_sprite((xpm_map_t)BichoUpperLeft, x, y, false, true);
  new_player->up_right = create_sprite((xpm_map_t)BichoUpperRight, x, y, false, true);
  new_player->down_left = create_sprite((xpm_map_t)BichoLowerLeft, x, y, false, true);
  new_player->down_right = create_sprite((xpm_map_t)BichoLowerRight, x, y, false, true);
  new_player->stationary = create_sprite((xpm_map_t)BichoStationary, x, y, false, false);

  new_player->player = create_gameobject_from_sprite(new_player->stationary, x, y, ox, oy, y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);

  new_player->x = x;
  new_player->y = y;
  new_player->speed[0] = 0.0f;
  new_player->speed[1] = 0.0f;
  new_player->origin_offset_x = ox;
  new_player->origin_offset_y = oy;
  new_player->hit_points = hp;

  return new_player;
}

void destroyPlayer(Player* player) {
  destroy_sprite(player->up);
  destroy_sprite(player->down);
  destroy_sprite(player->left);
  destroy_sprite(player->right);
  destroy_sprite(player->up_left);
  destroy_sprite(player->up_right);
  destroy_sprite(player->down_left);
  destroy_sprite(player->down_right);
  destroy_sprite(player->stationary);
  player->player->sprite = NULL;
  destroy_gameobject(player->player);
  free(player);
}

void updatePlayerPosition(Player* player) {

  int16_t old_y = (int16_t) player->y;

  player->x += player->speed[0];
  player->y += player->speed[1];

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
        updateGameObjectSprite(player->player, player->down_right);
      } else if (y < 0.0f) {
        updateGameObjectSprite(player->player, player->up_right);
      }
    } else {
      if (y > 0.0f) {
        updateGameObjectSprite(player->player, player->down_left);
      } else if (y < 0.0f) {
        updateGameObjectSprite(player->player, player->up_left);
      }
    }
  } else {
    if (x > 0.0f) {
      updateGameObjectSprite(player->player, player->right);
    } else if (x < 0.0f) {
      updateGameObjectSprite(player->player, player->left);
    } else if (y > 0.0f) {
      updateGameObjectSprite(player->player, player->down);
    } else if (y < 0.0f) {
      updateGameObjectSprite(player->player, player->up);
    } else {
      updateGameObjectSprite(player->player, player->stationary);
    }
  }

}

