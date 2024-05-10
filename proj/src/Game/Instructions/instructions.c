#include <lcom/lcf.h>
#include <stdint.h>

#include "instructions.h"
#include "../Menu/menu.h"

#include "../../Devices/device_controller.h"

#include "../../ImageAssets/Background.xpm"

typedef enum GameState {
  MAIN_MENU,
  INSTRUCTIONS,
  GAME,
  GAME_OVER,
  PAUSE,
  QUIT
} GameState;

extern GameState state;

extern ScreenInfo screen;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;

extern bool playing;

GameObject* instructions_background;

static void checkInstructionsKeyboardInput(KeyPresses** head) {

  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        state = MAIN_MENU;
        exitInstructions();
        enterMenu();
        break;
      
      default:
        break;
      }
    }

    if (current->next == NULL) {
      free(current);
      break;
    } else {
      KeyPresses* next = current->next;
      free(current);
      current = next;
    }
  }

  *head = NULL;
}


void initializeInstructions() {
  instructions_background = create_gameobject((xpm_map_t)Background, 0, 0, 0, 0, 0, false, false);
}

void enterInstructions() {
  instructions_background->sprite->is_visible = true;
}

void updateInstructions() {
  checkInstructionsKeyboardInput(&keyboard_device->keyPresses);
}

void exitInstructions() {
  instructions_background->sprite->is_visible = false;
}

void destroyInstructions() {
  destroy_gameobject(instructions_background);
}
