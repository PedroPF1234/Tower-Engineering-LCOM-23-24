// bullets.h
#pragma once

#include "../../../Devices/device_controller.h"

/**
 * @brief Represents a bullet in the game.
 */
typedef struct Bullet {
  GameObject* bullet;          /**< The game object associated with the bullet. */
    Sprite* sprite;              /**< The sprite associated with the bullet. */
    float x, y;                  /**< The current x and y positions of the bullet. */
    float speed[2];              /**< The speed of the bullet in the x and y directions. */
    int16_t origin_offset_x;     /**< The x offset of the bullet's origin. */
    int16_t origin_offset_y;     /**< The y offset of the bullet's origin. */
    int16_t damage;              /**< The damage of the bullet. */
    bool active;                 /**< Indicates whether the bullet is active. */
} Bullet;

/**
 * @brief Represents an array of bullets.
 */
typedef struct BulletArray {
    Bullet** bullets;            /**< Pointer to an array of bullet pointers. */
    uint32_t capacity;           /**< The total capacity of the array. */
    uint32_t length;             /**< The current number of bullets in the array. */
} BulletArray;

/**
 * @brief Initializes a new bullet.
 *
 * @param x The initial x position of the bullet.
 * @param y The initial y position of the bullet.
 * @param ox The x offset of the bullet's origin.
 * @param oy The y offset of the bullet's origin.
 * @param speed_x The initial speed of the bullet in the x direction.
 * @param speed_y The initial speed of the bullet in the y direction.
 * @param damage The damage of the bullet
 * @return A pointer to the newly initialized Bullet.
 */
Bullet* initializeBullet(float x, float y, int16_t ox, int16_t oy, float speed_x, float speed_y, int16_t damage);

/**
 * @brief Destroys a bullet and frees associated memory.
 *
 * @param bullet A pointer to the Bullet to be destroyed.
 */
void destroyBullet(Bullet* bullet);

/**
 * @brief Updates the position of a bullet based on its speed.
 *
 * @param bullet A pointer to the Bullet to be updated.
 */
void updateBulletPosition(Bullet* bullet);

/**
 * @brief Creates a new BulletArray with a specified capacity.
 *
 * @param capacity The initial capacity of the BulletArray.
 * @return A new BulletArray.
 */
BulletArray newBulletArray(uint32_t capacity);

/**
 * @brief Adds a bullet to the BulletArray.
 *
 * @param array A pointer to the BulletArray.
 * @param bullet A pointer to the Bullet to be added.
 */
void pushBulletArray(BulletArray* array, Bullet* bullet);

/**
 * @brief Retrieves a bullet from the BulletArray at a specified index.
 *
 * @param array A pointer to the BulletArray.
 * @param index The index of the Bullet to retrieve.
 * @return A pointer to the Bullet at the specified index, or NULL if the index is out of bounds.
 */
Bullet* getBulletArray(BulletArray* array, uint32_t index);

/**
 * @brief Removes a bullet from the BulletArray at a specified index and destroys it.
 *
 * @param array A pointer to the BulletArray.
 * @param index The index of the Bullet to be removed.
 */
void removeBulletArray(BulletArray* array, uint32_t index);

/**
 * @brief Destroys a BulletArray and frees associated memory.
 *
 * @param array A pointer to the BulletArray to be destroyed.
 */
void destroyBulletArray(BulletArray* array);

/**
 * @brief Updates the positions of all bullets in the BulletArray and removes any bullets that are no longer active.
 *
 * @param array A pointer to the BulletArray.
 */
void updateAllBulletPositions(BulletArray* array);
