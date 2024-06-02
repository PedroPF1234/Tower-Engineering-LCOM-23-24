#pragma once

#include "../../../Devices/device_controller.h"

#include "../Enemy/enemy.h"

/**
 * @brief Represents a tower in the game.
 */

// TowerTargetting is an enum type that represents the different ways a tower can target enemies.
typedef enum TowerTargetting {
  FIRST,
  LAST,
  CLOSEST,
} TowerTargetting;

// TurretType is an enum type that represents the different types of turrets that can be mounted on a tower.
typedef enum TurretType {
  CROSSBOW,
  CANNON,
  LASER
} TurretType;

/**
 * @brief Represents a tower placeholder in the game.
 */
typedef struct TowerBase {
  GameObject* base; // The game object associated with the tower base.
  GameObject* turret; // The game object associated with the turret currently mounted on the tower base.
  Sprite* baseNormal; // The sprite for the tower base when the cursor is not hovering over it.
  Sprite* baseHovered; // The sprite for the tower base when the cursor is hovering over it.
  Sprite* turretSprite; // The sprite for the turret currently mounted on the tower base.
  int8_t turretType; // The type of turret currently mounted on the tower base.
  int16_t x, y; // The current x and y positions of the tower base.
  int16_t origin_offset_x, origin_offset_y; // The x and y offsets of the tower base's origin.
  uint16_t range; // The range within which a player can press E to access the tower menu.
  uint16_t turret_radius; // The range of fire of the turret mounted on the tower base.
  uint16_t damage; // The damage dealt by the turret mounted on the tower base.
  uint8_t level; // The level of the tower mounted on the tower base. (increased by upgrading the tower)
  TowerTargetting targetting; // The targetting strategy of the turret mounted on the tower base.
  Enemy* target; // The current enemy target of the turret mounted on the tower base.
  int cooldown; // specific for each type of tower. The time between shots of the turret mounted on the tower base.
  int timeWithoutShooting; // The time since the turret mounted on the tower base last shot.
} TowerBase;

/**
 * @brief Represents an array of TowerBases.
 */
typedef struct TowerArray {
    TowerBase** towers;
    uint32_t capacity;
    uint32_t length;
} TowerArray;

/**
 * @brief Initializes the sprites for the different types of turrets.
 */
void initializeDifferentTowerSprites();

/**
 * @brief Initializes a new tower.
 *
 * @param x The initial x position of the tower.
 * @param y The initial y position of the tower.
 * @return A pointer to the newly initialized TowerBase.
 */
TowerBase* initializeTower(int16_t x, int16_t y);

/**
 * @brief Destroys a tower and frees associated memory.
 *
 * @param tower A pointer to the TowerBase to be destroyed.
 */
void destroyTower(TowerBase* tower);

/**
 * @brief Initializes a new array of TowerBases.
 *
 * @param capacity The initial capacity of the TowerArray.
 * @return A new TowerArray.
 */
TowerArray newTowerArray(uint32_t capacity);

/**
 * @brief Adds a TowerBase to a TowerArray.
 *
 * @param array The TowerArray to add the TowerBase to.
 * @param tower The TowerBase to add.
 */
void pushTowerArray(TowerArray* array, TowerBase* tower);

/**
 * @brief Retrieves a TowerBase from a TowerArray.
 *
 * @param array The TowerArray to retrieve the TowerBase from.
 * @param index The index of the TowerBase to retrieve.
 * @return The TowerBase at the specified index.
 */
TowerBase* getTowerArray(TowerArray* array, uint32_t index);

/**
 * @brief Removes a TowerBase from a TowerArray.
 *
 * @param array The TowerArray to remove the TowerBase from.
 * @param index The index of the TowerBase to remove.
 */
void removeTowerArray(TowerArray* array, uint32_t index);

/**
 * @brief Destroys a TowerArray and frees associated memory.
 *
 * @param array The TowerArray to destroy.
 */
void destroyTurretArray(TowerArray* array);


void setTowerHovered(TowerBase* tower, bool hovered);

/**
 * @brief Mounts a turret on a tower.
 *
 * @param tower A pointer to the TowerBase to be updated.
 * @param type The type of turret to be mounted.
 */
void mountTurret(TowerBase* tower, TurretType type);

/**
 * @brief Unmounts a turret from a tower.
 *
 * @param tower A pointer to the TowerBase to be updated.
 */
void unmountTurret(TowerBase* tower);

/**
 * @brief Hide the sprites of all the towers in a TowerArray.
 *
 * @param array A pointer to the TowerArray containing the tower to be updated.
 */
void hideTowers(TowerArray* array);

/**
 * @brief Show the sprites of all the towers in a TowerArray.
 *
 * @param array A pointer to the TowerArray containing the tower to be updated.
 */
void showTowers(TowerArray* array);

/**
 * @brief Rotate the turrets of all the towers in a TowerArray towards their targets.
 *
 * @param tower A pointer to the TowerBase.
 * @param target A pointer to the EnemyArray containing the enemies to target.
 */
void rotateTowersTowardsTarget(TowerArray* array, EnemyArray* target);

