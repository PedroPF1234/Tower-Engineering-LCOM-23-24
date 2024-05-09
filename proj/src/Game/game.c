#include <lcom/lcf.h>
#include <stdint.h>

#include "Menu/menu.h"
#include "Player/player.h"

#include "../ImageAssets/Button.xpm"
#include "../ImageAssets/Background.xpm"

typedef enum GameState {
  MAIN_MENU,
  INSTRUCTIONS,
  GAME,
  GAME_OVER,
  PAUSE,
  QUIT
} GameState;

bool multiplayer = false;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern bool last_pressed_was_mouse;

static GameState state = MAIN_MENU;

// Boot flags
static bool game_booted = false;
static bool playing = false;
//

extern ScreenInfo screen;

MenuNode* menuObjects = NULL;

// Objects of Main Menu
Button* playButton;
Button* instructionsButton;
Button* quitButton;
GameObject* background;
//

// Object for instructions
GameObject* instructions_background;
//

// Object for games
GameObject* game_background;
Player* player1;
Player* player2;
int8_t speed1[2] = {0, 0};
int8_t speed2[2] = {0, 0};
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
      printf("Starting game...\n");
      state = GAME;
      playing = true;
      game_background->sprite->is_visible = true;
      pressed_menu_button = false;
      menu_current_selection = -1;
      hideMenuButtons(&menuObjects);
      background->sprite->is_visible = false;
      break;

    case 1:
      state = INSTRUCTIONS;
      instructions_background->sprite->is_visible = true;
      pressed_menu_button = false;
      menu_current_selection = -1;
      hideMenuButtons(&menuObjects);
      background->sprite->is_visible = false;
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

static void checkMenu(MenuNode** head, KeyPresses** keyPresses) {
  checkMenuHovered(head);
  checkMenuKeyboardInput(keyPresses);
}

static void checkInstructionsKeyboardInput(KeyPresses** head) {

  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        state = MAIN_MENU;
        instructions_background->sprite->is_visible = false;
        background->sprite->is_visible = true;
        showMenuButtons(&menuObjects);
        printf("Exiting instructions...\n");
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

static void checkGameKeyboardInput(KeyPresses** head) {

  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      case DOWN_ARROW_MAKE:
        speed1[1] = 1;
        break;

      case UP_ARROW_MAKE:
        speed1[1] = -1;
        break;

      case LEFT_ARROW_MAKE:
        speed1[0] = -1;
        break;

      case RIGHT_ARROW_MAKE:
        speed1[0] = 1;
        break;

      case DOWN_ARROW_BREAK:
        speed1[1] = 0;
        break;

      case UP_ARROW_BREAK:
        speed1[1] = 0;
        break;

      case LEFT_ARROW_BREAK:
        speed1[0] = 0;
        break;

      case RIGHT_ARROW_BREAK:
        speed1[0] = 0;
        break;

      case W_KEY_MAKE:
        speed2[1] = -1;
        break;

      case A_KEY_MAKE:
        speed2[0] = -1;
        break;

      case D_KEY_MAKE:
        speed2[0] = 1;
        break;

      case W_BREAK:
        speed2[1] = 0;
        break;

      case A_BREAK:
        speed2[0] = 0;
        break;

      case D_BREAK:
        speed2[0] = 0;
        break;

      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        state = MAIN_MENU;
        game_background->sprite->is_visible = false;
        background->sprite->is_visible = true;
        playing = false;
        setAllSpritesInvisible(player1);
        setAllSpritesInvisible(player2);
        printf("Exiting game...\n");
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

int game_main_loop() {

  switch (state)
  {
  case MAIN_MENU:
    if (!game_booted) {

      background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);

      // Initialize Button Objects of Menu
      playButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-101, -50, -25, 1, true);

      instructionsButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2-1, -50, -25, 1, true);

      quitButton = initializeMenuButton((xpm_map_t)QuitButton, (xpm_map_t)QuitButtonHovered,
       screen.xres/2-1, screen.yres/2+99, -50, -25, 1, true);
      //

      // Insert all Menu Buttons to the MenuNodesList
      insertAtEndMenu(&menuObjects, playButton);
      insertAtEndMenu(&menuObjects, instructionsButton);
      insertAtEndMenu(&menuObjects, quitButton);
      //

      // Game initialization
      player1 = initializePlayer(32, 28, -16, -29, 100);
      player2 = initializePlayer(32, 28, -16, -29, 100);
      game_background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);
      //

      // Instructions initialization
      instructions_background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true, true);
      //

      game_booted = true;
    }

    // Checks which button is being hovered by the mouse or selected by the keyboard
    checkMenu(&menuObjects, &keyboard_device->keyPresses);

    break;
  
  case GAME:
    checkGameKeyboardInput(&keyboard_device->keyPresses);

    if (playing) {
      updatePlayerPosition(player1, speed1[0], speed1[1]);
      updatePlayerSpriteBasedOnPosition(player1, speed1[0], speed1[1]);
      updatePlayerPosition(player2, speed2[0], speed2[1]);
      updatePlayerSpriteBasedOnPosition(player2, speed2[0], speed2[1]);
    }
    
    break;

  case INSTRUCTIONS:
    checkInstructionsKeyboardInput(&keyboard_device->keyPresses);

    break;

  case QUIT:
    printf("Exiting game... through quit.\n");
    deleteListMenu(&menuObjects);
    destroy_gameobject(background);
    destroy_gameobject(instructions_background);
    destroy_gameobject(game_background);
    destroyPlayer(player1);
    destroyPlayer(player2);
    return 1;

  default:
    return 1;
  }

  return 0;

}
