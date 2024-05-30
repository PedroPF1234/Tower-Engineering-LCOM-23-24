#pragma once

#include "../../../Devices/device_controller.h"

typedef struct Shop {
  GameObject* shop;
} Shop;

Shop* initializeShop(int16_t x, int16_t y);
void destroyShop(Shop* shop);
