#include <lcom/lcf.h>
#include <stdint.h>

#include "enemy.h"

#include "../../../ImageAssets/Enemy.xpm"

extern ScreenInfo screen;

#define ENEMY_RED_HEALTH_COLOR {0x00, 0x00, 0xFF};
#define ENEMY_GREEN_HEALTH_COLOR {0x49, 0xFF, 0x15};

Enemy* initializeEnemy(float x, float y, int16_t ox, int16_t oy, int16_t hp, int16_t* targets, int16_t last_target) {
  Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));

  //Add the Sprites
  enemy->up = create_sprite((xpm_map_t)EnemyUp, x, y, false, true);
  enemy->down = create_sprite((xpm_map_t)EnemyDown, x, y,  false, true);
  enemy->left = create_sprite((xpm_map_t)EnemyLeft, x, y, false, true);
  enemy->right = create_sprite((xpm_map_t)EnemyRight, x, y, false, true);

  enemy->enemy = create_gameobject_from_sprite(enemy->right, x, y, ox, oy, y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);

  Sprite* healthBar = create_sprite((xpm_map_t)HealthBar, x, y, false, true);
  enemy->health_bar = create_gameobject_from_sprite(healthBar, x, y, 0, 0, y * Z_INDEX_PER_LAYER + HIGH_PRIORITY_Z_INDEX);

  enemy->origin_offset_x = ox;
  enemy->origin_offset_y = oy;
  enemy->speed[0] = 0.0f;
  enemy->speed[1] = 0.0f;
  enemy->x = x;
  enemy->y = y;
  enemy->hit_points = hp;
  enemy->max_hit_points = hp;
  enemy->targets = targets;
  enemy->targets_hit = 0;
  enemy->last_target = last_target;
  enemy->reached_target = false;

  return enemy;
}

void destroyEnemy(Enemy* enemy) {
  destroy_sprite(enemy->up);
  destroy_sprite(enemy->down);
  destroy_sprite(enemy->left);
  destroy_sprite(enemy->right);
  enemy->enemy->sprite = NULL;
  destroy_gameobject(enemy->enemy);
  destroy_gameobject(enemy->health_bar);
  free(enemy);
}

void updateEnemyPosition(Enemy* enemy) {

  if (enemy->speed[0] == 0.0f && enemy->speed[1] == 0.0f && !enemy->reached_target) {
    float target_x = (float)enemy->targets[enemy->targets_hit * 2];
    float target_y = (float)enemy->targets[enemy->targets_hit * 2 + 1];

    float x_diff = target_x - enemy->enemy->x;
    float y_diff = target_y - enemy->enemy->y;

    if (x_diff > 0) enemy->speed[0] = 10.0f;
    if (x_diff < 0) enemy->speed[0] = -10.0f;
    if (y_diff > 0) enemy->speed[1] = 10.0f;
    if (y_diff < 0) enemy->speed[1] = -10.0f;
  }

  int16_t old_y = (int16_t) enemy->y;

  enemy->x += (enemy->speed[0] / 30);
  enemy->y += (enemy->speed[1] / 30);

  if (enemy->x < 0) enemy->x = 0;
  if (enemy->y < 0) enemy->y = 0;
  if (enemy->x > screen.xres) enemy->x = screen.xres;
  if (enemy->y > screen.yres) enemy->y = screen.yres;

  int16_t new_x = (int16_t) enemy->x;
  int16_t new_y = (int16_t) enemy->y;

  enemy->enemy->x = new_x;
  enemy->enemy->y = new_y;
  enemy->health_bar->x = new_x;
  enemy->health_bar->y = new_y;

  if (old_y != new_y) {
    if (new_y < 0) new_y = 0;
    updateGameObjectZIndex(enemy->enemy, new_y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);
    updateGameObjectZIndex(enemy->health_bar, new_y * Z_INDEX_PER_LAYER + HIGH_PRIORITY_Z_INDEX);
  }

  if (new_x == enemy->targets[enemy->targets_hit * 2] && new_y == enemy->targets[enemy->targets_hit * 2 + 1]) {
    enemy->speed[0] = 0.0f;
    enemy->speed[1] = 0.0f;
    enemy->targets_hit++;

    if (enemy->targets_hit == enemy->last_target) {
      enemy->speed[0] = 0.0f;
      enemy->speed[1] = 0.0f;
      enemy->reached_target = true;
    }
  }

  // Temp section to test health bar
  //if (enemy->hit_points > 0) {
  //  enemy->hit_points--;
  //} else {
  //  enemy->hit_points = enemy->max_hit_points;
  //}

}

