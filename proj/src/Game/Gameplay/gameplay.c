#include <lcom/lcf.h>
#include <stdint.h>

#include "gameplay.h"
#include "../DataStructure/button.h"
#include "../Menu/menu.h"
#include "Player/player.h"
#include "Tower/towers.h"
#include "Arena/arena.h"
#include "Enemy/enemy.h"

#include "../gamestates.h"

#include "../../ImageAssets/Background.xpm"
#include "../../ImageAssets/Pause.xpm"
#include "../../ImageAssets/Button.xpm"

extern GameState state;

extern ScreenInfo screen;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern RTC_Time* rtc_time;
extern bool last_pressed_was_mouse;

extern bool playing;

static bool pressed_game_button = false;
static bool selecting_tower_base = false;
static bool to_spawn_enemy = false;

static bool paused = false;
static bool pressed_pause_button = false;

static int8_t game_current_selection = -1;
static int8_t pause_current_selection = -1;

bool multiplayer = false;

// Game Objects
// Arena
Arena* arenas;
Arena* current_arena;

// Backgrounds
GameObject* game_background;
GameObject* pause_background;

// Players
Player* player1;
Player* player2;

// Towers
TowerArray towers;

// Enemies
EnemyArray enemies;

// Pause Buttons
ButtonArray pause_buttons;

static void checkGameKeyboardInput(KeyPresses** head) {

  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      
      case UP_ARROW_BREAK:
        game_current_selection--;
        if (game_current_selection < 0) game_current_selection = 2;
        break;

      case DOWN_ARROW_BREAK:
        game_current_selection++;
        if (game_current_selection > 2) game_current_selection = 0;
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
          pause_background->sprite->is_visible = true;
        }
        break;

      case BACKSPACE_BREAK:
        state = MAIN_MENU;
        exitGame();
        enterMenu();
        break;

      case S_MAKE:
        if (player1->speed[1] < 0.0f) player1->speed[1] = 0.0f;
        else if (player1->speed[0] == 0.3f || player1->speed[0] == -0.3f) {
          player1->speed[1] = 0.212f;
          if (player1->speed[0] > 0.0f) player1->speed[0] = 0.212f;
          else player1->speed[0] = -0.212f;
        }
        else player1->speed[1] = 0.3f;
        break;

      case W_MAKE:
        if (player1->speed[1] > 0.0f) player1->speed[1] = 0.0f;
        else if (player1->speed[0] == 0.3f || player1->speed[0] == -0.3f) {
          player1->speed[1] = -0.212f;
          if (player1->speed[0] > 0.0f) player1->speed[0] = 0.212f;
          else player1->speed[0] = -0.212f;
        }
        else player1->speed[1] = -0.3f;
        break;

      case A_MAKE:
        if (player1->speed[0] > 0.0f) player1->speed[0] = 0.0f;
        else if (player1->speed[1] == 0.3f || player1->speed[1] == -0.3f) {
          player1->speed[0] = -0.212f;
          if (player1->speed[1] > 0.0f) player1->speed[1] = 0.212f;
          else player1->speed[1] = -0.212f;
        }
        else player1->speed[0] = -0.3f;
        break;

      case D_MAKE:
        if (player1->speed[0] < 0.0f) player1->speed[0] = 0.0f;
        else if (player1->speed[1] == 0.3f || player1->speed[1] == -0.3f) {
          player1->speed[0] = 0.212f;
          if (player1->speed[1] > 0.0f) player1->speed[1] = 0.212f;
          else player1->speed[1] = -0.212f;
        }
        else player1->speed[0] = 0.3f;
        break;

      case S_BREAK:
        player1->speed[1] = 0.0f;
        break;

      case W_BREAK:
        player1->speed[1] = 0.0f;
        break;

      case A_BREAK:
        player1->speed[0] = 0.0f;
        break;

      case D_BREAK:
        player1->speed[0] = 0.0f;
        break;

      case SPACE_BREAK:
        selecting_tower_base = !selecting_tower_base;
        if (selecting_tower_base) {
          game_current_selection = 0;
        } else {
          setTowerHovered(getTowerArray(&towers, game_current_selection), false);
          game_current_selection = -1;
        }
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

static void checkGameHovered(TowerArray* array) {
  if (selecting_tower_base) {

    for (int32_t i = 0; i < (int32_t)array->length; i++) {

      TowerBase* tower = getTowerArray(array, i);

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
            mountTowers(tower, 0);
          }

          setTowerHovered(tower, true);
          game_current_selection = i;

        } else if (!last_pressed_was_mouse && game_current_selection == i) {
          setTowerHovered(tower, true);
        } else {
          setTowerHovered(tower, false);
        }
      }
    }

    if (pressed_game_button) {
      pressed_game_button = false;
    }
  }
}

