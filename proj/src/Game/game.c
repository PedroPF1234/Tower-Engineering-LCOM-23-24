#include <lcom/lcf.h>
#include <stdint.h>

#include "Menu/menu.h"
#include "GameOver/gameover.h"
#include "Gameplay/gameplay.h"
#include "Instructions/instructions.h"

#include "gamestates.h"

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;

GameState state = MAIN_MENU;

// Boot flags
static bool game_booted = false;
bool playing = false;
//

int game_main_loop() {

  switch (state)
  {
  case SELECT_GAME:
  case MAIN_MENU:

    if (!game_booted) {
      initializeMenu();
      initializeGameplay();
      initializeGameOver();
      initializeInstructions();
      game_booted = true;
    }

    updateMenu();
    break;
  
  case GAME_OVER:
    updateGameOver();
    break;

  case SHOP_MENU:
  case BASE_MENU:
  case TOWER_MENU:
  case PAUSE:
  case GAME:

    updateGame();
    break;

  case INSTRUCTIONS:

    updateInstructions();
    break;

  case QUIT:
    destroyMenu();
    destroyGame();
    destroyGameOver();
    destroyInstructions();
    return 1;

  default:
    return 1;
  }

  return 0;

}
