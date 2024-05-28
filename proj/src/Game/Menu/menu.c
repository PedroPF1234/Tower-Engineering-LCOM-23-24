#include <lcom/lcf.h>
#include <stdint.h>

#include "menu.h"

#include "../Gameplay/gameplay.h"
#include "../Instructions/instructions.h"

#include "../gamestates.h"
#include "../DataStructure/button.h"

#include "../../ImageAssets/Button.xpm"
#include "../../ImageAssets/Background.xpm"
#include "../../ImageAssets/MiniMaps.xpm"

extern GameState state;

extern ScreenInfo screen;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;

extern bool playing;

static bool pressed_menu_button = false;
static int8_t menu_current_selection = -1;
static int8_t select_game_current_selection = -1;
static int8_t select_game_current_arena = -1;

GameObject* background;
GameObject* selectGameBackground;

ButtonArray menuButtons;
ButtonArray selectGameArenaButtons;

static void checkSelectGameHovered(ButtonArray* array) {
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
        select_game_current_selection = i;

      } else if (!last_pressed_was_mouse && menu_current_selection == i) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
      }
    }
  }

  if (pressed_menu_button) {
    switch (select_game_current_selection)
    {
    case -1:
      break;

    case 0:
      select_game_current_arena = 0;
      pressed_menu_button = false;
      break;

    case 1:
      select_game_current_arena = 1;
      pressed_menu_button = false;
      break;

    case 2:
      select_game_current_arena = 2;
      pressed_menu_button = false;
      break;
    
    case 4:
      state = GAME;
      enterGame(false, select_game_current_arena);
      exitMenu();
      break;

    case 3:
      state = MAIN_MENU;
      hideButtons(&selectGameArenaButtons);
      showButtons(&menuButtons);
      selectGameBackground->sprite->is_visible = false;
      select_game_current_arena = -1;
      select_game_current_selection = -1;
      pressed_menu_button = false;
      break;

    default:
      break;
    }
  }
}

static void checkSelectGameKeyboardInput(KeyPresses** head) {
  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      case DOWN_ARROW_MAKE:
        select_game_current_selection++;
        if (select_game_current_selection > 4) select_game_current_selection = 0;
        break;

      case UP_ARROW_MAKE:
        select_game_current_selection--;
        if (select_game_current_selection < 0) select_game_current_selection = 4;
        break;

      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        state = MAIN_MENU;
        hideButtons(&selectGameArenaButtons);
        showButtons(&menuButtons);
        selectGameBackground->sprite->is_visible = false;
        select_game_current_arena = -1;
        select_game_current_selection = -1;
        pressed_menu_button = false;
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
      state = SELECT_GAME;
      menu_current_selection = -1;
      hideButtons(&menuButtons);
      showButtons(&selectGameArenaButtons);
      selectGameBackground->sprite->is_visible = true;
      pressed_menu_button = false;
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
  selectGameArenaButtons = newButtonArray(20);

  background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);

  Sprite* selectBackgound = create_sprite((xpm_map_t) SelectGameBackground, screen.xres/2, screen.yres/2, true, false);
  selectGameBackground = create_gameobject_from_sprite(selectBackgound, screen.xres/2, screen.yres/2, -(selectBackgound->width/2), -(selectBackgound->height/2), 2);
  
  // Menu Buttons

  // Aux
  Sprite* playButton = create_sprite((xpm_map_t)PlayButton, screen.xres/2-1, screen.yres/2-101, true, false);

  // Play Button
  pushButtonArray(&menuButtons, initializeButton((xpm_map_t)PlayButtonHovered, (xpm_map_t)PlayButton, screen.xres/2-1, screen.yres/2-101, -(playButton->width/2), -(playButton->height/2), 1, true, true));

  // Instructions Button
  pushButtonArray(&menuButtons, initializeButton((xpm_map_t)InstructionsButtonHovered, (xpm_map_t)InstructionsButton, screen.xres/2-1, screen.yres/2-1, -(playButton->width/2), -(playButton->height/2), 1, true, true));

  // Quit Button
  pushButtonArray(&menuButtons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2-1, screen.yres/2+99, -(playButton->width/2), -(playButton->height/2), 1, true, true));

  // Select Game Arena Buttons
  // Aux
  Sprite* arena1 = create_sprite((xpm_map_t)FirstMiniMap, screen.xres/2-1, screen.yres/2-101, true, false);

  // Arena 1
  pushButtonArray(&selectGameArenaButtons, initializeButton((xpm_map_t)FirstMiniMap, (xpm_map_t)FirstMiniMap, screen.xres/2-301, screen.yres/2-201, -(arena1->width/2), -(arena1->height/2), 3, true, true));

  // Arena 2
  pushButtonArray(&selectGameArenaButtons, initializeButton((xpm_map_t)SecondMiniMap, (xpm_map_t)SecondMiniMap, screen.xres/2-1, screen.yres/2-201, -(arena1->width/2), -(arena1->height/2), 3, true, true));

  // Arena 3
  pushButtonArray(&selectGameArenaButtons, initializeButton((xpm_map_t)ThirdMiniMap, (xpm_map_t)ThirdMiniMap, screen.xres/2+299, screen.yres/2-201, -(arena1->width/2), -(arena1->height/2), 3, true, true));

  // Back Button
  pushButtonArray(&selectGameArenaButtons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2-301, screen.yres/2+299, -(playButton->width/2), -(playButton->height/2), 3, false, true));

  // Play Button
  pushButtonArray(&selectGameArenaButtons, initializeButton((xpm_map_t)PlayButtonHovered, (xpm_map_t)PlayButton, screen.xres/2+299, screen.yres/2+299, -(playButton->width/2), -(playButton->height/2), 3, false, true));

  hideButtons(&selectGameArenaButtons);
  selectGameBackground->sprite->is_visible = false;

  destroy_sprite(playButton);
  destroy_sprite(arena1);
}

void enterMenu() {
  resetDevicesChangingScreens();
  showButtons(&menuButtons);
  hideButtons(&selectGameArenaButtons);
  background->sprite->is_visible = true;
}

void updateMenu() {
  if (state == MAIN_MENU) {
    checkMenuHovered(&menuButtons);
    checkMenuKeyboardInput(&keyboard_device->keyPresses);
  } else if (state == SELECT_GAME) {
    checkSelectGameHovered(&selectGameArenaButtons);
    checkSelectGameKeyboardInput(&keyboard_device->keyPresses);
  }
}

void exitMenu() {
  pressed_menu_button = false;
  menu_current_selection = -1;
  select_game_current_selection = -1;
  select_game_current_arena = -1;
  hideButtons(&menuButtons);
  hideButtons(&selectGameArenaButtons);
  background->sprite->is_visible = false;
  selectGameBackground->sprite->is_visible = false;
}

void destroyMenu() {
  destroyButtonArray(&menuButtons);
  destroy_gameobject(background);
}
