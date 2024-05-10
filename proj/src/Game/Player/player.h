#pragma once

#include "../../Devices/device_controller.h"

typedef struct Player {
  GameObject* up;
  GameObject* down;
  GameObject* left;
  GameObject* right;
  GameObject* up_left;
  GameObject* up_right;
  GameObject* down_left;
  GameObject* down_right;
  GameObject* stationary;
  float x, y;
  int16_t origin_offset_x, origin_offset_y;
  int16_t hit_points;
} Player;

void initializeGame();
void enterGame(bool multi);
void updateGame();
void exitGame();
void destroyGame();
