#pragma once

#include "../../../Devices/device_controller.h"

typedef struct TowerBase {
  GameObject* base;
  GameObject* turret;
  Sprite* baseNormal;
  Sprite* baseHovered;
  int16_t x, y;
  int16_t origin_offset_x, origin_offset_y;
  int16_t hit_points;
} TowerBase;

typedef struct TowerArray {
    TowerBase* towers;
    uint32_t capacity;
    uint32_t length;
} TowerArray;

TowerBase* initializeTower(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp);
TowerArray newTowerArray(uint32_t capacity);
void pushTowerArray(TowerArray* array, TowerBase* tower);
TowerBase* getTowerArray(TowerArray* array, uint32_t index);
void removeTowerArray(TowerArray* array, uint32_t index);
void emptyArray(TowerArray* array);
void destroyTower(TowerBase* tower);
void setTowerHovered(TowerBase* tower, bool hovered);
