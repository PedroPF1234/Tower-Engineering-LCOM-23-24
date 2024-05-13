#pragma once

#include "../../Devices/device_controller.h"

typedef struct Player {
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
} Player;

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

void initializeGame();
void enterGame(bool multi);
void updateGame();
void exitGame();
void destroyGame();

//Temp
void testRotate();
//