static void checkPauseKeyboardInput(KeyPresses** head) {
  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      
      case UP_ARROW_BREAK:
        pause_current_selection--;
        if (pause_current_selection < 0) pause_current_selection = 2;
        break;

      case DOWN_ARROW_BREAK:
        pause_current_selection++;
        if (pause_current_selection > 2) pause_current_selection = 0;
        break;

      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        if (state == PAUSE) {
          state = GAME;
          paused = false;
          pause_background->sprite->is_visible = false;
        }
        break;

      case S_BREAK:
        pause_current_selection++;
        if (pause_current_selection > 2) pause_current_selection = 0;
        break;

      case W_BREAK:
        pause_current_selection--;
        if (pause_current_selection < 0) pause_current_selection = 2;
        break;

      case ENTER_BREAK:
      case SPACE_BREAK:
        pressed_pause_button = true;
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

static void checkPauseHovered(ButtonArray* array) {
  for (int32_t i = 0; i < (int32_t)array->length; i++) {

    Button* button = getButtonArray(array, i);
    GameObject* buttonObject = button->button;

    int16_t mouse_x = mouse_device->mouse->x;
    int16_t mouse_y = mouse_device->mouse->y;
    int16_t leftMostBound = button->x + button->origin_offset_x;
    int16_t rightMostBound = button->x + button->origin_offset_x + button->button->sprite->width;
    int16_t upMostBound = button->y + button->origin_offset_y;
    int16_t downMostBound = button->y + button->origin_offset_y + button->button->sprite->height;

    if (!pressed_pause_button) {
      if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
          mouse_y > upMostBound && mouse_y < downMostBound && last_pressed_was_mouse) {

        if (mouse_device->left_button_is_pressed) {
          pressed_pause_button = true;
        }

        updateGameObjectSprite(buttonObject, button->hovering);
        pause_current_selection = i;

      } else if (!last_pressed_was_mouse && pause_current_selection == i) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
      }
    }
  }

  if (pressed_pause_button) {
    switch (pause_current_selection)
    {
    case -1:
      break;
    
    case 0:
      if (state == PAUSE) {
        state = GAME;
        paused = false;
        pause_background->sprite->is_visible = false;
      }
      break;

    case 1:
      state = MAIN_MENU;
      exitGame();
      enterMenu();
      break;
      break;

    default:
      break;
    }
  }
}

void initializeGameplay() {
  initializeDifferentTowerSprites();
  arenas = initializeArenas();
  player1 = initializePlayer(32, 28, -16, -29, 100);
  player2 = initializePlayer(32, 28, -16, -29, 100);
  towers = newTowerArray(20);
  enemies = newEnemyArray(100);
  pause_buttons = newButtonArray(20);

  pushButtonArray(&pause_buttons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2, screen.yres/2 - 100, -50, -25, 0xFFFE, true));

  pushButtonArray(&pause_buttons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2, screen.yres/2 + 100, -50, -25, 0xFFFE, true));

  // Pushing turrets by default method. Supposed to use current_arena to push the turrets later one.
  // So the pushing will be moved to the "enterGame" function.
  pushTowerArray(&towers, initializeTower(128, 128, -55, -55, 100));
  pushTowerArray(&towers, initializeTower(256, 256, -55, -55, 100));
  pushTowerArray(&towers, initializeTower(384, 384, -55, -55, 100));
  // 

  game_background = create_spriteless_gameobject(0, 0, 0, 0, 0);
  pause_background = create_gameobject((xpm_map_t)PauseBackground, screen.xres/2, screen.yres/2, -300, -300, 0xFFFE, true, false);
}

void enterGame(bool multi, uint8_t arena) {
  multiplayer = multi;
  current_arena = &arenas[arena];
  add_sprite_to_spriteless_gameobject(game_background, current_arena->background);
  playing = true;
  player1->player->sprite->is_visible = true;
  if (multi) player2->player->sprite->is_visible = true;

  // Wont be needed when the pushing of turrets is moved to here.
  showTowers(&towers);
  //
}

void updateGame() {
  
  if (rtc_time->just_updated) {
    if (to_spawn_enemy) {
      to_spawn_enemy = false;
      pushEnemyArray(&enemies, initializeEnemy((float)current_arena->spawn_x, (float)current_arena->spawn_y, 0, 0, multiplayer ? 150 : 100, current_arena->coordinate_targets, current_arena->num_targets));
    } else {
      to_spawn_enemy = true;
    }
  }

  checkGameKeyboardInput(&keyboard_device->keyPresses);
  if (!paused) {
    checkGameHovered(&towers);
    if (playing) {
      updatePlayerPosition(player1);
      updatePlayerSpriteBasedOnPosition(player1);
      updateAllEnemyPositions(&enemies);

      if (multiplayer) {
        updatePlayerPosition(player2);
        updatePlayerSpriteBasedOnPosition(player2);
      }
    }
  } else {
    checkPauseKeyboardInput(&keyboard_device->keyPresses);
    checkPauseHovered(&pause_buttons);
  }
  rotateTowersTowardsTarget(&towers, &enemies); 
}

void exitGame() {
  playing = false;
  player1->player->sprite->is_visible = false;
  player2->player->sprite->is_visible = false;

  // Should be replaced by the destroyTurretArray function.
  hideTowers(&towers);
  //
  destroyEnemyArray(&enemies);
  remove_sprite_from_spriteless_gameobject(game_background);
}

void destroyGame() {
  destroy_gameobject(game_background);
  destroyPlayer(player1);
  destroyPlayer(player2);
  destroyTurretArray(&towers);
  destroyArenas(arenas);
  destroyEnemyArray(&enemies);
}
