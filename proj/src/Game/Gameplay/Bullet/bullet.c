// bullets.c
#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "bullet.h"

//Temos de criar um xpm para as bullets
//#include "../../../ImageAssets/Bullet.xpm"

extern ScreenInfo screen;

Bullet* initializeBullet(float x, float y, int16_t ox, int16_t oy, float speed_x, float speed_y, int16_t damage) {
    Bullet* bullet = (Bullet*)malloc(sizeof(Bullet));

    //Add Sprites
    //We need xpm for bullets
    //bullet->sprite = create_sprite((xpm_map_t)Bullet, x, y, false, true);
    //bullet->bullet = create_gameobject_from_sprite(bullet->sprite, x, y, 0, 0, y * Z_INDEX_PER_LAYER + MEDIUM_PRIORITY_Z_INDEX);

    bullet->x = x;
    bullet->y = y;
    bullet->speed[0] = speed_x;
    bullet->speed[1] = speed_y;
    bullet->origin_offset_x = ox;
    bullet->origin_offset_y = oy;
    bullet->damage = damage;
    bullet->active = true;

    return bullet;
}

void destroyBullet(Bullet* bullet) {
    destroy_sprite(bullet->sprite);
    bullet->bullet->sprite = NULL;
    destroy_gameobject(bullet->bullet);
    free(bullet);
}

void updateBulletPosition(Bullet* bullet) {
    bullet->x += bullet->speed[0];
    bullet->y += bullet->speed[1];

    bullet->bullet->x = (int16_t)bullet->x;
    bullet->bullet->y = (int16_t)bullet->y;

    // Check if bullet is out of screen bounds
    if (bullet->x < 0 || bullet->x > screen.xres || bullet->y < 0 || bullet->y > screen.yres) {
        bullet->active = false;
    }
}

BulletArray newBulletArray(uint32_t capacity) {
    BulletArray new_array;
    new_array.length = 0;

    if (capacity) {
        new_array.bullets = (Bullet**)malloc(capacity * sizeof(Bullet*));
        new_array.capacity = capacity;
    } else {
        new_array.bullets = (Bullet**)malloc(10 * sizeof(Bullet*));
        new_array.capacity = 10;
    }

    return new_array;
}

void pushBulletArray(BulletArray* array, Bullet* bullet) {
    if (array->capacity != array->length) {
        array->bullets[array->length] = bullet;
    } else {
        uint32_t newCapacity = array->capacity * 2;
        Bullet** oldPointer = array->bullets;
        Bullet** newPointer = (Bullet**)malloc(newCapacity * sizeof(Bullet*));
        array->bullets = newPointer;
        for (uint32_t i = 0; i < array->length; i++) {
            newPointer[i] = oldPointer[i];
        }
        free(oldPointer);
        array->bullets[array->length] = bullet;
    }
    array->length++;
}

Bullet* getBulletArray(BulletArray* array, uint32_t index) {
    if (index < array->length) {
        return array->bullets[index];
    } else {
        return NULL;
    }
}

void removeBulletArray(BulletArray* array, uint32_t index) {
    if (index < array->length) {
        destroyBullet(array->bullets[index]);
        for (uint32_t i = index; i < array->length - 1; i++) {
            array->bullets[i] = array->bullets[i + 1];
        }
        array->length--;
    }
}

void destroyBulletArray(BulletArray* array) {
    for (uint32_t i = 0; i < array->length; i++) {
        destroyBullet(array->bullets[i]);
    }
    array->length = 0;
}

void updateAllBulletPositions(BulletArray* array) {
    for (uint32_t i = 0; i < array->length; i++) {
        updateBulletPosition(array->bullets[i]);
        if (!array->bullets[i]->active) {
            removeBulletArray(array, i);
            i--; 
        }
    }
}