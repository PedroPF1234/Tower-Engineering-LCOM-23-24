#pragma once

#include "../../Devices/device_controller.h"

typedef struct Enemy {
    GameObject* up;
    GameObject* down;
    GameObject* left;
    GameObject* right;
    GameObject* up_left;
    GameObject* up_right;
    GameObject* down_left;
    GameObject* down_right;
    GameObject* stationary;
    int16_t x, y;
    int16_t origin_offset_x, origin_offset_y;
    int16_t hit_points;
} Enemy;

Enemy* initializeEnemy(int16_t x, int16_t y,  int16_t ox, int16_t oy,int16_t hp);
void destroyEnemy(Enemy* enemy);
void updateEnemy(Enemy* enemy, int8_t x, int8_t y);
void updateEnemySpriteBasedOnPosition(Enemy* enemy, int8_t x, int8_t y);
void setAllSpritesInvisible(Enemy* enemy);