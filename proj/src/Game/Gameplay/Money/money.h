#pragma once

#include "../../../Devices/device_controller.h"

SpriteArray digits;

/**
 * @brief Represents the money in the game.
 
 */
typedef struct Money {
  GameObject* coin;  /*The game object that represents the golden coin icon*/
  SpriteArray moneyDigits; /*The sprites that represent the digits of the money amount*/
  GameObjectArray moneyDigitsGameObjects; /*The game objects that represent the digits of the money amount*/
  int32_t money_amount; /*The amount of money a player has*/
} Money;


/**
 * @brief Represents an array of Money.
 */
typedef struct MoneyArray { 
  Money** money; /*Pointer to an array of Money pointers*/
  uint32_t length; /*The current number of Money in the array*/
  uint32_t capacity; /*The total capacity of the array*/
} MoneyArray;


/**
 * @brief Initializes a new Money.
 *
 * @param money_amount The initial amount of money.
 * @param type The color of the money digits. 0 for white, 1 for green and 2 for red.
 * @return A pointer to the newly initialized Money.
 */
Money* initializeMoney(int32_t money_amount, int8_t type);

/**
 * @brief Counts the digits of a certain money amount
 *
 * @param num The money amount.
 * @return The number of digits in the money amount.
 */
int countDigits(int num);

/**
 * @brief Retrieves the digits of a certain money amount
 *
 * @param num The money amount.
 * @param size The number of digits in the money amount.
 * @return An array with the digits of the money amount.
 */
int* getDigits(int num, int* size);

/**
 * @brief Associates the digit sprites with each digit of the money amount.
 *
 * @param money A pointer to the Money.
 * @param money_amount The amount of money
 * @param size The number of digits in the money amount.
 * @param type The color of the money digits. 0 for white, 1 for green and 2 for red.
 */
void associateSprites(Money* money, int money_amount, int size, int8_t type);

/**
 * @brief Destroys a Money and frees associated memory.
 *
 * @param money A pointer to the Money to be updated.
 */
void destroyMoney(Money* money);

/**
 * @brief Creates the game objects that represent the digits of the money amount using the associated sprites.
 *
 * @param money A pointer to the Money.
 */
void createMoneyDigitsGameObjects(Money* money);

/**
 * @brief Updates the Money Game Objects. Used when the money amount changes / is moved.
 *
 * @param money A pointer to the Money to be updated.
 * @param type The color of the money digits. 0 for white, 1 for green and 2 for red.
 * @param x The x position of the money.
 * @param y The y position of the money.
 */
void updateGameObjectSprites(Money* money, uint8_t type, int16_t x, int16_t y);


/**
 * @brief Initializes a new MoneyArray.
 *
 * @param capacity The initial capacity of the MoneyArray.
 * @return A new MoneyArray.
 */
MoneyArray newMoneyArray(uint32_t capacity);

/**
 * @brief Adds a Money to the MoneyArray.
 *
 * @param moneyArray A pointer to the MoneyArray.
 * @param money A pointer to the Money to be added.
 */
void insertMoneyArray(MoneyArray* moneyArray, Money* money);

/**
 * @brief Removes a Money from the MoneyArray.
 *
 * @param moneyArray A pointer to the MoneyArray.
 * @param money A pointer to the Money to be removed.
 */
void removeMoneyArray(MoneyArray* moneyArray, Money* money);

/**
 * @brief Retrieves a Money from the MoneyArray at a specified index.
 *
 * @param moneyArray A pointer to the MoneyArray.
 * @param index The index of the Money to retrieve.
 * @return A pointer to the Money at the specified index, or NULL if the index is out of bounds.
 */
Money* getMoneyArray(MoneyArray *moneyArray, uint32_t index);

/**
 * @brief Destroys a MoneyArray and frees associated memory.
 *
 * @param moneyArray A pointer to the MoneyArray to be destroyed.
 */
void destroyMoneyArray(MoneyArray *moneyArray);

/**
 * @brief Removes Money Sprites from Render Pipeline.
 *
 * @param money A pointer to the Money
 */
void hideMoney(Money* money);

/**
 * @brief Inserts Money Sprites into Render Pipeline.
 *
 * @param money A pointer to the Money
 */
void showMoney(Money* money);

/**
 * @brief Removes all the Money Sprites in MoneyArray from Render Pipeline.
 *
 * @param money A pointer to the Money Array.
 */
void hideMoneyArray(MoneyArray* moneyArray);

/**
 * @brief Inserts all the Money Sprites in MoneyArray into Render Pipeline.
 *
 * @param money A pointer to the Money Array.
 */
void showMoneyArray(MoneyArray* moneyArray);

