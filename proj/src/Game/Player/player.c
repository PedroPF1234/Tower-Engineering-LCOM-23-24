#include <lcom/lcf.h>
#include <stdint.h>

#include "player.h"
#include "../Menu/menu.h"

#include "../../Devices/device_controller.h"

#include "../../ImageAssets/Bicho.xpm"
#include "../../ImageAssets/TowerBase.xpm"
#include "../../ImageAssets/Background.xpm"
#include "../../ImageAssets/Pause.xpm"

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

static bool pressed_game_button = false;
static uint8_t game_current_selection = -1;

static bool paused = false;

bool multiplayer = false;

GameObject* game_background;
GameObject* pause_text;
Player* player1;
Player* player2;
static float speedPlayer1[2] = {0, 0};
static float speedPlayer2[2] = {0, 0};

TowerNode* towers = NULL;

TowerBase* tower1;
TowerBase* tower2;
TowerBase* tower3;

static void checkGameKeyboardInput(KeyPresses** head) {

  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      case DOWN_ARROW_MAKE:

        if (speedPlayer1[1] < 0.0f) speedPlayer1[1] = 0.0f;
        else if (speedPlayer1[0] == 0.3f || speedPlayer1[0] == -0.3f) {
          speedPlayer1[1] = 0.212f;
          if (speedPlayer1[0] > 0.0f) speedPlayer1[0] = 0.212f;
          else speedPlayer1[0] = -0.212f;
        }
        else speedPlayer1[1] = 0.3f;
        break;

      case UP_ARROW_MAKE:
        if (speedPlayer1[1] > 0.0f) speedPlayer1[1] = 0.0f;
        else if (speedPlayer1[0] == 0.3f || speedPlayer1[0] == -0.3f) {
          speedPlayer1[1] = -0.212f;
          if (speedPlayer1[0] > 0.0f) speedPlayer1[0] = 0.212f;
          else speedPlayer1[0] = -0.212f;
        }
        else speedPlayer1[1] = -0.3f;
        break;

      case LEFT_ARROW_MAKE:
        if (speedPlayer1[0] > 0.0f) speedPlayer1[0] = 0.0f;
        else if (speedPlayer1[1] == 0.3f || speedPlayer1[1] == -0.3f) {
          speedPlayer1[0] = -0.212f;
          if (speedPlayer1[1] > 0.0f) speedPlayer1[1] = 0.212f;
          else speedPlayer1[1] = -0.212f;
        }
        else speedPlayer1[0] = -0.3f;
        break;

      case RIGHT_ARROW_MAKE:
        if (speedPlayer1[0] < 0.0f) speedPlayer1[0] = 0.0f;
        else if (speedPlayer1[1] == 0.3f || speedPlayer1[1] == -0.3f) {
          speedPlayer1[0] = 0.212f;
          if (speedPlayer1[1] > 0.0f) speedPlayer1[1] = 0.212f;
          else speedPlayer1[1] = -0.212f;
        }
        else speedPlayer1[0] = 0.3f;
        break;

      case DOWN_ARROW_BREAK:
        speedPlayer1[1] = 0.0f;
        break;

      case UP_ARROW_BREAK:
        speedPlayer1[1] = 0.0f;
        break;

      case LEFT_ARROW_BREAK:
        speedPlayer1[0] = 0.0f;
        break;

      case RIGHT_ARROW_BREAK:
        speedPlayer1[0] = 0.0f;
        break;

      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        if (state == GAME) {
          state = PAUSE;
          paused = true;
          pause_text->sprite->is_visible = true;
        } else if (state == PAUSE) {
          state = GAME;
          paused = false;
          pause_text->sprite->is_visible = false;
        }
        /*
        state = MAIN_MENU;
        exitGame();
        enterMenu();
        */
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

static void checkGameHovered(TowerNode** head) {
  TowerNode* current_tower = *head;
  uint8_t index = 0;

  while (current_tower != NULL) {

    TowerBase* tower = current_tower->tower;
    GameObject* towerObject = tower->base;

    int16_t mouse_x = mouse_device->mouse->x;
    int16_t mouse_y = mouse_device->mouse->y;
    int16_t leftMostBound = tower->x + tower->origin_offset_x;
    int16_t rightMostBound = tower->x + tower->origin_offset_x + tower->base->sprite->width;
    int16_t upMostBound = tower->y + tower->origin_offset_y;
    int16_t downMostBound = tower->y + tower->origin_offset_y + tower->base->sprite->height;

    if (!pressed_game_button) {
      if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
          mouse_y > upMostBound && mouse_y < downMostBound && last_pressed_was_mouse) {

        if (mouse_device->left_button_is_pressed) {
          pressed_game_button = true;
        }

        updateGameObjectSprite(towerObject, tower->baseHovered);
        game_current_selection = index;

      } else if (!last_pressed_was_mouse && game_current_selection == index) {
        updateGameObjectSprite(towerObject, tower->baseHovered);
      } else {
        updateGameObjectSprite(towerObject, tower->baseNormal);
      }
    }

    current_tower = current_tower->next;
    index++;
  }

  if (pressed_game_button) {
    pressed_game_button = false;
  }
}

