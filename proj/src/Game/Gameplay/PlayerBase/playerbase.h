#pragma once


#include "../../../Devices/device_controller.h"
#include "../Money/money.h"


typedef struct PlayerBase {
  GameObject* baseObject;
  GameObject* health_bar;
  GameObject* baseButton;
  int16_t hit_points;
  int16_t max_hit_points;
  Money* price;
} PlayerBase;

PlayerBase* initializePlayerBase(int16_t x, int16_t y, int16_t hp);
void destroyPlayerBase(PlayerBase* playerbase);
void updatePlayerBaseHealthBar(PlayerBase* playerbase);
