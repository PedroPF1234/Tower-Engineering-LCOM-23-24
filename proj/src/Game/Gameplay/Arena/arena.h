#pragma once

#include "../Tower/towers.h"
#include "../Shop/shop.h"
#include "../PlayerBase/playerbase.h"

#include "../../../Devices/device_controller.h"

/**
 * @brief Struct that represents an Arena
 * 
 * Struct that contains the background sprite, the decorations, the towers, the shop and the player base.
 * 
 * @param background The background sprite of the arena.
 * @param decorations An array of AnimatedGameObject pointers that represent the animated decorations on the arena.
 * @param towers The array of Tower objects that are present in the arena.
 * @param shop The shop object that is present in the arena.
 * @param base The player base object that is present in the arena.
 * @param num_targets The number of target coordinates that the enemies will follow.
 * @param num_decorations The number of decorations in the arena (Although variable, it is always 3).
 * @param target_coordinates The array of the coordinates that the enemies will follow.
 * @param spawn_x The x coordinate of the spawn point of the enemies.
 * @param spawn_y The y coordinate of the spawn point of the enemies.
*/
typedef struct Arena {
  Sprite* background;
  AnimatedGameObject** decorations;
  TowerArray towers;
  Shop shop;
  PlayerBase base;
  uint8_t num_targets;
  uint8_t num_decorations;
  int16_t* target_coordinates;
  int16_t spawn_x, spawn_y;
} Arena;

/**
 * @brief Initializes the arenas.
 * 
 * Initializes the arenas by reading the information from the files and creating the Arena objects.
 * 
 * @return An array of Arena objects.
*/
Arena* initializeArenas();

/**
 * @brief Destroys the arenas and frees the associated memory.
 * 
 * @param arena The array of Arena objects to be destroyed.
*/
void destroyArenas(Arena* arena);

/**
 * @brief Hides the arena by making the associated sprites invisible.
 * 
 * @param arena A pointer to the Arena to be hidden.
*/
void hideArena(Arena* arena);

/**
 * @brief Shows the arena by making the associated sprites visible.
 * 
 * @param arena A pointer to the Arena to be shown.
*/
void showArena(Arena* arena);