static Player* initializePlayer(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp) {
  Player* new_player = (Player*)malloc(sizeof(Player));

  new_player->up = create_sprite((xpm_map_t)BichoUp, x, y, 1000, false, true);
  new_player->down = create_sprite((xpm_map_t)BichoDown, x, y, 1000, false, true);
  new_player->left = create_sprite((xpm_map_t)BichoLeft, x, y, 1000, false, true);
  new_player->right = create_sprite((xpm_map_t)BichoRight, x, y, 1000, false, true);
  new_player->up_left = create_sprite((xpm_map_t)BichoUpperLeft, x, y, 1000, false, true);
  new_player->up_right = create_sprite((xpm_map_t)BichoUpperRight, x, y, 1000, false, true);
  new_player->down_left = create_sprite((xpm_map_t)BichoLowerLeft, x, y, 1000, false, true);
  new_player->down_right = create_sprite((xpm_map_t)BichoLowerRight, x, y, 1000, false, true);
  new_player->stationary = create_sprite((xpm_map_t)BichoStationary, x, y, 1000, false, false);

  new_player->player = create_gameobject_from_sprite(new_player->stationary, x, y, ox, oy);

  new_player->x = x;
  new_player->y = y;
  new_player->origin_offset_x = ox;
  new_player->origin_offset_y = oy;
  new_player->hit_points = hp;

  return new_player;
}

static TowerBase* initializeTower(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp) {
  TowerBase* new_tower = (TowerBase*)malloc(sizeof(TowerBase));

  new_tower->baseNormal = create_sprite((xpm_map_t)TowerBaseNormal, x, y, 1000, false, false);
  new_tower->baseHovered = create_sprite((xpm_map_t)TowerBaseHovered, x, y, 1000, false, true);
  // Add sprite for tower turrets.

  new_tower->base = create_gameobject_from_sprite(new_tower->baseNormal, x, y, ox, oy);
  // Add gameobject for tower turrets.

  new_tower->x = x;
  new_tower->y = y;
  new_tower->origin_offset_x = ox;
  new_tower->origin_offset_y = oy;
  new_tower->hit_points = hp;

  return new_tower;
}

static void destroyPlayer(Player* player) {
  destroy_sprite(player->up);
  destroy_sprite(player->down);
  destroy_sprite(player->left);
  destroy_sprite(player->right);
  destroy_sprite(player->up_left);
  destroy_sprite(player->up_right);
  destroy_sprite(player->down_left);
  destroy_sprite(player->down_right);
  destroy_sprite(player->stationary);
  destroy_gameobject_after_sprite_destroyed(player->player);
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

  player->player->x = new_x;
  player->player->y = new_y;
}

