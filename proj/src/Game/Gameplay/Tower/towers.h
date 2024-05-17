#pragma once

#include "../../../Devices/device_controller.h"

#include "../Enemy/enemy.h"

typedef enum TowerTargetting {
  FIRST,
  LAST,
  CLOSEST
} TowerTargetting;

typedef enum TurretType {
  CROSSBOW,
  CANNON,
  LASER
} TurretType;

typedef struct TowerBase {
  GameObject* base;
  GameObject* turret;
  Sprite* baseNormal;
  Sprite* baseHovered;
  Sprite* turretSprite;
  int16_t x, y;
  int16_t origin_offset_x, origin_offset_y;
  int16_t hit_points;
  uint16_t range;
  uint16_t turret_radius;
  uint16_t damage;
  TowerTargetting targetting;
} TowerBase;

typedef struct TowerArray {
    TowerBase** towers;
    uint32_t capacity;
    uint32_t length;
} TowerArray;

void initializeDifferentTowerSprites();
TowerBase* initializeTower(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp);
void destroyTower(TowerBase* tower);

TowerArray newTowerArray(uint32_t capacity);
void pushTowerArray(TowerArray* array, TowerBase* tower);
TowerBase* getTowerArray(TowerArray* array, uint32_t index);
void removeTowerArray(TowerArray* array, uint32_t index);
void destroyTurretArray(TowerArray* array);


void setTowerHovered(TowerBase* tower, bool hovered);
void mountTurret(TowerBase* tower, TurretType type);
void hideTowers(TowerArray* array);
void showTowers(TowerArray* array);
void rotateTowersTowardsTarget(TowerArray* array, EnemyArray* target);

