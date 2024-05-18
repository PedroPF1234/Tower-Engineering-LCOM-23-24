#include <lcom/lcf.h>
#include <stdint.h>

#include "menu.h"

#include "../Gameplay/gameplay.h"
#include "../Instructions/instructions.h"

#include "../gamestates.h"
#include "../DataStructure/button.h"

#include "../../ImageAssets/Button.xpm"
#include "../../ImageAssets/Background.xpm"

extern GameState state;

extern ScreenInfo screen;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;

extern bool playing;

static bool pressed_menu_button = false;
static int8_t menu_current_selection = -1;

GameObject* background;

ButtonArray menuButtons;

static void checkMenuHovered(ButtonArray* array) {

  for (int32_t i = 0; i < (int32_t)array->length; i++) {

    Button* button = getButtonArray(array, i);
    GameObject* buttonObject = button->button;

    int16_t mouse_x = mouse_device->mouse->x;
    int16_t mouse_y = mouse_device->mouse->y;
    int16_t leftMostBound = button->x + button->origin_offset_x;
    int16_t rightMostBound = button->x + button->origin_offset_x + button->button->sprite->width;
    int16_t upMostBound = button->y + button->origin_offset_y;
    int16_t downMostBound = button->y + button->origin_offset_y + button->button->sprite->height;

    if (!pressed_menu_button) {
      if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
          mouse_y > upMostBound && mouse_y < downMostBound && last_pressed_was_mouse) {

        if (mouse_device->left_button_is_pressed) {
          pressed_menu_button = true;
        }

        updateGameObjectSprite(buttonObject, button->hovering);
        menu_current_selection = i;

      } else if (!last_pressed_was_mouse && menu_current_selection == i) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
      }
    }
  }

  if (pressed_menu_button) {
    switch (menu_current_selection)
    {
    case -1:
      break;
    
    case 0:
      state = GAME;
      exitMenu();
      enterGame(false, 0);
      break;

    case 1:
      state = INSTRUCTIONS;
      exitMenu();
      enterInstructions();
      break;

    case 2:
      state = QUIT;
      break;

    default:
      break;
    }
  }
}

static void checkMenuKeyboardInput(KeyPresses** head) {

  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      case DOWN_ARROW_MAKE:
        menu_current_selection++;
        if (menu_current_selection > 2) menu_current_selection = 0;
        break;

      case UP_ARROW_MAKE:
        menu_current_selection--;
        if (menu_current_selection < 0) menu_current_selection = 2;
        break;

      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        state = QUIT;
        break;

      case ENTER_MAKE:
        pressed_menu_button = true;
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

void initializeMenu() {
  menuButtons = newButtonArray(20);
  background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);

  // Play Button
  pushButtonArray(&menuButtons, initializeButton((xpm_map_t)PlayButtonHovered, (xpm_map_t)PlayButton, screen.xres/2-1, screen.yres/2-101, -224, -25, 1, true));

  // Instructions Button
  pushButtonArray(&menuButtons, initializeButton((xpm_map_t)InstructionsButtonHovered, (xpm_map_t)InstructionsButton, screen.xres/2-1, screen.yres/2-1, -224, -25, 1, true));

  // Quit Button
  pushButtonArray(&menuButtons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2-1, screen.yres/2+99, -224, -25, 1, true));
}

void enterMenu() {
  resetDevicesChangingScreens();
  showButtons(&menuButtons);
  background->sprite->is_visible = true;
}

void updateMenu() {
  checkMenuHovered(&menuButtons);
  checkMenuKeyboardInput(&keyboard_device->keyPresses);
}

void exitMenu() {
  pressed_menu_button = false;
  menu_current_selection = -1;
  hideButtons(&menuButtons);
  background->sprite->is_visible = false;
}

void destroyMenu() {
  destroyButtonArray(&menuButtons);
  destroy_gameobject(background);
}
