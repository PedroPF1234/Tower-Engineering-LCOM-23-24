#pragma once

/**
 * @brief Represents the different states the game can be in.
 */
typedef enum GameState {
  MAIN_MENU,
  SELECT_GAME,
  INSTRUCTIONS,
  GAME,
  BASE_MENU,
  TOWER_MENU,
  SHOP_MENU,
  GAME_OVER,
  PAUSE,
  QUIT
} GameState;
