#pragma once

#include "../../Devices/device_controller.h"

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
} Enemy;

Enemy* initializeEnemy(float x, float y,  int16_t ox, int16_t oy, int16_t hp);
void destroyEnemy(Enemy* enemy);
void updateEnemyPosition(Enemy* enemy);
void updateEnemySpriteBasedOnPosition(Enemy* enemy);