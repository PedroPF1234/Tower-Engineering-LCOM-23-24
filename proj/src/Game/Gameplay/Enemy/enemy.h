#pragma once

#include "../../../Devices/device_controller.h"

typedef struct Enemy {
    GameObject* enemy; // Default sprite is stationary
    Sprite* up;
    Sprite* down;
    Sprite* left;
    Sprite* right;
    Sprite* up_left;
    Sprite* up_right;
    Sprite* down_left;
    Sprite* down_right;
    Sprite* stationary;
    float x, y;
    float speed[2];
    int16_t origin_offset_x, origin_offset_y;
    int16_t hit_points;
    int16_t current_target_x, current_target_y;
    int16_t targets_hit;
} Enemy;

typedef struct EnemyArray {
    Enemy* enemies;
    uint32_t capacity;
    uint32_t length;
} EnemyArray;

Enemy* initializeEnemy(float x, float y, int16_t ox, int16_t oy, int16_t hp, int16_t target_x, int16_t target_y);
void destroyEnemy(Enemy* enemy);
void updateEnemyPosition(Enemy* enemy);
void updateEnemySpriteBasedOnPosition(Enemy* enemy);

EnemyArray newEnemyArray(uint32_t capacity);
void pushEnemyArray(EnemyArray* array, Enemy* enemy);
Enemy* getEnemyArray(EnemyArray* array, uint32_t index);
void removeEnemyArray(EnemyArray* array, uint32_t index);
void destroyEnemyArray(EnemyArray* array);
