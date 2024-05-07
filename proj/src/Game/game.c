#include <lcom/lcf.h>
#include <stdint.h>

#include "Menu/menu.h"

#include "../ImageAssets/Button.xpm"
#include "../ImageAssets/Background.xpm"

typedef enum gameState {
  MAIN_MENU,
  INSTRUCTIONS,
  GAME,
  GAME_OVER,
  PAUSE,
  QUIT
} gameState;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;

static gameState state = MAIN_MENU;

static bool game_booted = false;
static bool first_time_menu = true;

static bool instructions_booted = false;
static bool first_time_instructions = true;

static ScreenInfo screen;

MenuNode* menuObjects = NULL;

// Objects of Main Menu
Button* playButton;
Button* instructionsButton;
Button* quitButton;
GameObject* background;
//

// Object for instructions
GameObject* instructions_background;
//

static bool pressed_menu_button = false;
static int8_t menu_current_selection = -1;

static void checkMenuHovered(MenuNode** head) {

  MenuNode* current_button = *head;
  uint8_t index = 0;

  while (current_button != NULL) {

    Button* button = current_button->button;
    GameObject* normal = button->no_hovering;
    GameObject* hovering = button->hovering;

    int16_t mouse_x = mouse_device->mouse->x;
    int16_t mouse_y = mouse_device->mouse->y;
    int16_t leftMostBound = button->x + button->origin_offset_x;
    int16_t rightMostBound = button->x + button->origin_offset_x + button->hovering->sprite->width;
    int16_t upMostBound = button->y + button->origin_offset_y;
    int16_t downMostBound = button->y + button->origin_offset_y + button->hovering->sprite->height;

    if (!pressed_menu_button) {
      if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
          mouse_y > upMostBound && mouse_y < downMostBound && last_pressed_was_mouse) {

        if (mouse_device->left_button_is_pressed) {
          pressed_menu_button = true;
        }

        normal->sprite->is_visible = false;
        hovering->sprite->is_visible = true;
        menu_current_selection = index;

      } else if (!last_pressed_was_mouse && menu_current_selection == index) {
        normal->sprite->is_visible = false;
        hovering->sprite->is_visible = true;
      } else {
        normal->sprite->is_visible = true;
        hovering->sprite->is_visible = false;
      }
    }

    current_button = current_button->next;
    index++;
  }

  if (pressed_menu_button) {
    switch (menu_current_selection)
    {
    case -1:
      break;
    
    case 0:
      state = GAME;
      break;

    case 1:
      state = INSTRUCTIONS;
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
        if (menu_current_selection > 1) menu_current_selection = 0;
        break;

      case UP_ARROW_MAKE:
        menu_current_selection--;
        if (menu_current_selection < 0) menu_current_selection = 1;
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

static void checkMenu(MenuNode** head, KeyPresses** keyPresses) {
  checkMenuHovered(head);
  checkMenuKeyboardInput(keyPresses);
}

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
        printf("Exiting instructions...\n");
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

int game_main_loop() {

  switch (state)
  {
  case MAIN_MENU:
    if (!game_booted) {

      screen = getScreenInfo();

      background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);

      // Initialize Button Objects of Menu
      playButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-101, -50, -25, 1, true);

      instructionsButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-1, -50, -25, 1, true);

      quitButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2+99, -50, -25, 1, true);
      //

      // Insert all Menu Buttons to the MenuNodesList
      insertAtEndMenu(&menuObjects, playButton);
      insertAtEndMenu(&menuObjects, instructionsButton);
      insertAtEndMenu(&menuObjects, quitButton);
      //

      game_booted = true;
    }

    if (instructions_booted && first_time_menu) {
      first_time_instructions = true;
      first_time_menu = false;
      pressed_menu_button = false;
      showMenuButtons(&menuObjects);
      instructions_background->sprite->is_visible = false;
      menu_current_selection = -1;
    }

    // Checks which button is being hovered by the mouse or selected by the keyboard
    checkMenu(&menuObjects, &keyboard_device->keyPresses);

    break;
  
  case GAME:
    printf("Exiting game... through game.\n");
    deleteListMenu(&menuObjects);
    destroy_gameobject(background);
    return 1;

  case INSTRUCTIONS:
    if (!instructions_booted) {

      instructions_background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);

      instructions_booted = true;

    }

    if (first_time_instructions) {
      first_time_instructions = false;
      first_time_menu = true;
      hideMenuButtons(&menuObjects);
      instructions_background->sprite->is_visible = true;
    }

    checkInstructionsKeyboardInput(&keyboard_device->keyPresses);

    break;

  case QUIT:
    printf("Exiting game... through quit.\n");
    deleteListMenu(&menuObjects);
    destroy_gameobject(background);
    destroy_gameobject(instructions_background);
    return 1;
  default:
    return 1;
  }

  return 0;

}