void updateEnemySpriteBasedOnPosition(Enemy* enemy) {

  float x = enemy->speed[0];
  float y = enemy->speed[1];

  if (x != 0.0f) {
    if (x > 0.0f) {
      updateGameObjectSprite(enemy->enemy, enemy->right);
    } else if (x < 0.0f) {
      updateGameObjectSprite(enemy->enemy, enemy->left);
    }
  } else if (y != 0.0f) {
    if (y > 0.0f) {
      updateGameObjectSprite(enemy->enemy, enemy->down);
    } else if (y < 0.0f) {
      updateGameObjectSprite(enemy->enemy, enemy->up);
    }
  }

  if (enemy->hit_points != enemy->max_hit_points) {

    if (enemy->hit_points < 0) enemy->hit_points = 0;
    int16_t healt_width = enemy->health_bar->sprite->width;
    int16_t health_height = enemy->health_bar->sprite->height;

    int16_t health_current_width = (healt_width * enemy->hit_points) / enemy->max_hit_points;

    Sprite* health_bar = enemy->health_bar->sprite;

    for (int i = 0; i < health_current_width; i++) {
      for (int j = 0; j < health_height; j++) {
        uint8_t color[3] = ENEMY_GREEN_HEALTH_COLOR;
        memcpy(health_bar->map + (i + j * healt_width) * 3, &color, 3);
      }
    }

    for (int i = health_current_width; i < healt_width; i++) {
      for (int j = 0; j < health_height; j++) {
        uint8_t color[3] = ENEMY_RED_HEALTH_COLOR;
        memcpy(health_bar->map + (i + j * healt_width) * 3, color, 3);
      }
    }
  }

  enemy->enemy->origin_offset_x = -(enemy->enemy->sprite->width / 2);
  enemy->enemy->origin_offset_y = -(enemy->enemy->sprite->height / 2);
  enemy->health_bar->origin_offset_x = -(enemy->health_bar->sprite->width / 2);
  enemy->health_bar->origin_offset_y = -(enemy->enemy->sprite->height / 2) - 10;
}

// Set capacity to 0 for default capacity
EnemyArray newEnemyArray(uint32_t capacity) {
  EnemyArray new_array;
  new_array.length = 0;

  if (capacity) {
    new_array.enemies = (Enemy**)malloc(capacity * sizeof(Enemy*));
    new_array.capacity = capacity;
  } else { // Default capacity
    new_array.enemies = (Enemy**)malloc(10 * sizeof(Enemy*));
    new_array.capacity = 10;
  }

  return new_array;
}

void pushEnemyArray(EnemyArray* array, Enemy* enemy) {
  if (array->capacity != array->length) {
      array->enemies[array->length] = enemy;
  } else {
    uint32_t newCapacity = array->capacity * 2;
    Enemy** oldPointer = array->enemies;
    Enemy** newPointer = (Enemy**)malloc(newCapacity * sizeof(Enemy*));
    array->enemies = newPointer;
    for (uint32_t i = 0; i < array->length; i++) {
      newPointer[i] = oldPointer[i];
    }
    free(oldPointer);
    array->enemies[array->length] = enemy;
  }
  array->length++;
}

Enemy* getEnemyArray(EnemyArray* array, uint32_t index) {
  if (index < array->length) {
      return array->enemies[index];
  } else {
      return NULL;
  }
}

void removeEnemyArray(EnemyArray* array, uint32_t index) {
  if (index < array->length) {
    destroyEnemy(getEnemyArray(array, index));
    for (uint32_t i = index; i < array->length - 1; i++) {
      array->enemies[i] = array->enemies[i + 1];
    }
    array->length--;
  }
}

void destroyEnemyArray(EnemyArray* array) {
  for (uint32_t i = 0; i < array->length; i++) {
    destroyEnemy(array->enemies[i]);
  }
  array->length = 0;
}

uint32_t updateAllEnemyPositions(EnemyArray* array) {

  uint32_t damage = 0;

  for (uint32_t i = 0; i < array->length; i++) {
    updateEnemyPosition(array->enemies[i]);
    updateEnemySpriteBasedOnPosition(array->enemies[i]);
  }
  
  if (getEnemyArray(array, 0) != NULL) {
    if (getEnemyArray(array, 0)->reached_target) {
      removeEnemyArray(array, 0);
      damage += 300;
    }
  }

  return damage;

}