static void destroyTower(TowerBase* tower) {
  destroy_sprite(tower->baseNormal);
  destroy_sprite(tower->baseHovered);
  // Destroy sprites for tower turrets.
  destroy_gameobject_after_sprite_destroyed(tower->base);
  // Destroy gameobjects for tower turrets.
  free(tower);
}

static void updatePlayerSpriteBasedOnPosition(Player* player, float x, float y) {

  if (x!=0.0f && y!=0.0f) {
    if (x > 0.0f) {
      if (y > 0.0f) {
        updateGameObjectSprite(player->player, player->down_right);
      } else if (y < 0.0f) {
        updateGameObjectSprite(player->player, player->up_right);
      }
    } else {
      if (y > 0.0f) {
        updateGameObjectSprite(player->player, player->down_left);
      } else if (y < 0.0f) {
        updateGameObjectSprite(player->player, player->up_left);
      }
    }
  } else {
    if (x > 0.0f) {
      updateGameObjectSprite(player->player, player->right);
    } else if (x < 0.0f) {
      updateGameObjectSprite(player->player, player->left);
    } else if (y > 0.0f) {
      updateGameObjectSprite(player->player, player->down);
    } else if (y < 0.0f) {
      updateGameObjectSprite(player->player, player->up);
    } else {
      updateGameObjectSprite(player->player, player->stationary);
    }
  }

}

static void addTowerToList(TowerNode** head, TowerBase* tower) {
    TowerNode* new_node = (TowerNode*)malloc(sizeof(TowerNode));
    new_node->tower = tower;
    new_node->next = *head;
    *head = new_node;
}

static void deleteListGame(TowerNode** head) {
    TowerNode* current = *head;
    TowerNode* next;

    while (current != NULL) {
        next = current->next;
        destroyTower(current->tower);
        free(current);
        current = next;
    }

    *head = NULL;
}

void initializeGame() {
  player1 = initializePlayer(32, 28, -16, -29, 100);
  player2 = initializePlayer(32, 28, -16, -29, 100);
  tower1 = initializeTower(128, 128, -64, -64, 100);
  tower2 = initializeTower(256, 256, -64, -64, 100);
  tower3 = initializeTower(384, 384, -64, -64, 100);
  addTowerToList(&towers, tower1);
  addTowerToList(&towers, tower2);
  addTowerToList(&towers, tower3);
  game_background = create_gameobject((xpm_map_t)Background, 0, 0, 0, 0, 0, true, true);
  pause_text = create_gameobject((xpm_map_t)PauseText, 0, 0, 0, 0, 0xFFFE, false, false);
}

void enterGame(bool multi) {
  multiplayer = multi;
  playing = true;
  game_background->sprite->is_visible = true;
  player1->player->sprite->is_visible = true;
  if (multi) player2->player->sprite->is_visible = true;
  tower1->base->sprite->is_visible = true;
  tower2->base->sprite->is_visible = true;
  tower3->base->sprite->is_visible = true;
}

void updateGame() {
  checkGameKeyboardInput(&keyboard_device->keyPresses);
  if (!paused) {
    checkGameHovered(&towers);
    if (playing) {
      updatePlayerPosition(player1, speedPlayer1[0], speedPlayer1[1]);
      updatePlayerSpriteBasedOnPosition(player1, speedPlayer1[0], speedPlayer1[1]);

      if (multiplayer) {
        updatePlayerPosition(player2, speedPlayer2[0], speedPlayer2[1]);
        updatePlayerSpriteBasedOnPosition(player2, speedPlayer2[0], speedPlayer2[1]);
      }
    }
  }
}

void exitGame() {
  playing = false;
  game_background->sprite->is_visible = false;
  player1->player->sprite->is_visible = false;
  player2->player->sprite->is_visible = false;
  tower1->base->sprite->is_visible = false;
  tower2->base->sprite->is_visible = false;
  tower3->base->sprite->is_visible = false;
}

void destroyGame() {
  destroy_gameobject(game_background);
  destroyPlayer(player1);
  destroyPlayer(player2);
  deleteListGame(&towers);
  free(towers);
}
