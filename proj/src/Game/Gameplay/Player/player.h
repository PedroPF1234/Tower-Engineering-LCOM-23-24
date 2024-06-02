#pragma once

#include "../Arena/arena.h"

#include "../../../Devices/device_controller.h"

/**
 * @brief Enum that represents the direction of the player.
 * 
 * Enum that represents the direction of the player. The player can move in 8 directions, or be idle in one of the 4 cardinal directions.
*/
typedef enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT,
  UP_IDLE,
  DOWN_IDLE,
  LEFT_IDLE,
  RIGHT_IDLE
} Direction;

/**
 * @brief Struct that represents the player.
 * 
 * Struct that contains the player game object, the sprites, the current direction, the position, the speed, the origin offset, the hit points and the weapon status.
 * 
 * @param player The player game object.
 * @param sprites The array of AnimatedSprites that represent the player walking in different directions.
 * @param current_direction The current direction of the player.
 * @param x The x position of the player.
 * @param y The y position of the player.
 * @param speed The array representing speed of the player in the x and y directions.
 * @param origin_offset_x The offset of the origin of the player sprite in the x direction.
 * @param origin_offset_y The offset of the origin of the player sprite in the y direction.
 * @param hit_points The current hit points of the player.
 * @param max_hit_points The maximum hit points of the player.
 * @param hasWeapon A boolean that represents if the player has a weapon.
*/
typedef struct Player {
  AnimatedGameObject* player;
  AnimatedSpriteArray sprites;
  Direction current_direction;
  float x, y;
  float speed[2];
  int16_t origin_offset_x, origin_offset_y;
  int16_t hit_points;
  int16_t max_hit_points;
  bool hasWeapon;
                
} Player;

/**
 * @brief Initializes a new player.
 * 
 * Initializes a new player by creating the player game object, the sprites, setting the initial position, the origin offset, the hit points and the weapon status.
 * 
 * @param x The initial x position of the player.
 * @param y The initial y position of the player.
 * @param ox The offset of the origin of the player sprite in the x direction.
 * @param oy The offset of the origin of the player sprite in the y direction.
 * @param hp The initial hit points of the player.
 * @return A pointer to the newly initialized Player object.
*/
Player* initializePlayer(float x, float y, int16_t ox, int16_t oy, int16_t hp);

/**
 * @brief Destroys a player and frees associated memory.
 * 
 * @param player A pointer to the Player to be destroyed.
*/
void destroyPlayer(Player* player);

/**
 * @brief Updates the position of the player based on the input and the current arena, checking for collisions with the arena's objects.
 * 
 * @param player A pointer to the Player to be updated.
 * @param arena The current arena.
*/
void updatePlayerPosition(Player* player, Arena arena);

/**
 * @brief Updates the player sprite based on the input and current position of the player.
 * 
 * @param player A pointer to the Player to be updated.
*/
void updatePlayerSpriteBasedOnPosition(Player* player);
