#include <lcom/lcf.h>
#include <stdint.h>

#include "player.h"
#include "../Menu/menu.h"

#include "../../Devices/device_controller.h"

#include "../../ImageAssets/Bicho.xpm"
#include "../../ImageAssets/Background.xpm"
#include "../../ImageAssets/Crossbow.xpm"

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
static float speedPlayer1[2] = {0, 0};
static float speedPlayer2[2] = {0, 0};

//Temp
GameObject* crossbow;
Sprite* crossbow_sprite;
static int currentAngle = 0;
//

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

void initializeGame() {
  player1 = initializePlayer(32, 28, -16, -29, 100);
  player2 = initializePlayer(32, 28, -16, -29, 100);
  game_background = create_gameobject((xpm_map_t)Background, 0, 0, 0, 0, 0, true, true);

  // Temp
  int num_sprites;
  crossbow_sprite = create_rotation_abled_sprite((xpm_map_t)Crossbow, 500, 500, 0xFFFF, false, true, &num_sprites);

  crossbow = create_gameobject_from_sprite(crossbow_sprite, 500, 500, 0, 0);
  //
}

//Temp
void testRotate() {
  currentAngle += 1;
  if (currentAngle >= 36) currentAngle = 0;
  Sprite* current = &crossbow_sprite[currentAngle];
  crossbow->origin_offset_x = -(current->width/2);
  crossbow->origin_offset_y = -(current->height/2);
  updateGameObjectSprite(crossbow, current);
}
//

void enterGame(bool multi) {
  multiplayer = multi;
  playing = true;
  game_background->sprite->is_visible = true;
  player1->player->sprite->is_visible = true;
  if (multi) player2->player->sprite->is_visible = true;
}

void updateGame() {
  checkGameKeyboardInput(&keyboard_device->keyPresses);
  if (playing) {
    updatePlayerPosition(player1, speedPlayer1[0], speedPlayer1[1]);
    updatePlayerSpriteBasedOnPosition(player1, speedPlayer1[0], speedPlayer1[1]);

    if (multiplayer) {
      updatePlayerPosition(player2, speedPlayer2[0], speedPlayer2[1]);
      updatePlayerSpriteBasedOnPosition(player2, speedPlayer2[0], speedPlayer2[1]);
    }
  }
}

void exitGame() {
  playing = false;
  game_background->sprite->is_visible = false;
  player1->player->sprite->is_visible = false;
  player2->player->sprite->is_visible = false;
}

void destroyGame() {
  destroy_gameobject(game_background);
  destroyPlayer(player1);
  destroyPlayer(player2);
}
