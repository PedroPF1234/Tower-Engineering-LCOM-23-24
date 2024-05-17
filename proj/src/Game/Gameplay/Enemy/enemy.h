#pragma once

#include "../../../Devices/device_controller.h"

typedef struct Enemy {
    GameObject* enemy; // Default sprite is stationary
    GameObject* health_bar;
    Sprite* up;
    Sprite* down;
    Sprite* left;
    Sprite* right;
    float x, y;
    float speed[2];
    int16_t origin_offset_x, origin_offset_y;
    int16_t hit_points;
    int16_t max_hit_points;
    int16_t* targets;
    int16_t targets_hit;
    int16_t last_target;
    bool reached_target;
} Enemy;

typedef struct EnemyArray {
    Enemy** enemies;
    uint32_t capacity;
    uint32_t length;
} EnemyArray;

Enemy* initializeEnemy(float x, float y, int16_t ox, int16_t oy, int16_t hp, int16_t* targets, int16_t last_target);
void destroyEnemy(Enemy* enemy);
void updateEnemyPosition(Enemy* enemy);
void updateEnemySpriteBasedOnPosition(Enemy* enemy);

EnemyArray newEnemyArray(uint32_t capacity);
void pushEnemyArray(EnemyArray* array, Enemy* enemy);
Enemy* getEnemyArray(EnemyArray* array, uint32_t index);
void removeEnemyArray(EnemyArray* array, uint32_t index);
void destroyEnemyArray(EnemyArray* array);
void updateAllEnemyPositions(EnemyArray* array);
