#include <lcom/lcf.h>
#include <stdint.h>

#include "player.h"

#include "../../../ImageAssets/Player.xpm"


extern ScreenInfo screen;

Player* initializePlayer(float x, float y, int16_t ox, int16_t oy, int16_t hp) {
  Player* new_player = (Player*)malloc(sizeof(Player));

  char** playerAnimatedSprites[] = {PlayerUp1, PlayerUp2, PlayerDown1, PlayerDown2, PlayerLeft1, PlayerLeft2, PlayerRight1, PlayerRight2, PlayerUpLeft1, PlayerUpLeft2, PlayerUpRight1, PlayerUpRight2, PlayerDownLeft1, PlayerDownLeft2, PlayerDownRight1, PlayerDownRight2};

  char** playerIdleSprites[] = {PlayerUp, PlayerDown, PlayerLeft, PlayerRight};

  AnimatedSpriteArray temp = newAnimatedSpriteArrayFromSprites(playerIdleSprites, 1, 4, 0);

  new_player->sprites = newAnimatedSpriteArrayFromSprites(playerAnimatedSprites, 2, 8, 166);
  for (uint32_t i = 0; i < temp.length; i++) {
    pushAnimatedSpriteArray(&new_player->sprites, getAnimatedSpriteArray(&temp, i));
  }

  new_player->player = create_animated_gameobject(getAnimatedSpriteArray(&new_player->sprites, DOWN), x, y, 0);
  new_player->current_direction = DOWN;

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

void updatePlayerPosition(Player* player, Arena arena) {

  int16_t old_y = (int16_t) player->y;

  bool can_move = true;

  float new_x = player->x;
  float new_y = player->y;

  new_x += player->speed[0] / 30;
  new_y += player->speed[1] / 30;

  if (new_x < 0) player->x = 0;
  if (new_y < 0) player->y = 0;
  if (new_x > screen.xres) player->x = screen.xres;
  if (new_y > screen.yres) player->y = screen.yres;

  // Collision Detection 
  int16_t shop_left_corner = arena.shop_x - 95;
  int16_t shop_right_corner = arena.shop_x + 95;

  if (new_x > shop_left_corner && new_x < shop_right_corner && new_y <= arena.shop_y && new_y >= arena.shop_y - 40) {
    can_move = false;
  }

  if (can_move) {
    player->x = new_x;
    player->y = new_y;

    player->player->gameObject->x = (int16_t) new_x;
    player->player->gameObject->y = (int16_t) new_y;

    if (old_y != (int16_t) new_y) {
      if (new_y < 0) new_y = 0;
      updateGameObjectZIndex(player->player->gameObject, (int16_t) new_y * Z_INDEX_PER_LAYER + MEDIUM_PRIORITY_Z_INDEX);
    }
  }
}

void updatePlayerSpriteBasedOnPosition(Player* player) {

  float x = player->speed[0];
  float y = player->speed[1];

  if (x!=0.0f && y!=0.0f) {
    if (x > 0.0f) {
      if (y > 0.0f && player->current_direction != DOWN_RIGHT) {
        switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, DOWN_RIGHT));
        player->current_direction = DOWN_RIGHT;
      } else if (y < 0.0f && player->current_direction != UP_RIGHT) {
        switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, UP_RIGHT));
        player->current_direction = UP_RIGHT;
      }
    } else {
      if (y > 0.0f && player->current_direction != DOWN_LEFT) {
        switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, DOWN_LEFT));
        player->current_direction = DOWN_LEFT;
      } else if (y < 0.0f && player->current_direction != UP_LEFT) {
        switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, UP_LEFT));
        player->current_direction = UP_LEFT;
      }
    }
  } else {
    if (x > 0.0f && player->current_direction != RIGHT) {
      switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, RIGHT));
      player->current_direction = RIGHT;
    } else if (x < 0.0f && player->current_direction != LEFT) {
      switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, LEFT));
      player->current_direction = LEFT;
    } else if (y > 0.0f && player->current_direction != DOWN) {
      switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, DOWN));
      player->current_direction = DOWN;
    } else if (y < 0.0f && player->current_direction != UP) {
      switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, UP));
      player->current_direction = UP;
    } else if (x == 0.0f && y == 0.0f) {
      switch(player->current_direction) {
        case UP:
          switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, UP_IDLE));
          player->current_direction = UP_IDLE;
          break;
        case DOWN:
          switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, DOWN_IDLE));
          player->current_direction = DOWN_IDLE;
          break;
        case LEFT:
          switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, LEFT_IDLE));
          player->current_direction = LEFT_IDLE;
          break;
        case RIGHT:
          switchAnimatedSpriteOfAnimatedGameObject(player->player, getAnimatedSpriteArray(&player->sprites, RIGHT_IDLE));
          player->current_direction = RIGHT_IDLE;
          break;
        default:
          break;
      }
    }
  }

  player->player->gameObject->origin_offset_x = -(player->player->gameObject->sprite->width / 2);
  player->player->gameObject->origin_offset_y = -(player->player->gameObject->sprite->height);

}

