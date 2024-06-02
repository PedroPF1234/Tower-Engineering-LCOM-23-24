#pragma once

/**
 * @brief Includes all things related to the gameplay of the game.
 */


/**
 * @brief Initializes the gameplay.
 */
void initializeGameplay();

/**
 * @brief Enters the game.
 * 
 * @param multi Whether the game is multiplayer or not.
 * @param arena The arena where the game will be played.
 */
void enterGame(bool multi, uint8_t arena);

/**
 * @brief Updates all the game objects and buttons.. etc in the game.
 */
void updateGame();

/**
 * @brief Exits the game.
 */
void exitGame();

/**
 * @brief Destroys all the game objects and buttons.. etc in the game and frees their memory.
 */
void destroyGame();


