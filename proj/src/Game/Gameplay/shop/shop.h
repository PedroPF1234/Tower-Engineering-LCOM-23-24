#pragma once

#include "../../../Devices/device_controller.h"

#include "../Money/money.h"

typedef struct Shop {
  GameObject* shopObject;
  GameObject* shopButton;
  MoneyArray prices;
} Shop;

Shop* initializeShop(int16_t x, int16_t y);
void destroyShop(Shop* shop);
