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

static gameState state = MAIN_MENU;

static bool game_booted = false;

static ScreenInfo screen;

MenuNode* menuObjects = NULL;

// Objects of Main Menu
Button* quitButton;
Button* playButton;
GameObject* background;
//

static uint8_t menu_current_selection = 0;
static int8_t selected_button = -1;

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

    if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
        mouse_y > upMostBound   && mouse_y < downMostBound) {

      if (mouse_device->left_button_is_pressed) {
        selected_button = index;
      }

      normal->sprite->is_visible = false;
      hovering->sprite->is_visible = true;
      menu_current_selection = index;

    } else {
      normal->sprite->is_visible = true;
      hovering->sprite->is_visible = false;
    }

    current_button = current_button->next;
    index++;
  }

  switch (selected_button)
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

    // Checks which button is being hovered by the mouse
    checkMenuHovered(&menuObjects);

    if (keyboard_device->escape_key_pressed) {
      state = QUIT;
    }
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
