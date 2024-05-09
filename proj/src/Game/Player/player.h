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
  int16_t x, y;
  int16_t origin_offset_x, origin_offset_y;
  int16_t hit_points;
} Player;

Player* initializePlayer(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp);
void destroyPlayer(Player* player);
void updatePlayerPosition(Player* player, int8_t x, int8_t y);
void updatePlayerSpriteBasedOnPosition(Player* player, int8_t x, int8_t y);
void setAllSpritesInvisible(Player* player);
