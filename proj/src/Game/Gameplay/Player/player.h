#pragma once

#include "../../../Devices/device_controller.h"

typedef enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT
} Direction;

typedef struct Player {
  AnimatedGameObject* player;
  AnimatedSpriteArray sprites;
  float x, y;
  float speed[2];
  int16_t origin_offset_x, origin_offset_y;
  int16_t hit_points;
  int16_t max_hit_points;
} Player;

Player* initializePlayer(float x, float y, int16_t ox, int16_t oy, int16_t hp);
void destroyPlayer(Player* player);
void updatePlayerPosition(Player* player);
void updatePlayerSpriteBasedOnPosition(Player* player);
