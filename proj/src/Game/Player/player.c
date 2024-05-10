#include <lcom/lcf.h>
#include <stdint.h>

#include "player.h"
#include "../Menu/menu.h"

#include "../../Devices/device_controller.h"

#include "../../ImageAssets/Bicho.xpm"
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

bool multiplayer = false;

GameObject* game_background;
Player* player1;
Player* player2;
float speed[2] = {0, 0};

static void checkGameKeyboardInput(KeyPresses** head) {

  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      case DOWN_ARROW_MAKE:
        speed[1] = 0.3f;
        break;

      case UP_ARROW_MAKE:
        speed[1] = -0.3f;
        break;

      case LEFT_ARROW_MAKE:
        speed[0] = -0.3f;
        break;

      case RIGHT_ARROW_MAKE:
        speed[0] = 0.3f;
        break;

      case DOWN_ARROW_BREAK:
        speed[1] = 0.0f;
        break;

      case UP_ARROW_BREAK:
        speed[1] = 0.0f;
        break;

      case LEFT_ARROW_BREAK:
        speed[0] = 0.0f;
        break;

      case RIGHT_ARROW_BREAK:
        speed[0] = 0.0f;
        break;

      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        state = MAIN_MENU;
        exitGame();
        enterMenu();
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


static Player* initializePlayer(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp) {
  Player* new_player = (Player*)malloc(sizeof(Player));

  new_player->up = create_gameobject((xpm_map_t)BichoUp, x, y, ox, oy, 1000, false, false);
  new_player->down = create_gameobject((xpm_map_t)BichoDown, x, y, ox, oy, 1000, false, false);
  new_player->left = create_gameobject((xpm_map_t)BichoLeft, x, y, ox, oy, 1000, false, false);
  new_player->right = create_gameobject((xpm_map_t)BichoRight, x, y, ox, oy, 1000, false, false);
  new_player->up_left = create_gameobject((xpm_map_t)BichoUpperLeft, x, y, ox, oy, 1000, false, false);
  new_player->up_right = create_gameobject((xpm_map_t)BichoUpperRight, x, y, ox, oy, 1000, false, false);
  new_player->down_left = create_gameobject((xpm_map_t)BichoLowerLeft, x, y, ox, oy, 1000, false, false);
  new_player->down_right = create_gameobject((xpm_map_t)BichoLowerRight, x, y, ox, oy, 1000, false, false);
  new_player->stationary = create_gameobject((xpm_map_t)BichoStationary, x, y, ox, oy, 1000, false, false);
  new_player->x = x;
  new_player->y = y;
  new_player->origin_offset_x = ox;
  new_player->origin_offset_y = oy;
  new_player->hit_points = hp;

  return new_player;
}

static void destroyPlayer(Player* player) {
  destroy_gameobject(player->up);
  destroy_gameobject(player->down);
  destroy_gameobject(player->left);
  destroy_gameobject(player->right);
  destroy_gameobject(player->up_left);
  destroy_gameobject(player->up_right);
  destroy_gameobject(player->down_left);
  destroy_gameobject(player->down_right);
  destroy_gameobject(player->stationary);
  free(player);
}

static void updatePlayerPosition(Player* player, float x, float y) {
  player->x += x;
  player->y += y;

  if (player->x < 0) player->x = 0;
  if (player->y < 0) player->y = 0;
  if (player->x > screen.xres) player->x = screen.xres;
  if (player->y > screen.yres) player->y = screen.yres;

  uint16_t new_x = (uint16_t) player->x;
  uint16_t new_y = (uint16_t) player->y;

  player->up->x = new_x;
  player->up->y = new_y;

  player->down->x = new_x;
  player->down->y = new_y;

  player->left->x = new_x;
  player->left->y = new_y;

  player->right->x = new_x;
  player->right->y = new_y;

  player->up_left->x = new_x;
  player->up_left->y = new_y;

  player->up_right->x = new_x;
  player->up_right->y = new_y;

  player->down_left->x = new_x;
  player->down_left->y = new_y;

  player->down_right->x = new_x;
  player->down_right->y = new_y;

  player->stationary->x = new_x;
  player->stationary->y = new_y;
}

static void setAllSpritesInvisible(Player* player) {
  player->up->sprite->is_visible = false;
  player->down->sprite->is_visible = false;
  player->left->sprite->is_visible = false;
  player->right->sprite->is_visible = false;
  player->up_left->sprite->is_visible = false;
  player->up_right->sprite->is_visible = false;
  player->down_left->sprite->is_visible = false;
  player->down_right->sprite->is_visible = false;
  player->stationary->sprite->is_visible = false;
}

static void updatePlayerSpriteBasedOnPosition(Player* player, float x, float y) {

  setAllSpritesInvisible(player);

  if (x!=0.0f && y!=0.0f) {
    if (x > 0.0f) {
      if (y > 0.0f) {
        player->down_right->sprite->is_visible = true;
      } else if (y < 0.0f) {
        player->up_right->sprite->is_visible = true;
      }
    } else {
      if (y > 0.0f) {
        player->down_left->sprite->is_visible = true;
      } else if (y < 0.0f) {
        player->up_left->sprite->is_visible = true;
      }
    }
  } else {
    if (x > 0.0f) {
      player->right->sprite->is_visible = true;
    } else if (x < 0.0f) {
      player->left->sprite->is_visible = true;
    } else if (y > 0.0f) {
      player->down->sprite->is_visible = true;
    } else if (y < 0.0f) {
      player->up->sprite->is_visible = true;
    } else {
      player->stationary->sprite->is_visible = true;
    }
  }

}

void initializeGame() {
  player1 = initializePlayer(32, 28, -16, -29, 100);
  player2 = initializePlayer(32, 28, -16, -29, 100);
  game_background = create_gameobject((xpm_map_t)Background, 0, 0, 0, 0, 0, true, true);
}

void enterGame(bool multi) {
  multiplayer = multi;
  playing = true;
  game_background->sprite->is_visible = true;
  player1->stationary->sprite->is_visible = true;
  if (multi) player2->stationary->sprite->is_visible = true;
}

void updateGame() {
  checkGameKeyboardInput(&keyboard_device->keyPresses);
  if (playing) {
    updatePlayerPosition(player1, speed[0], speed[1]);
    updatePlayerSpriteBasedOnPosition(player1, speed[0], speed[1]);
  }
}

void exitGame() {
  playing = false;
  game_background->sprite->is_visible = false;
  setAllSpritesInvisible(player1);
  setAllSpritesInvisible(player2);
}

void destroyGame() {
  destroy_gameobject(game_background);
  destroyPlayer(player1);
  destroyPlayer(player2);
}
