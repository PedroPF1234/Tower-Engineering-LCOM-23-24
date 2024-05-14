#pragma once

#include "../../Devices/device_controller.h"

typedef struct Enemy {
    GameObject* player; // Default sprite is stationary
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
    int16_t origin_offset_x, origin_offset_y;
    int16_t hit_points;
} Enemy;

Enemy* initializeEnemy(int16_t x, int16_t y,  int16_t ox, int16_t oy,int16_t hp);
void destroyEnemy(Enemy* enemy);
void updateEnemy(Enemy* enemy, int8_t x, int8_t y);
void updateEnemySpriteBasedOnPosition(Enemy* enemy, int8_t x, int8_t y);
void setAllSpritesInvisible(Enemy* enemy);