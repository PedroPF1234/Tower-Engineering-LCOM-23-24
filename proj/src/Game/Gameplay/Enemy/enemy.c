#include "enemy.h"
#include <stdlib.h>

#include "enemy.h"

#include "../../ImageAssets/Bicho.xpm"

extern ScreenInfo screen;

Enemy* initializeEnemy(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp) {
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));

    //Add the Sprites
    enemy->up = create_sprite((xpm_map_t)BichoUp, x, y, false, false);
    enemy->down = create_sprite((xpm_map_t)BichoDown, x, y,  false, false);
    enemy->left = create_sprite((xpm_map_t)BichoLeft, x, y, false, false);
    enemy->right = create_sprite((xpm_map_t)BichoRight, x, y, false, false);
    enemy->up_left = create_sprite((xpm_map_t)BichoUpperLeft, x, y, false, false);
    enemy->up_right = create_sprite((xpm_map_t)BichoUpperRight, x, y, false, false);
    enemy->down_left = create_sprite((xpm_map_t)BichoLowerLeft, x, y, false, false);
    enemy->down_right = create_sprite((xpm_map_t)BichoLowerRight, x, y, false, false);
    enemy->stationary = create_sprite((xpm_map_t)BichoStationary, x, y, false, false);

    enemy->enemy = create_gameobject_from_sprite(enemy->stationary, x, y, ox, oy, y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);

    enemy->origin_offset_x = ox;
    enemy->origin_offset_y = oy;
    enemy->speed[0] = 0.0f;
    enemy->speed[1] = 0.0f;
    enemy->x = x;
    enemy->y = y;
    enemy->hit_points = hp;

    return enemy;
}

void destroyEnemy(Enemy* enemy) {
  destroy_spritet(enemy->up);
  destroy_sprite(enemy->down);
  destroy_sprite(enemy->left);
  destroy_sprite(enemy->right);
  destroy_sprite(enemy->up_left);
  destroy_sprite(enemy->up_right);
  destroy_sprite(enemy->down_left);
  destroy_sprite(enemy->down_right);
  destroy_sprite(enemy->stationary);
  enemy->enemy->sprite = NULL;
  destroy_gameobject(enemy->enemy);
  free(enemy);
}

void updateEnemyPosition(Enemy* enemy, int8_t x, int8_t y) {
  enemy->x += enemy->speed[0];
  enemy->y += enemy->speed[1];

  if (enemy->x < 0) enemy->x = 0;
  if (enemy->y < 0) enemy->y = 0;
  if (enemy->x > screen.xres) enemy->x = screen.xres;
  if (enemy->y > screen.yres) enemy->y = screen.yres;

  uint16_t new_x = (uint16_t) enemy->x;
  uint16_t new_y = (uint16_t) enemy->y;

  enemy->enemy->x = new_x;
  enemy->enemy->y = new_y;

  updateGameObjectZIndex(enemy->enemy, new_y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);
}

void updateEnemySpriteBasedOnPosition(Enemy* enemy) {

  float x = enemy->speed[0];
  float y = enemy->speed[1];

  if (x!=0.0f && y!=0.0f) {
    if (x > 0.0f) {
      if (y > 0.0f) {
        updateGameObjectSprite(enemy->enemy, enemy->down_right);
      } else if (y < 0.0f) {
        updateGameObjectSprite(enemy->enemy, enemy->up_right);
      }
    } else {
      if (y > 0.0f) {
        updateGameObjectSprite(enemy->enemy, enemy->down_left);
      } else if (y < 0.0f) {
        updateGameObjectSprite(enemy->enemy, enemy->up_left);
      }
    }
  } else {
    if (x > 0.0f) {
      updateGameObjectSprite(enemy->enemy, enemy->right);
    } else if (x < 0.0f) {
      updateGameObjectSprite(enemy->enemy, enemy->left);
    } else if (y > 0.0f) {
      updateGameObjectSprite(enemy->enemy, enemy->down);
    } else if (y < 0.0f) {
      updateGameObjectSprite(enemy->enemy, enemy->up);
    } else {
      updateGameObjectSprite(enemy->enemy, enemy->stationary);
    }
  }
}
