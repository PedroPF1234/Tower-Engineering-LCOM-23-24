#pragma once

#include "../../../Devices/device_controller.h"

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
  float speed[2];
  int16_t origin_offset_x, origin_offset_y;
  int16_t hit_points;
} Player;

Player* initializePlayer(float x, float y, int16_t ox, int16_t oy, int16_t hp);
void destroyPlayer(Player* player);
void updatePlayersPositions(Player* player);
void updatePlayersSpritesBasedOnPosition(Player* player);