#include <lcom/lcf.h>
#include <stdint.h>

#include "../Devices/device_controller.h"

#include "../ImageAssets/Button.xpm"
#include "../ImageAssets/ButtonPressed.xpm"

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



// Objects of Main Menu
GameObject* button;
GameObject* button_pressed;

//static uint8_t menu_current_selection = 0;

int game_main_loop() {

  switch (state)
  {
  case MAIN_MENU:
    if (!game_booted) {
      button = create_gameobject((xpm_map_t) Button, 640, 512, -50, -25, 1, true, true);
      button_pressed = create_gameobject((xpm_map_t) ButtonPressed, 640, 512, -50, -25, 1, true, false);
      game_booted = true;
    }

    if (mouse_device->mouse->x > button->x + button->origin_offset_x &&
        mouse_device->mouse->x < button->x + button->origin_offset_x + button->sprite->width &&
        mouse_device->mouse->y > button->y + button->origin_offset_y &&
        mouse_device->mouse->y < button->y + button->origin_offset_y + button->sprite->height) {

      if (mouse_device->left_button_is_pressed) {
        keyboard_device->escape_key_pressed = true;
      }

      button->sprite->is_visible = false;
      button_pressed->sprite->is_visible = true;

    } else {
      button->sprite->is_visible = true;
      button_pressed->sprite->is_visible = false;
    }

    if (keyboard_device->escape_key_pressed) {
      printf("Exiting game...\n");
      destroy_gameobject(button);
      destroy_gameobject(button_pressed);
      return 1;
    }
    break;
  
  default:
    return 1;
  }

  return 0;

}
