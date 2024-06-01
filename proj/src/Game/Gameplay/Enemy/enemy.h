#pragma once

#include "../../../Devices/device_controller.h"

/**
 * @brief Represents an enemy in the game.
 */
typedef struct Enemy {
    GameObject* enemy;             /**< The game object associated with the enemy. */
    GameObject* health_bar;        /**< The game object representing the enemy's health bar. */
    Sprite* up;                    /**< The sprite for the enemy facing up. */
    Sprite* down;                  /**< The sprite for the enemy facing down. */
    Sprite* left;                  /**< The sprite for the enemy facing left. */
    Sprite* right;                 /**< The sprite for the enemy facing right. */
    float x, y;                    /**< The current x and y positions of the enemy. */
    float speed[2];                /**< The speed of the enemy in the x and y directions. */
    int16_t origin_offset_x;       /**< The x offset of the enemy's origin. */
    int16_t origin_offset_y;       /**< The y offset of the enemy's origin. */
    int16_t hit_points;            /**< The current hit points of the enemy. */
    int16_t max_hit_points;        /**< The maximum hit points of the enemy. */
    int16_t* targets;              /**< The array of target positions the enemy moves towards. */
    int16_t targets_hit;           /**< The number of targets the enemy has reached. */
    int16_t last_target;           /**< The index of the last target in the targets array. */
    bool reached_target;           /**< Indicates whether the enemy has reached its final target. */
} Enemy;

typedef struct EnemyArray {
    Enemy** enemies;               /**< Pointer to an array of enemy pointers. */
    uint32_t capacity;             /**< The total capacity of the array. */
    uint32_t length;               /**< The current number of enemies in the array. */
} EnemyArray;

/**
 * @brief Initializes a new enemy.
 *
 * @param x The initial x position of the enemy.
 * @param y The initial y position of the enemy.
 * @param ox The x offset of the enemy's origin.
 * @param oy The y offset of the enemy's origin.
 * @param hp The initial hit points of the enemy.
 * @param targets An array of target positions for the enemy to move towards.
 * @param last_target The index of the last target in the targets array.
 * @return A pointer to the newly initialized Enemy.
 */
Enemy* initializeEnemy(float x, float y, int16_t ox, int16_t oy, int16_t hp, int16_t* targets, int16_t last_target);

/**
 * @brief Destroys an enemy and frees associated memory.
 *
 * @param enemy A pointer to the Enemy to be destroyed.
 */
void destroyEnemy(Enemy* enemy);

/**
 * @brief Updates the position of an enemy based on its speed.
 *
 * @param enemy A pointer to the Enemy to be updated.
 */
void updateEnemyPosition(Enemy* enemy);

/**
 * @brief Updates the sprite of an enemy based on its current position and direction.
 *
 * @param enemy A pointer to the Enemy to be updated.
 */
void updateEnemySpriteBasedOnPosition(Enemy* enemy);

/**
 * @brief Creates a new EnemyArray with a specified capacity.
 *
 * @param capacity The initial capacity of the EnemyArray.
 * @return A new EnemyArray.
 */
EnemyArray newEnemyArray(uint32_t capacity);

/**
 * @brief Adds an enemy to the EnemyArray.
 *
 * @param array A pointer to the EnemyArray.
 * @param enemy A pointer to the Enemy to be added.
 */
void pushEnemyArray(EnemyArray* array, Enemy* enemy);

/**
 * @brief Retrieves an enemy from the EnemyArray at a specified index.
 *
 * @param array A pointer to the EnemyArray.
 * @param index The index of the Enemy to retrieve.
 * @return A pointer to the Enemy at the specified index, or NULL if the index is out of bounds.
 */
Enemy* getEnemyArray(EnemyArray* array, uint32_t index);

/**
 * @brief Removes an enemy from the EnemyArray at a specified index and destroys it.
 *
 * @param array A pointer to the EnemyArray.
 * @param index The index of the Enemy to be removed.
 */
void removeEnemyArray(EnemyArray* array, uint32_t index);

/**
 * @brief Destroys an EnemyArray and frees associated memory.
 *
 * @param array A pointer to the EnemyArray to be destroyed.
 */
void destroyEnemyArray(EnemyArray* array);

/**
 * @brief Updates the positions of all enemies in the EnemyArray and removes any enemies that have reached their final target.
 *
 * @param array A pointer to the EnemyArray.
 */
void updateAllEnemyPositions(EnemyArray* array);