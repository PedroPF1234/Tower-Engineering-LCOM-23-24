#include "../../../GameObjects/gameobject.h"
#include "../../Gameplay/Player/player.h"

/**
 * @brief Struct that represents a weapon
 * 
 * Contains the left and right weapon game objects. The weapon is separated into two game objects since the sprite representing it had to be mirrored, depending on the direction the mouse is relative to the player.
 *
 * @param weaponL The weapon game object with the sprite facing left.
 * @param weaponR The weapon game object with the sprite facing right.
*/
typedef struct Weapon
{
  GameObject* weaponL;
  GameObject* weaponR;
} Weapon;

/**
 * @brief Initializes a new weapon.
 * 
 * @param x The initial x position of the weapon.
 * @param y The initial y position of the weapon.
 * @return A pointer to the newly initialized Weapon object.
*/
Weapon* initializeWeapon(int16_t x, int16_t y);

/**
 * @brief Destroys a weapon and frees associated memory.
 * 
 * @param weapon A pointer to the Weapon to be destroyed.
*/
void destroyWeapon(Weapon* weapon);

/**
 * @brief Updates the position/angle of the weapon based on the player's and the mouse's positions.
 * 
 * @param weapon A pointer to the Weapon to be updated.
 * @param player A pointer to the Player object.
 * @param mouse_x The x position of the mouse.
 * @param mouse_y The y position of the mouse.
*/
void updateWeapon(Weapon* weapon, Player* player, int16_t mouse_x, int16_t mouse_y);

/**
 * @brief Shows the weapon by making the associated sprite visible.
 * 
 * @param weapon A pointer to the Weapon to be shown.
*/
void showWeapon(Weapon* weapon);

/**
 * @brief Hides the weapon by making the associated sprite invisible.
 * 
 * @param weapon A pointer to the Weapon to be hidden.
*/
void hideWeapon(Weapon* weapon);
