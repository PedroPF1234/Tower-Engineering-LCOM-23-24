#pragma once


#include "../../../Devices/device_controller.h"


typedef struct PlayerBase {
  GameObject* baseObject;
  GameObject* health_bar;
  int16_t hit_points;
  int16_t max_hit_points;
} PlayerBase;

PlayerBase* initializePlayerBase(int16_t x, int16_t y, int16_t hp);
void destroyPlayerBase(PlayerBase* playerbase);
void updatePlayerBaseHealthBar(PlayerBase* playerbase);
