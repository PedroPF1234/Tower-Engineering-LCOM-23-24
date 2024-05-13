#include "enemy.h"
#include <stdlib.h>

#include "enemy.h"

#include "../../ImageAssets/Bicho.xpm"

extern ScreenInfo screen;

Enemy* initializeEnemy(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp) {
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));

    //Add the Sprites
    enemy->up = create_gameobject((xpm_map_t)BichoUp, x, y, ox, oy, 1000, false, false);
    enemy->down = create_gameobject((xpm_map_t)BichoDown, x, y, ox, oy, 1000, false, false);
    enemy->left = create_gameobject((xpm_map_t)BichoLeft, x, y, ox, oy, 1000, false, false);
    enemy->right = create_gameobject((xpm_map_t)BichoRight, x, y, ox, oy, 1000, false, false);
    enemy->up_left = create_gameobject((xpm_map_t)BichoUpperLeft, x, y, ox, oy, 1000, false, false);
    enemy->up_right = create_gameobject((xpm_map_t)BichoUpperRight, x, y, ox, oy, 1000, false, false);
    enemy->down_left = create_gameobject((xpm_map_t)BichoLowerLeft, x, y, ox, oy, 1000, false, false);
    enemy->down_right = create_gameobject((xpm_map_t)BichoLowerRight, x, y, ox, oy, 1000, false, false);
    enemy->stationary = create_gameobject((xpm_map_t)BichoStationary, x, y, ox, oy, 1000, false, false);

    enemy->origin_offset_x = ox;
    enemy->origin_offset_y = oy;
    enemy->x = x;
    enemy->y = y;
    enemy->hit_points = hp;
    return enemy;
}

void destroyEnemy(Enemy* enemy) {
  destroy_gameobject(enemy->up);
  destroy_gameobject(enemy->down);
  destroy_gameobject(enemy->left);
  destroy_gameobject(enemy->right);
  destroy_gameobject(enemy->up_left);
  destroy_gameobject(enemy->up_right);
  destroy_gameobject(enemy->down_left);
  destroy_gameobject(enemy->down_right);
  destroy_gameobject(enemy->stationary);
  free(enemy);
}

void updateEnemy(Enemy* enemy, int8_t x, int8_t y) {
    enemy->x += x;
    enemy->y += y;

    if (enemy->x < 0) enemy->x = 0;
    if (enemy->y < 0) enemy->y = 0;
    if (enemy->x > screen.xres) enemy->x = screen.xres;
    if (enemy->y > screen.yres) enemy->y = screen.yres;

    uint16_t new_x = enemy->x;
    uint16_t new_y = enemy->y;

    enemy->up->x = new_x;
    enemy->up->y = new_y;

    enemy->down->x = new_x;
    enemy->down->y = new_y;

    enemy->left->x = new_x;
    enemy->left->y = new_y;

    enemy->right->x = new_x;
    enemy->right->y = new_y;

    enemy->up_left->x = new_x;
    enemy->up_left->y = new_y;

    enemy->up_right->x = new_x;
    enemy->up_right->y = new_y;

    enemy->down_left->x = new_x;
    enemy->down_left->y = new_y;

    enemy->down_right->x = new_x;
    enemy->down_right->y = new_y;

    enemy->stationary->x = new_x;
    enemy->stationary->y = new_y;
}

void setAllSpritesInvisible(Enemy* enemy) {
  enemy->up->sprite->is_visible = false;
  enemy->down->sprite->is_visible = false;
  enemy->left->sprite->is_visible = false;
  enemy->right->sprite->is_visible = false;
  enemy->up_left->sprite->is_visible = false;
  enemy->up_right->sprite->is_visible = false;
  enemy->down_left->sprite->is_visible = false;
  enemy->down_right->sprite->is_visible = false;
  enemy->stationary->sprite->is_visible = false;
}

void updateEnemySpriteBasedOnPosition(Enemy* enemy, int8_t x, int8_t y) {

  setAllSpritesInvisible(enemy);

  if (x!=0 && y!=0) {
    if (x > 0) {
      if (y > 0) {
        enemy->down_right->sprite->is_visible = true;
      } else if (y < 0) {
        enemy->up_right->sprite->is_visible = true;
      }
    } else {
      if (y > 0) {
        enemy->down_left->sprite->is_visible = true;
      } else if (y < 0) {
        enemy->up_left->sprite->is_visible = true;
      }
    }
  } else {
    if (x > 0) {
      enemy->right->sprite->is_visible = true;
    } else if (x < 0) {
      enemy->left->sprite->is_visible = true;
    } else if (y > 0) {
      enemy->down->sprite->is_visible = true;
    } else if (y < 0) {
      enemy->up->sprite->is_visible = true;
    } else {
      enemy->stationary->sprite->is_visible = true;
    }
  }
