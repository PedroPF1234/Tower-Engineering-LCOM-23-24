#include <lcom/lcf.h>
#include <stdint.h>

#include "menu.h"

#include "../Gameplay/gameplay.h"
#include "../Instructions/instructions.h"

#include "../gamestates.h"

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

static ButtonArray newButtonArray(uint32_t capacity) {
  ButtonArray array;
  array.length = 0;

  if (capacity) {
    array.buttons = (Button*)malloc(capacity * sizeof(Button));
    array.capacity = capacity;
  } else {
    array.buttons = (Button*)malloc(sizeof(Button) * 10);
    array.capacity = 10;
  }
  return array;
}

static void pushButtonArray(ButtonArray* array, Button* button) {

  if (array->capacity != array->length) {
        array->buttons[array->length] = *button;
    } else {
      uint32_t newCapacity = array->capacity * 2;
      Button* oldPointer = array->buttons;
      Button* newPointer = (Button*)malloc(newCapacity * sizeof(Button));
      array->buttons = newPointer;
      for (uint32_t i = 0; i < array->length; i++) {
        newPointer[i] = oldPointer[i];
      }
      free(oldPointer);
      array->buttons[array->length] = *button;
    }
    array->length++;
}

Button* getButtonArray(ButtonArray* array, uint32_t index) {
    if (index < array->length) {
        return &array->buttons[index];
    } else {
        return NULL;
    }
}

void removeButtonArray(ButtonArray* array, uint32_t index) {
    if (index < array->length) {
      Button* button = getButtonArray(array, index);
      destroy_sprite(button->hovering);
      destroy_sprite(button->no_hovering);
      button->button->sprite = NULL;
      destroy_gameobject(button->button);
      for (uint32_t i = index; i < array->length - 1; i++) {
          array->buttons[i] = array->buttons[i + 1];
      }
      memset(&array->buttons[array->length - 1], 0, sizeof(Button));
      array->length--;
    }
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

static void destroyButtonArray(ButtonArray* array) {

    for (int32_t i = 0; i < (int32_t)array->length; i++) {
        Button* button = getButtonArray(array, i);
        destroy_sprite(button->hovering);
        destroy_sprite(button->no_hovering);
        button->button->sprite = NULL;
        destroy_gameobject(button->button);
        free(button);
    }
}

static Button* initializeMenuButton(xpm_map_t hovered, xpm_map_t no_hovered, int16_t x, int16_t y,
                             int16_t ox, int16_t oy, uint16_t z, bool square) {

  Button* button = (Button*)malloc(sizeof(Button));
  Sprite* normal = create_sprite((xpm_map_t)no_hovered, x, y, false, true);
  Sprite* hoveredSprite = create_sprite((xpm_map_t)hovered, x, y, false, true);
  GameObject* buttonObject = create_gameobject_from_sprite(normal, x, y, ox, oy, z);

  button->hovering = hoveredSprite;
  button->no_hovering = normal;
  button->button = buttonObject;
  button->x = x;
  button->y = y;
  button->origin_offset_x = ox;
  button->origin_offset_y = oy;

  return button;
}

static void hideMenuButtons(ButtonArray* array) {
  for (int32_t i = 0; i < (int32_t)array->length; i++) {
    getButtonArray(array, i)->button->sprite->is_visible = false;
  }
}

static void showMenuButtons(ButtonArray* array) {
  for (int32_t i = 0; i < (int32_t)array->length; i++) {
    getButtonArray(array, i)->button->sprite->is_visible = true;
  }
}

void initializeMenu() {
  menuButtons = newButtonArray(20);
  background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);

  // Play Button
  pushButtonArray(&menuButtons, initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered, screen.xres/2-1, screen.yres/2-101, -50, -25, 1, true));

  // Instructions Button
  pushButtonArray(&menuButtons, initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered, screen.xres/2-1, screen.yres/2-1, -50, -25, 1, true));

  // Quit Button
  pushButtonArray(&menuButtons, initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered, screen.xres/2-1, screen.yres/2+99, -50, -25, 1, true));
}

void enterMenu() {
  showMenuButtons(&menuButtons);
  background->sprite->is_visible = true;
}

void updateMenu() {
  checkMenuHovered(&menuButtons);
  checkMenuKeyboardInput(&keyboard_device->keyPresses);
}

void exitMenu() {
  pressed_menu_button = false;
  menu_current_selection = -1;
  hideMenuButtons(&menuButtons);
  background->sprite->is_visible = false;
}

void destroyMenu() {
  destroyButtonArray(&menuButtons);
  destroy_gameobject(background);
}
