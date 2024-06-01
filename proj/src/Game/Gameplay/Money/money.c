#include <lcom/lcf.h>
#include <stdint.h>

#include "money.h"
#include "../../../ImageAssets/Money.xpm"

Money* initializeMoney() {
  Money* new_money = (Money*)malloc(sizeof(Money));

  Sprite* coin = create_sprite((xpm_map_t)Coin, 0, 0, false, false);
  
  new_money->coin = create_gameobject_from_sprite(coin, coin->width/2, coin->height/2, -(coin->width/2), -(coin->height/2), 0);
  new_money->money_amount = 500;
  new_money->moneyDigits = newSpriteArray(20);
  new_money->moneyDigitsGameObjects = newGameObjectArray(20);

  char** digitsXPM[] = {Digit0, Digit1, Digit2, Digit3, Digit4, Digit5, Digit6, Digit7, Digit8, Digit9, Digit0G, Digit1G, Digit2G, Digit3G, Digit4G, Digit5G, Digit6G, Digit7G, Digit8G, Digit9G, Digit0R, Digit1R, Digit2R, Digit3R, Digit4R, Digit5R, Digit6R, Digit7R, Digit8R, Digit9R};

  digits = newApriteFromCharArray(digitsXPM, 30);

  //type is 0 for now, introduce type changing logic
  int size = 0;
  associateSprites(new_money, new_money->money_amount, size, 0);
  createMoneyDigitsGameObjects(new_money);

  return new_money;
}

int countDigits(int num) {
    int count = 0;
    if (num == 0) {
        return 1;
    }
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count;
}

int* getDigits(int num, int* size) {
    *size = countDigits(num);
    
    int* digits = (int*)malloc(*size * sizeof(int));
    if (digits == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int i = *size - 1;
    while (i >= 0) {
        digits[i] = num % 10;
        num /= 10;
        i--;
    }

    return digits;
}

// Type can be 0 for white number, 1 for green number and 2 for red number
void associateSprites(Money* money, int money_amount, int size, int8_t type) {
  cleanSpriteArray(&money->moneyDigits);
  int* num_digits = getDigits(money_amount, &size);

  for (int i = 0; i < size; i++) {
    pushSpriteArray(&money->moneyDigits, getSpriteArray(&digits,num_digits[i]+type*10));
  }
  free(num_digits);
}

void createMoneyDigitsGameObjects(Money* money) {
  int16_t x = money->coin->x;
  int16_t y = money->coin->y;
  for (uint32_t i = 0; i < money->moneyDigits.length; i++) {
    Sprite* sprite = getSpriteArray(&money->moneyDigits, i);
    GameObject* gameObject = create_gameobject_from_sprite(sprite,x*3+i*30 , y, -(sprite->width/2), -(sprite->height/2), 0xFFF1);
    insertGameObjectArray(&money->moneyDigitsGameObjects, gameObject);
  }
}

void updateGameObjectSprites(Money* money, uint8_t type) {
  
  if(money->money_amount <= 0) money->money_amount = 0;

  int size_ = countDigits(money->money_amount);

  // New
  int* num_digits = getDigits(money->money_amount, &size_);

  uint32_t size = (uint32_t) size_;
  
  if (size > money->moneyDigitsGameObjects.length) {
    for (uint32_t i = money->moneyDigitsGameObjects.length; i < size; i++) {
      Sprite* sprite = getSpriteArray(&digits, num_digits[i]+(type*10));
      GameObject* gameObject = create_gameobject_from_sprite(sprite, money->coin->x*3+i*30, money->coin->y, -(sprite->width/2), -(sprite->height/2), 0xFFF1);
      insertGameObjectArray(&money->moneyDigitsGameObjects, gameObject);
    }
  } else if (size < money->moneyDigitsGameObjects.length) {
    for (uint32_t i = money->moneyDigitsGameObjects.length - 1; i >= size; i--) {
      GameObject* gameObject = getGameObjectArray(&money->moneyDigitsGameObjects, i);
      removeGameObjectArray(&money->moneyDigitsGameObjects, gameObject);
      destroy_gameobject_safe_sprite(gameObject);
    }
  } 

  // New version
  for (uint32_t i = 0; i < money->moneyDigitsGameObjects.length; i++) {
    GameObject* gameObject = getGameObjectArray(&money->moneyDigitsGameObjects, i);
    Sprite* sprite = getSpriteArray(&digits, num_digits[i]+(type*10));
    updateGameObjectSprite(gameObject, sprite);
  } 

  free(num_digits);
  
}

void destroyMoney(Money* money) {
  destroy_gameobject(money->coin);
  destroySpriteArray(&money->moneyDigits);
  destroySpriteArray(&digits);
  free(money);
}
