#include <lcom/lcf.h>
#include <stdint.h>

#include "player.h"

#include "../../ImageAssets/Bicho.xpm"

Player* initializePlayer(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp) {
  Player* new_player = (Player*)malloc(sizeof(Player));

  new_player->up = create_gameobject((xpm_map_t)BichoUp, x, y, ox, oy, 1000, false, false);
  new_player->down = create_gameobject((xpm_map_t)BichoDown, x, y, ox, oy, 1000, false, false);
  new_player->left = create_gameobject((xpm_map_t)BichoLeft, x, y, ox, oy, 1000, false, false);
  new_player->right = create_gameobject((xpm_map_t)BichoRight, x, y, ox, oy, 1000, false, false);
  new_player->up_left = create_gameobject((xpm_map_t)BichoUpperLeft, x, y, ox, oy, 1000, false, false);
  new_player->up_right = create_gameobject((xpm_map_t)BichoUpperRight, x, y, ox, oy, 1000, false, false);
  new_player->down_left = create_gameobject((xpm_map_t)BichoLowerLeft, x, y, ox, oy, 1000, false, false);
  new_player->down_right = create_gameobject((xpm_map_t)BichoLowerRight, x, y, ox, oy, 1000, false, false);
  new_player->stationary = create_gameobject((xpm_map_t)BichoStationary, x, y, ox, oy, 1000, false, false);
  new_player->x = x;
  new_player->y = y;
  new_player->origin_offset_x = ox;
  new_player->origin_offset_y = oy;
  new_player->hit_points = hp;

  return new_player;
}

void destroyPlayer(Player* player) {
  destroy_gameobject(player->up);
  destroy_gameobject(player->down);
  destroy_gameobject(player->left);
  destroy_gameobject(player->right);
  destroy_gameobject(player->up_left);
  destroy_gameobject(player->up_right);
  destroy_gameobject(player->down_left);
  destroy_gameobject(player->down_right);
  destroy_gameobject(player->stationary);
  free(player);
}

void updatePlayerPosition(Player* player, int8_t x, int8_t y) {
  player->x += x;
  player->y += y;

  player->up->x += x;
  player->up->y += y;

  player->down->x += x;
  player->down->y += y;

  player->left->x += x;
  player->left->y += y;

  player->right->x += x;
  player->right->y += y;

  player->up_left->x += x;
  player->up_left->y += y;

  player->up_right->x += x;
  player->up_right->y += y;

  player->down_left->x += x;
  player->down_left->y += y;

  player->down_right->x += x;
  player->down_right->y += y;

  player->stationary->x += x;
  player->stationary->y += y;
}

void setAllSpritesInvisible(Player* player) {
  player->up->sprite->is_visible = false;
  player->down->sprite->is_visible = false;
  player->left->sprite->is_visible = false;
  player->right->sprite->is_visible = false;
  player->up_left->sprite->is_visible = false;
  player->up_right->sprite->is_visible = false;
  player->down_left->sprite->is_visible = false;
  player->down_right->sprite->is_visible = false;
  player->stationary->sprite->is_visible = false;
}

void updatePlayerSpriteBasedOnPosition(Player* player, int8_t x, int8_t y) {

  setAllSpritesInvisible(player);

  if (x!=0 && y!=0) {
    if (x > 0) {
      if (y > 0) {
        player->down_right->sprite->is_visible = true;
      } else if (y < 0) {
        player->up_right->sprite->is_visible = true;
      }
    } else {
      if (y > 0) {
        player->down_left->sprite->is_visible = true;
      } else if (y < 0) {
        player->up_left->sprite->is_visible = true;
      }
    }
  } else {
    if (x > 0) {
      player->right->sprite->is_visible = true;
    } else if (x < 0) {
      player->left->sprite->is_visible = true;
    } else if (y > 0) {
      player->down->sprite->is_visible = true;
    } else if (y < 0) {
      player->up->sprite->is_visible = true;
    } else {
      player->stationary->sprite->is_visible = true;
    }
  }

}
