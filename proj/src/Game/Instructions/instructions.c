#include <lcom/lcf.h>
#include <stdint.h>

#include "instructions.h"
#include "../Menu/menu.h"

#include "../../Devices/device_controller.h"
#include "../DataStructure/button.h"

#include "../../ImageAssets/Button.xpm"
#include "../../ImageAssets/InstructionBackground.xpm"

#include "../gamestates.h"

extern GameState state;

extern ScreenInfo screen;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;

extern bool playing;

static bool pressed_inst_button = false;
static int8_t inst_current_selection = -1;

GameObject* instructions_background;

ButtonArray instButtons;

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


static void checkInstructionHovered(ButtonArray* array) {

  for (int32_t i = 0; i < (int32_t)array->length; i++) {

    Button* button = getButtonArray(array, i);
    GameObject* buttonObject = button->button;

    int16_t mouse_x = mouse_device->mouse->x;
    int16_t mouse_y = mouse_device->mouse->y;
    int16_t leftMostBound = button->x + button->origin_offset_x;
    int16_t rightMostBound = button->x + button->origin_offset_x + button->button->sprite->width;
    int16_t upMostBound = button->y + button->origin_offset_y;
    int16_t downMostBound = button->y + button->origin_offset_y + button->button->sprite->height;

    if (!pressed_inst_button) {
      if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
          mouse_y > upMostBound && mouse_y < downMostBound && last_pressed_was_mouse) {

        if (mouse_device->left_button_is_pressed) {
          pressed_inst_button = true;
        }

        updateGameObjectSprite(buttonObject, button->hovering);
        inst_current_selection = i;

      } else if (!last_pressed_was_mouse && inst_current_selection == i) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
      }
    }
  }

  if (pressed_inst_button) {
    switch (inst_current_selection)
    {
    case -1:
      break;
  
    case 0:
      state = MAIN_MENU;
      exitInstructions();
      enterMenu();
      break;

    default:
      break;
    }
  }
}

void initializeInstructions() {
  instButtons = newButtonArray(20);

  instructions_background = create_gameobject((xpm_map_t)InstructionBackground, 0, 0, 0, 0, 0, true, false);

  // Go back Button
  pushButtonArray(&instButtons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2-1, screen.yres/2+350, 1, false, false));
}

void enterInstructions() {
  resetDevicesChangingScreens();
  showButtons(&instButtons);
  instructions_background->sprite->is_visible = true;
}

void updateInstructions() {
  checkInstructionHovered(&instButtons);
  checkInstructionsKeyboardInput(&keyboard_device->keyPresses);
}

void exitInstructions() {
  pressed_inst_button = false;
  inst_current_selection = -1;
  hideButtons(&instButtons);
  instructions_background->sprite->is_visible = false;
}

void destroyInstructions() {
  destroyButtonArray(&instButtons);
  destroy_gameobject(instructions_background);
}
