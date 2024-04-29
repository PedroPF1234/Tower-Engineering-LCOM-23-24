#include <lcom/lcf.h>
#include <stdint.h>

#include "../Devices/device_controller.h"

#include "../ImageAssets/Button.xpm"

typedef enum gameState {
  MAIN_MENU,
  GAME,
  GAME_OVER,
  PAUSE
} gameState;

typedef struct Button {
  GameObject* hovering;
  GameObject* no_hovering;
  int16_t x, y;
  int16_t origin_offset_x, origin_offset_y;
} Button;

typedef struct MenuNode {
    Button* button;
    struct MenuNode* next;
} MenuNode;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;

static gameState state = MAIN_MENU;

static bool game_booted = false;

static ScreenInfo screen;

MenuNode* menuObjects = NULL;

// Objects of Main Menu
GameObject* quit;
GameObject* quitHovered;
Button* quitButton;

static uint8_t menu_current_selection = 0;


/*
static void insertAtEnd(MenuNode** head, Button* button) {

  MenuNode* newNode = (MenuNode*)malloc(sizeof(MenuNode));

  newNode->button = button;
  newNode->next = NULL;

  MenuNode* current = *head;

  while (current != NULL) {
    current = current->next;  
  }

  current->next = newNode;
}
*/

static void insertAtBegin(MenuNode** head, Button* button) {
  
  MenuNode* newNode = (MenuNode*)malloc(sizeof(MenuNode));

  newNode->button = button;
  newNode->next = *head;

  *head = newNode;
}

static void deleteNode(MenuNode **head, Button* button) {
    MenuNode *temp = *head, *prev = NULL;

    // If the node to be deleted is the head node
    if (temp != NULL && temp->button == button) {
        *head = temp->next;
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

static Button* initializeMenuButton(GameObject* hovered, GameObject* no_hovered) {

  Button* button = (Button*)malloc(sizeof(Button));
  button->hovering = hovered;
  button->no_hovering = no_hovered;
  button->x = hovered->x;
  button->y = hovered->y;
  button->origin_offset_x = hovered->origin_offset_x;
  button->origin_offset_y = hovered->origin_offset_y;

  return button;
}

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

      // Initialize GameObjects of Menu
      quit = create_gameobject((xpm_map_t) QuitButton, screen.xres/2, screen.yres/2,
         -50, -25, 1, true, true);
      quitHovered = create_gameobject((xpm_map_t) QuitButtonHovered, screen.xres/2, screen.yres/2,
         -50, -25, 1, true, false);
      //

      // Initialize Button Objects of Menu
      quitButton = initializeMenuButton(quitHovered, quit);
      //

      // Insert all Menu Buttons to the MenuNodesList
      insertAtBegin(&menuObjects, quitButton);
      //

      game_booted = true;
    }

    // Checks which button is being hovered by the mouse
    checkMenuHovered(&menuObjects);

    if (keyboard_device->escape_key_pressed) {
      printf("Exiting game...\n");
      deleteNode(&menuObjects, quitButton);
      destroy_gameobject(quit);
      destroy_gameobject(quitHovered);
      return 1;
    }
    break;
  
  default:
    return 1;
  }

  return 0;

}
