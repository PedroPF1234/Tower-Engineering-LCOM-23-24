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
Button* quitButton;
Button* playButton;

static uint8_t menu_current_selection = 0;

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

static void insertAtBeginMenu(MenuNode** head, Button* button) {
  
  MenuNode* newNode = (MenuNode*)malloc(sizeof(MenuNode));

  newNode->button = button;
  newNode->next = *head;

  *head = newNode;
}

static void deleteNodeMenu(MenuNode **head, Button* button) {
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

static Button* initializeMenuButton(xpm_map_t hovered, xpm_map_t no_hovered, int16_t x, int16_t y,
                                    int16_t ox, int16_t oy, uint16_t z, bool square) {

  Button* button = (Button*)malloc(sizeof(Button));
  GameObject* hoveredObject = create_gameobject((xpm_map_t)hovered, x, y, ox, oy, z, square, false);
  GameObject* noHovered = create_gameobject((xpm_map_t)no_hovered, x, y, ox, oy, z, square, true);

  button->hovering = hoveredObject;
  button->no_hovering = noHovered;
  button->x = x;
  button->y = y;
  button->origin_offset_x = ox;
  button->origin_offset_y = oy;

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

      // Initialize Button Objects of Menu
      quitButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-1, -50, -25, 1, true);

      playButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-101, -50, -25, 1, true);
      //

      // Insert all Menu Buttons to the MenuNodesList
      insertAtBeginMenu(&menuObjects, quitButton);
      insertAtEndMenu(&menuObjects, playButton);
      //

      game_booted = true;
    }

    // Checks which button is being hovered by the mouse
    checkMenuHovered(&menuObjects);

    if (keyboard_device->escape_key_pressed) {
      printf("Exiting game...\n");
      deleteNodeMenu(&menuObjects, quitButton);
      deleteNodeMenu(&menuObjects, playButton);
      return 1;
    }
    break;
  
  default:
    return 1;
  }

  return 0;

}
