// bullets.h
#pragma once

#include "../../../Devices/device_controller.h"

typedef struct Bullet {
    GameObject* bullet;
    Sprite* sprite;
    float x, y;
    float speed[2];
    int16_t origin_offset_x, origin_offset_y;
    int16_t damage;
    bool active;
} Bullet;

typedef struct BulletArray {
    Bullet** bullets;
    uint32_t capacity;
    uint32_t length;
} BulletArray;

Bullet* initializeBullet(float x, float y, int16_t ox, int16_t oy, float speed_x, float speed_y, int16_t damage);
void destroyBullet(Bullet* bullet);
void updateBulletPosition(Bullet* bullet);

BulletArray newBulletArray(uint32_t capacity);
void pushBulletArray(BulletArray* array, Bullet* bullet);
Bullet* getBulletArray(BulletArray* array, uint32_t index);
void removeBulletArray(BulletArray* array, uint32_t index);
void destroyBulletArray(BulletArray* array);
void updateAllBulletPositions(BulletArray* array);
