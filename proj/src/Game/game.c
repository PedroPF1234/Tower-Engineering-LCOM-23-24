#include <lcom/lcf.h>
#include <stdint.h>

#include "Menu/menu.h"

#include "../ImageAssets/Button.xpm"
#include "../ImageAssets/Background.xpm"

typedef enum gameState {
  MAIN_MENU,
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

static ScreenInfo screen;

MenuNode* menuObjects = NULL;

// Objects of Main Menu
Button* quitButton;
Button* playButton;
GameObject* background;
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
      state = QUIT;
    default:
      break;
    }
  }
}

static void checkMenuKeyboardInput(KeyPresses** head) {

  KeyPresses* current = *head;

  while (current != NULL) {
    printf("Key: %x\n", current->key);
    printf("Special: %d\n", current->special);
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
      printf("Freeing key %x and breaking\n", current->key);
      free(current);
      break;
    } else {
      printf("Freeing key %x and moving forward\n", current->key);
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

      quitButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-1, -50, -25, 1, true);
      //

      // Insert all Menu Buttons to the MenuNodesList
      insertAtEndMenu(&menuObjects, playButton);
      insertAtEndMenu(&menuObjects, quitButton);
      //

      game_booted = true;
    }

    // Checks which button is being hovered by the mouse or selected by the keyboard
    checkMenuKeyboardInput(&keyboard_device->keyPresses);
    checkMenuHovered(&menuObjects);

    break;
  
  case GAME:
    printf("Exiting game... through game.\n");
    deleteListMenu(&menuObjects);
    destroy_gameobject(background);
    return 1;

  case QUIT:
    printf("Exiting game... through quit.\n");
    deleteListMenu(&menuObjects);
    destroy_gameobject(background);
    return 1;
  default:
    return 1;
  }

  return 0;

}
