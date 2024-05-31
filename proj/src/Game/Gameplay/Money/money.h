#pragma once

#include "../../../Devices/device_controller.h"

SpriteArray digits;

typedef struct Money {
  GameObject* coin;
  SpriteArray moneyDigits;
  GameObjectArray moneyDigitsGameObjects;
  int32_t money_amount;
} Money;

Money* initializeMoney();
int countDigits(int num);
int* getDigits(int num, int* size);
void associateSprites(Money* money, int money_amount, int size, int8_t type);
void destroyMoney(Money* money);
void createMoneyDigitsGameObjects(Money* money);
void updateGameObjectSprites(Money* money, uint8_t type);
