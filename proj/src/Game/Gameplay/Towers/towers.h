#pragma once

#include "../../../Devices/device_controller.h"

typedef struct TowerBase {
  GameObject* base;
  GameObject* turret;
  Sprite* baseNormal;
  Sprite* baseHovered;
  Sprite* turretNormal;
  int16_t x, y;
  int16_t origin_offset_x, origin_offset_y;
  int16_t hit_points;
} TowerBase;

typedef struct TowerNode {
    TowerBase* tower;
    struct TowerNode* next;
} TowerNode;

TowerBase* initializeTower(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp);
void addTowerToList(TowerNode** head, TowerBase* tower);
void deleteListGame(TowerNode** head);
void destroyTower(TowerBase* tower);
