#pragma once

#include "../../../Devices/device_controller.h"

typedef struct Shop {
  GameObject* shopObject;
  GameObject* shopButton;
} Shop;

Shop* initializeShop(int16_t x, int16_t y);
void destroyShop(Shop* shop);
