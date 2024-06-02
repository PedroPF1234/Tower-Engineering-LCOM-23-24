#pragma once

#include "../../../Devices/device_controller.h"

#include "../Money/money.h"


/**
 * @brief Represents the shop.
 */
typedef struct Shop {
  GameObject* shopObject; /*The game object associated with the shop*/
  GameObject* shopButton; /*The game object associated with the shop's E hint button*/
  MoneyArray prices; /*The prices of the towers/turrets in the shop*/
} Shop;

/**
 * @brief Initializes a new shop.
 *
 * @param x The initial x position of the shop.
 * @param y The initial y position of the shop.
 * @return A pointer to the newly initialized Shop.
 */
Shop* initializeShop(int16_t x, int16_t y);

/**
 * @brief Destroys all the Game Objects associated with a Shop and frees their associated memory.
 *
 * @param shop A pointer to the Shop to be destroyed.
 */
void destroyShop(Shop* shop);
