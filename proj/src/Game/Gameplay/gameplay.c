#include <lcom/lcf.h>
#include <stdint.h>

#include "gameplay.h"
#include "../Menu/menu.h"
#include "Player/player.h"
#include "Towers/towers.h"

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

        if (player1->speed[1] < 0.0f) player1->speed[1] = 0.0f;
        else if (player1->speed[0] == 0.3f || player1->speed[0] == -0.3f) {
          player1->speed[1] = 0.212f;
          if (player1->speed[0] > 0.0f) player1->speed[0] = 0.212f;
          else player1->speed[0] = -0.212f;
        }
        else player1->speed[1] = 0.3f;
        break;

      case UP_ARROW_MAKE:
        if (player1->speed[1] > 0.0f) player1->speed[1] = 0.0f;
        else if (player1->speed[0] == 0.3f || player1->speed[0] == -0.3f) {
          player1->speed[1] = -0.212f;
          if (player1->speed[0] > 0.0f) player1->speed[0] = 0.212f;
          else player1->speed[0] = -0.212f;
        }
        else player1->speed[1] = -0.3f;
        break;

      case LEFT_ARROW_MAKE:
        if (player1->speed[0] > 0.0f) player1->speed[0] = 0.0f;
        else if (player1->speed[1] == 0.3f || player1->speed[1] == -0.3f) {
          player1->speed[0] = -0.212f;
          if (player1->speed[1] > 0.0f) player1->speed[1] = 0.212f;
          else player1->speed[1] = -0.212f;
        }
        else player1->speed[0] = -0.3f;
        break;

      case RIGHT_ARROW_MAKE:
        if (player1->speed[0] < 0.0f) player1->speed[0] = 0.0f;
        else if (player1->speed[1] == 0.3f || player1->speed[1] == -0.3f) {
          player1->speed[0] = 0.212f;
          if (player1->speed[1] > 0.0f) player1->speed[1] = 0.212f;
          else player1->speed[1] = -0.212f;
        }
        else player1->speed[0] = 0.3f;
        break;

      case DOWN_ARROW_BREAK:
        player1->speed[1] = 0.0f;
        break;

      case UP_ARROW_BREAK:
        player1->speed[1] = 0.0f;
        break;

      case LEFT_ARROW_BREAK:
        player1->speed[0] = 0.0f;
        break;

      case RIGHT_ARROW_BREAK:
        player1->speed[0] = 0.0f;
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
        break;

      case BACKSPACE_BREAK:
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

void initializeGameplay() {
  player1 = initializePlayer(32, 28, -16, -29, 100);
  player2 = initializePlayer(32, 28, -16, -29, 100);
  tower1 = initializeTower(128, 128, -55, -55, 100);
  tower2 = initializeTower(256, 256, -55, -55, 100);
  tower3 = initializeTower(384, 384, -55, -55, 100);
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
      updatePlayerPosition(player1);
      updatePlayerSpriteBasedOnPosition(player1);

      if (multiplayer) {
        updatePlayerPosition(player2);
        updatePlayerSpriteBasedOnPosition(player2);
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
