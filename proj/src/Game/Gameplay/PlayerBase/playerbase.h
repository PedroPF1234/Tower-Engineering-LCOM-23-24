#pragma once


#include "../../../Devices/device_controller.h"
#include "../Money/money.h"

/**
 * @brief Represents the player's base.
 */
typedef struct PlayerBase {
  GameObject* baseObject; /**< The game object associated with the player's base. */
  GameObject* health_bar; /**< The game object associated with the player's base's health bar. */
  GameObject* baseButton; /**< The game object associated with the player's base's E hint button. */
  int16_t hit_points; /**< The current hit points of the player's base. */
  int16_t max_hit_points; /**< The maximum hit points of the player's base. */
  Money* price; /**< The price of the weapon in a player's base. */
} PlayerBase;

/**
 * @brief Initializes a new player base.
 *
 * @param x The initial x position of the player base.
 * @param y The initial y position of the player base.
 * @param hp The initial hit points of the player base.
 * @return A pointer to the newly initialized PlayerBase.
 */
PlayerBase* initializePlayerBase(int16_t x, int16_t y, int16_t hp);

/**
 * @brief Destroys all the Game Objects associated with a Player Base and frees their associated memory.
 *
 * @param playerbase A pointer to the PlayerBase to be destroyed.
 */
void destroyPlayerBase(PlayerBase* playerbase);

/**
 * @brief Updates the displayed health of the health bar of a player base.
 *
 * @param playerbase A pointer to the PlayerBase whose health bar is to be updated.
 */
void updatePlayerBaseHealthBar(PlayerBase* playerbase);
