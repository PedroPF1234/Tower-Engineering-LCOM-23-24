#pragma once

#include "../../../Devices/device_controller.h"

SpriteArray digits;

typedef struct Money {
  GameObject* coin;
  SpriteArray moneyDigits;
  GameObjectArray moneyDigitsGameObjects;
  int32_t money_amount;
} Money;

typedef struct MoneyArray {
  Money** money;
  uint32_t length;
  uint32_t capacity;
} MoneyArray;

Money* initializeMoney(int32_t money_amount, int8_t type);
int countDigits(int num);
int* getDigits(int num, int* size);
void associateSprites(Money* money, int money_amount, int size, int8_t type);
void destroyMoney(Money* money);
void createMoneyDigitsGameObjects(Money* money);
void updateGameObjectSprites(Money* money, uint8_t type, int16_t x, int16_t y);

MoneyArray newMoneyArray(uint32_t capacity);
void insertMoneyArray(MoneyArray* moneyArray, Money* money);
void removeMoneyArray(MoneyArray* moneyArray, Money* money);
Money* getMoneyArray(MoneyArray *moneyArray, uint32_t index);
void destroyMoneyArray(MoneyArray *moneyArray);
void hideMoney(Money* money);
void showMoney(Money* money);
void hideMoneyArray(MoneyArray* moneyArray);
void showMoneyArray(MoneyArray* moneyArray);

