#include <lcom/lcf.h>
#include <stdint.h>

#include "money.h"
#include "../../../ImageAssets/Money.xpm"

Money* initializeMoney(int32_t money_amount, int8_t type) {
  Money* new_money = (Money*)malloc(sizeof(Money));

  Sprite* coin = create_sprite((xpm_map_t)Coin, 0, 0, false, false);
  
  new_money->coin = create_gameobject_from_sprite(coin, coin->width/2, coin->height/2, -(coin->width/2), -(coin->height/2), 0);
  new_money->money_amount = money_amount;
  new_money->moneyDigits = newSpriteArray(20);
  new_money->moneyDigitsGameObjects = newGameObjectArray(20);

  char** digitsXPM[] = {Digit0, Digit1, Digit2, Digit3, Digit4, Digit5, Digit6, Digit7, Digit8, Digit9, Digit0G, Digit1G, Digit2G, Digit3G, Digit4G, Digit5G, Digit6G, Digit7G, Digit8G, Digit9G, Digit0R, Digit1R, Digit2R, Digit3R, Digit4R, Digit5R, Digit6R, Digit7R, Digit8R, Digit9R};

  digits = newApriteFromCharArray(digitsXPM, 30);

  int size = 0;
  associateSprites(new_money, new_money->money_amount, size, type);
  createMoneyDigitsGameObjects(new_money);

  hideGameObjects(&new_money->moneyDigitsGameObjects);

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


void updateGameObjectSprites(Money* money, uint8_t type, int16_t x, int16_t y) {

  printf("Money pointer: %p\n", money);

  if (!(x == 0 && y == 0)) {

    money->coin->x = x;
    money->coin->y = y;

    int16_t new_x = money->coin->x;
    int16_t new_y = money->coin->y;

    for (uint32_t i = 0; i < money->moneyDigits.length; i++) {
      GameObject* gameObject = getGameObjectArray(&money->moneyDigitsGameObjects, i);
      gameObject->x = new_x+money->coin->sprite->width+i*30;
      gameObject->y = new_y;
    }
  }

  if(money->money_amount <= 0) money->money_amount = 0;

  int size_ = countDigits(money->money_amount);

  // New
  int* num_digits = getDigits(money->money_amount, &size_);

  uint32_t size = (uint32_t) size_;

  printf("Array size before size comparsion: %d\n", money->moneyDigitsGameObjects.length);
  
  if (size > money->moneyDigitsGameObjects.length) {
    for (uint32_t i = money->moneyDigitsGameObjects.length; i < size; i++) {
      Sprite* sprite = getSpriteArray(&digits, num_digits[i]+(type*10));
      GameObject* gameObject = create_gameobject_from_sprite(sprite, money->coin->x*3+i*30, money->coin->y, -(sprite->width/2), -(sprite->height/2), 0xFFF1);
      insertGameObjectArray(&money->moneyDigitsGameObjects, gameObject);
    }
  } else if (size < money->moneyDigitsGameObjects.length) {
    printf("Size is smaller, should elliminate one or more digits\n");
    for (uint32_t i = money->moneyDigitsGameObjects.length - 1; i >= size; i--) {
      printf("Removing digit %d\n", i);
      printf("This should be called once\n");
      GameObject* gameObject = getGameObjectArray(&money->moneyDigitsGameObjects, i);
      printf("Got the gameobject\n");
      removeGameObjectArray(&money->moneyDigitsGameObjects, gameObject);
      printf("Removed the gameobject\n");
      destroy_gameobject_safe_sprite(gameObject);
      printf("Destroyed the gameobject\n");
    }
  } 

  printf("Array size after size comparsion: %d\n", money->moneyDigitsGameObjects.length);

  for (uint32_t i = 0; i < money->moneyDigitsGameObjects.length; i++) {
    printf("Updating digit %d\n", i);
    GameObject* gameObject = getGameObjectArray(&money->moneyDigitsGameObjects, i);
    printf("Got the gameobject\n");
    Sprite* sprite = getSpriteArray(&digits, num_digits[i]+(type*10));
    printf("Got the sprite with value %d\n", num_digits[i]);
    updateGameObjectSprite(gameObject, sprite);
    printf("Updated the sprite\n");
  } 

  free(num_digits);
  
}

void destroyMoney(Money* money) {
  destroy_gameobject(money->coin);
  destroyGameObjectArray(&money->moneyDigitsGameObjects);
  free(money);
}


MoneyArray newMoneyArray(uint32_t capacity){

  MoneyArray new_array;

  new_array.length = 0;

  if (capacity) {
    new_array.money = (Money**)malloc(sizeof(Money*) * capacity);
    new_array.capacity = capacity;
  } else {
    new_array.money = (Money**)malloc(sizeof(Money*) * 10);
    new_array.capacity = 10;
  }

  return new_array;
}

void insertMoneyArray(MoneyArray* moneyArray, Money* newMoney) {
  if (moneyArray->capacity != moneyArray->length) {
    moneyArray->money[moneyArray->length] = newMoney;
  } else {
    moneyArray->capacity = moneyArray->capacity * 2;
    Money** oldPointer = moneyArray->money;
    Money** newPointer = (Money**)malloc(moneyArray->capacity * sizeof(Money*));
    moneyArray->money = newPointer;
    for (uint32_t i = 0; i < moneyArray->length; i++) {
      newPointer[i] = oldPointer[i];
    }
    free(oldPointer);
    moneyArray->money[moneyArray->length] = newMoney;
  }

  moneyArray->length++;  
}

void removeMoneyArray(MoneyArray* moneyArray, Money* money) {
  if (moneyArray->length == 0) {
    return;
  }

  for (uint32_t i = 0; i < moneyArray->length; i++) {
    if (moneyArray->money[i] == money) {
      for (uint32_t j = i; j < moneyArray->length - 1; j++) {
        moneyArray->money[j] = moneyArray->money[j + 1];
      }
      moneyArray->money[moneyArray->length - 1] = (Money*)0;
      moneyArray->length--;
      break;
    }
  }
}

void hideMoneyArray(MoneyArray* moneyArray) {
  for (uint32_t i = 0; i < moneyArray->length; i++) {
    Money* tmp = getMoneyArray(moneyArray, i);
    for (uint32_t j = 0; j < tmp->moneyDigitsGameObjects.length; j++) {
      hideGameObject(getGameObjectArray(&tmp->moneyDigitsGameObjects, j));
    }
  }
}

void showMoneyArray(MoneyArray* moneyArray) {
  for (uint32_t i = 0; i < moneyArray->length; i++) {
    Money* tmp = getMoneyArray(moneyArray, i);
    for (uint32_t j = 0; j < tmp->moneyDigitsGameObjects.length; j++) {
      showGameObject(getGameObjectArray(&tmp->moneyDigitsGameObjects, j));
    }
  }
}

void hideMoney(Money* money) {
  for (uint32_t i = 0; i < money->moneyDigitsGameObjects.length; i++) {
    hideGameObject(getGameObjectArray(&money->moneyDigitsGameObjects, i));
  }
}

void showMoney(Money* money) {
  for (uint32_t i = 0; i < money->moneyDigitsGameObjects.length; i++) {
    showGameObject(getGameObjectArray(&money->moneyDigitsGameObjects, i));
  }
}

Money* getMoneyArray(MoneyArray *moneyArray, uint32_t index) {
  if (index < moneyArray->length) {
    return moneyArray->money[index];
  } else {
    return NULL;
  }
}

void destroyMoneyArray(MoneyArray *moneyArray) {
  printf("Destrying Money Array\n");
  uint32_t length = moneyArray->length;
  for (uint32_t i = 0; i < length; i++) {
    printf("Before destroying %d member\n", i);
    Money* money = getMoneyArray(moneyArray, 0);
    printf("Ammount saved in this member: %d\n", money->money_amount);
    removeMoneyArray(moneyArray, money);
    printf("Removed member %d from array.\n", i);
    destroyMoney(money);
    printf("Destroyed member %d from array.\n", i);
  }
}

