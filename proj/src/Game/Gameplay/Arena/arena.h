#pragma once

#include "../Tower/towers.h"
#include "../Shop/shop.h"
#include "../PlayerBase/playerbase.h"

#include "../../../Devices/device_controller.h"

typedef struct Arena {
  Sprite* background;
  AnimatedGameObject** decorations;
  TowerArray towers;
  Shop shop;
  PlayerBase base;
  uint8_t num_targets;
  uint8_t num_decorations;
  int16_t* target_coordinates;
  int16_t spawn_x, spawn_y;
} Arena;

Arena* initializeArenas();
void destroyArenas(Arena* arena);
void hideArena(Arena* arena);
void showArena(Arena* arena);
