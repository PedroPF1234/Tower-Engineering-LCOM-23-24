#include <lcom/lcf.h>
#include <stdint.h>

#include "gameover.h"

#include "../gamestates.h"
#include "../DataStructure/button.h"

#include "../../ImageAssets/Button.xpm"
#include "../../ImageAssets/Background.xpm"

#include "../Gameplay/gameplay.h"
#include "../Menu/menu.h"

extern GameState state;
extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;
extern ScreenInfo screen;

static int8_t arena;

ButtonArray gameoverButtons;
GameObject* gameoverBackground;

static bool pressed_gameover_button = false;
static int8_t gameover_current_selection = -1;

static void checkGameOverHovered(ButtonArray* array) {
  for (int32_t i = 0; i < (int32_t)array->length; i++) {

    Button* button = getButtonArray(array, i);
    GameObject* buttonObject = button->button;

    int16_t mouse_x = mouse_device->mouse->x;
    int16_t mouse_y = mouse_device->mouse->y;
    int16_t leftMostBound = button->x + button->origin_offset_x;
    int16_t rightMostBound = button->x + button->origin_offset_x + button->button->sprite->width;
    int16_t upMostBound = button->y + button->origin_offset_y;
    int16_t downMostBound = button->y + button->origin_offset_y + button->button->sprite->height;

    if (!pressed_gameover_button) {
      if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
          mouse_y > upMostBound && mouse_y < downMostBound && last_pressed_was_mouse) {

        if (mouse_device->left_button_is_pressed) {
          pressed_gameover_button = true;
        }

        updateGameObjectSprite(buttonObject, button->hovering);
        gameover_current_selection = i;

      } else if (!last_pressed_was_mouse && gameover_current_selection == i) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
      }
    }
  }

  if (pressed_gameover_button) {
    pressed_gameover_button = false;
    switch (gameover_current_selection)
    {
    case -1:
      break;
    
    case 0:
      state = GAME;
      hideButtons(&gameoverButtons);
      exitGameOver();
      enterGame(false,arena);
      break;

    case 1:
      state = MAIN_MENU;
      hideButtons(&gameoverButtons);
      enterMenu();
      break;

    default:
      break;
    }
  }
}

static void checkGameOverKeyboardInput(KeyPresses** head) {

}


void initializeGameOver(){

  gameoverButtons = newButtonArray(20);

  gameoverBackground = create_gameobject((xpm_map_t) GameOverBackground, 0, 0, 0, 0, 0, true, false);

  // Menu Buttons
  // Play Again Button
  pushButtonArray(&gameoverButtons, initializeButton((xpm_map_t)PlayAgainButtonHovered, (xpm_map_t)PlayAgainButton, screen.xres/2-1, screen.yres/2+150, 1, false, true));

  // Quit Button
  pushButtonArray(&gameoverButtons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2-1, screen.yres/2+350, 1, false, true));

  hideButtons(&gameoverButtons);

}

void enterGameOver(int8_t arena_){
  arena = arena_;
  resetDevicesChangingScreens();
  showButtons(&gameoverButtons);
  gameoverBackground->sprite->is_visible = true;
}

void updateGameOver(){
  if (state == GAME_OVER) {
    checkGameOverHovered(&gameoverButtons);
    checkGameOverKeyboardInput(&keyboard_device->keyPresses);
  } 
}

void exitGameOver(){
  pressed_gameover_button = false;
  gameover_current_selection = -1;
  hideButtons(&gameoverButtons);
  gameoverBackground->sprite->is_visible = false;
}

void destroyGameOver(){
  destroyButtonArray(&gameoverButtons);
  destroy_gameobject(gameoverBackground);
}
