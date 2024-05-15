#include <lcom/lcf.h>
#include <stdint.h>

#include "enemy.h"

#include "../../../ImageAssets/Bicho.xpm"

extern ScreenInfo screen;

Enemy* initializeEnemy(float x, float y, int16_t ox, int16_t oy, int16_t hp, int16_t target_x, int16_t target_y) {
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
  enemy->current_target_x = target_x;
  enemy->current_target_y = target_y;
  enemy->targets_hit = 0;

  return enemy;
}

void destroyEnemy(Enemy* enemy) {
  destroy_sprite(enemy->up);
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

void updateEnemyPosition(Enemy* enemy) {
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

EnemyArray newEnemyArray(uint32_t capacity) {
  EnemyArray new_array;
  new_array.length = 0;

  if (capacity) {
    new_array.enemies = (Enemy*)malloc(capacity * sizeof(Enemy));
    new_array.capacity = capacity;
  } else { // Default capacity
    new_array.enemies = (Enemy*)malloc(10 * sizeof(Enemy));
    new_array.capacity = 10;
  }

  return new_array;
}

void pushEnemyArray(EnemyArray* array, Enemy* enemy) {
  if (array->capacity != array->length) {
      array->enemies[array->length] = *enemy;
  } else {
    uint32_t newCapacity = array->capacity * 2;
    Enemy* oldPointer = array->enemies;
    Enemy* newPointer = (Enemy*)malloc(newCapacity * sizeof(Enemy));
    array->enemies = newPointer;
    for (uint32_t i = 0; i < array->length; i++) {
      newPointer[i] = oldPointer[i];
    }
    free(oldPointer);
    array->enemies[array->length] = *enemy;
  }
  array->length++;
}

Enemy* getEnemyArray(EnemyArray* array, uint32_t index) {
  if (index < array->length) {
      return &array->enemies[index];
  } else {
      return NULL;
  }
}

void removeEnemyArray(EnemyArray* array, uint32_t index) {
  if (index < array->length) {
      destroyEnemy(&array->enemies[index]);
      for (uint32_t i = index; i < array->length - 1; i++) {
          array->enemies[i] = array->enemies[i + 1];
      }
      memset(&array->enemies[array->length - 1], 0, sizeof(Enemy));
      array->length--;
  }
}

void destroyEnemyArray(EnemyArray* array) {
  for (uint32_t i = 0; i < array->length; i++) {
      destroyEnemy(&array->enemies[i]);
  }
  array->length = 0;
  memset(array->enemies, 0, array->capacity * sizeof(Enemy));
}
