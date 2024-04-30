#include <lcom/lcf.h>
#include <stdint.h>

#include "Menu/menu.h"

#include "../ImageAssets/Button.xpm"

typedef enum gameState {
  MAIN_MENU,
  GAME,
  GAME_OVER,
  PAUSE
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

static uint8_t menu_current_selection = 0;

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
        keyboard_device->escape_key_pressed = true;
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
}

int game_main_loop() {

  switch (state)
  {
  case MAIN_MENU:
    if (!game_booted) {

      screen = getScreenInfo();

      // Initialize Button Objects of Menu
      quitButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-1, -50, -25, 1, true);

      playButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-101, -50, -25, 1, true);
      //

      // Insert all Menu Buttons to the MenuNodesList
      insertAtEndMenu(&menuObjects, quitButton);
      insertAtEndMenu(&menuObjects, playButton);
      //

      game_booted = true;
    }

    // Checks which button is being hovered by the mouse
    checkMenuHovered(&menuObjects);

    if (keyboard_device->escape_key_pressed) {
      printf("Exiting game...\n");
      deleteListMenu(&menuObjects);
      return 1;
    }
    break;
  
  default:
    return 1;
  }

  return 0;

}
