#include <lcom/lcf.h>
#include <stdint.h>

#include "menu.h"

#include "../Player/player.h"
#include "../Instructions/instructions.h"

#include "../../ImageAssets/Button.xpm"
#include "../../ImageAssets/Background.xpm"

typedef enum GameState {
  MAIN_MENU,
  INSTRUCTIONS,
  GAME,
  GAME_OVER,
  PAUSE,
  QUIT
} GameState;

extern GameState state;

extern ScreenInfo screen;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;

extern bool playing;

static bool pressed_menu_button = false;
static int8_t menu_current_selection = -1;

Button* playButton;
Button* instructionsButton;
Button* quitButton;
GameObject* background;

MenuNode* menuObjects = NULL;

static void checkMenuHovered(MenuNode** head) {

  MenuNode* current_button = *head;
  uint8_t index = 0;

  while (current_button != NULL) {

    Button* button = current_button->button;
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
        menu_current_selection = index;

      } else if (!last_pressed_was_mouse && menu_current_selection == index) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
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
      exitMenu();
      enterGame(false);
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

static void insertAtEndMenu(MenuNode** head, Button* button) {

  MenuNode* newNode = (MenuNode*)malloc(sizeof(MenuNode));

  newNode->button = button;
  newNode->next = NULL;

  MenuNode* current = *head;

  if (*head == NULL) {
    *head = newNode;
    return;
  }

  while (current->next != NULL) {
    current = current->next;  
  }

  current->next = newNode;
}
/*
static void deleteNodeMenu(MenuNode** head, Button* button) {
    MenuNode *temp = *head, *prev = NULL;

    // If the node to be deleted is the head node
    if (temp != NULL && temp->button == button) {
        *head = temp->next;
        destroy_gameobject(temp->button->hovering);
        destroy_gameobject(temp->button->no_hovering);
        free(temp->button);
        free(temp);
        return;
    }

    // Find the node to be deleted
    while (temp != NULL && temp->button != button) {
        prev = temp;
        temp = temp->next;
    }

    // If not found
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp);
}
*/
static void deleteListMenu(MenuNode** head) {
    MenuNode* current = *head;
    MenuNode* next;

    while (current != NULL) {
        next = current->next;
        destroy_sprite(current->button->hovering);
        destroy_sprite(current->button->no_hovering);
        destroy_gameobject_after_sprite_destroyed(current->button->button);
        free(current->button);
        free(current);
        current = next;
    }

    *head = NULL;
}

static Button* initializeMenuButton(xpm_map_t hovered, xpm_map_t no_hovered, int16_t x, int16_t y,
                             int16_t ox, int16_t oy, uint16_t z, bool square) {

  Button* button = (Button*)malloc(sizeof(Button));
  Sprite* normal = create_sprite((xpm_map_t)no_hovered, x, y, z, false, true);
  Sprite* hoveredSprite = create_sprite((xpm_map_t)hovered, x, y, z, false, true);
  GameObject* buttonObject = create_gameobject_from_sprite(normal, x, y, ox, oy);

  button->hovering = hoveredSprite;
  button->no_hovering = normal;
  button->button = buttonObject;
  button->x = x;
  button->y = y;
  button->origin_offset_x = ox;
  button->origin_offset_y = oy;

  return button;
}

static void hideMenuButtons(MenuNode** head) {
  MenuNode* current = *head;

  while (current != NULL) {
    current->button->button->sprite->is_visible = false;
    current = current->next;
  }
}

static void showMenuButtons(MenuNode** head) {
  MenuNode* current = *head;

  while (current != NULL) {
    current->button->button->sprite->is_visible = true;
    current = current->next;
  }
}

void initializeMenu() {
  background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);

  playButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
    screen.xres/2-1, screen.yres/2-101, -50, -25, 1, true);

  instructionsButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
    screen.xres/2-1, screen.yres/2-1, -50, -25, 1, true);

  quitButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
    screen.xres/2-1, screen.yres/2+99, -50, -25, 1, true);

  insertAtEndMenu(&menuObjects, playButton);
  insertAtEndMenu(&menuObjects, instructionsButton);
  insertAtEndMenu(&menuObjects, quitButton);
}

void enterMenu() {
  showMenuButtons(&menuObjects);
  background->sprite->is_visible = true;
}

void updateMenu() {
  checkMenuHovered(&menuObjects);
  checkMenuKeyboardInput(&keyboard_device->keyPresses);
}

void exitMenu() {
  pressed_menu_button = false;
  menu_current_selection = -1;
  hideMenuButtons(&menuObjects);
  background->sprite->is_visible = false;
}

void destroyMenu() {
  deleteListMenu(&menuObjects);
  destroy_gameobject(background);
}
