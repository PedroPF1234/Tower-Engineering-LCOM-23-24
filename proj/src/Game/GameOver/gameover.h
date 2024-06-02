#pragma once

#include "../../Devices/device_controller.h"

/**
 * @brief Represents the game over screen.
 */


/**
 * @brief Initializes the game over screen.
 */
void initializeGameOver(); 

/**
 * @brief Enters the game over screen.
 * 
 * @param arena_ The arena where the game over screen will be displayed.
 */
void enterGameOver(int8_t arena_); 

/**
 * @brief Updates the game over screen.
 */
void updateGameOver(); 

/**
 * @brief Exits the game over screen.
 */
void exitGameOver(); 

/**
 * @brief Destroys the game over screen's associated GameObjects and Buttons, freeing their memory.
 */
void destroyGameOver();
