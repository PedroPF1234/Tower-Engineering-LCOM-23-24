#include <lcom/lcf.h>
#include <stdint.h>

#include "gameplay.h"
#include "../DataStructure/button.h"
#include "../Menu/menu.h"
#include "Player/player.h"
#include "Tower/towers.h"
#include "Arena/arena.h"
#include "Enemy/enemy.h"
#include "Bullet/bullet.h"
#include "PlayerBase/playerbase.h"
#include "Shop/shop.h"

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

static bool first_time_paused = true;
static bool pressed_pause_button = false;

static int8_t game_current_selection = -1;
static int8_t pause_current_selection = -1;

int16_t shop_y = 200;
int16_t shop_x = 1100;

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
BulletArray bullets;

// PlayerBase
PlayerBase* player_base;

// Shop
Shop* shop;

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
        else if (player1->speed[0] == 10.0f || player1->speed[0] == -10.0f) {
          player1->speed[1] = 7.0f;
          if (player1->speed[0] > 0.0f) player1->speed[0] = 7.0f;
          else player1->speed[0] = -7.0f;
        } else if (player1->speed[0] == 7.0f || player1->speed[0] == -7.0f) 
          player1->speed[1] = 7.0f;
        else player1->speed[1] = 10.0f;
        break;

      case W_MAKE:
        if (player1->speed[1] > 0.0f) player1->speed[1] = 0.0f;
        else if (player1->speed[0] == 10.0f || player1->speed[0] == -10.0f) {
          player1->speed[1] = -7.0f;
          if (player1->speed[0] > 0.0f) player1->speed[0] = 7.0f;
          else player1->speed[0] = -7.0f;
        } else if (player1->speed[0] == 7.0f || player1->speed[0] == -7.0f) 
          player1->speed[1] = -7.0f;
        else player1->speed[1] = -10.0f;
        break;

      case A_MAKE:
        if (player1->speed[0] > 0.0f) player1->speed[0] = 0.0f;
        else if (player1->speed[1] == 10.0f || player1->speed[1] == -10.0f) {
          player1->speed[0] = -7.0f;
          if (player1->speed[1] > 0.0f) player1->speed[1] = 7.0f;
          else player1->speed[1] = -7.0f;
        } else if (player1->speed[1] == 7.0f || player1->speed[1] == -7.0f) 
          player1->speed[0] = -7.0f;
        else player1->speed[0] = -10.0f;
        break;

      case D_MAKE:
        if (player1->speed[0] < 0.0f) player1->speed[0] = 0.0f;
        else if (player1->speed[1] == 10.0f || player1->speed[1] == -10.0f) {
          player1->speed[0] = 7.0f;
          if (player1->speed[1] > 0.0f) player1->speed[1] = 7.0f;
          else player1->speed[1] = -7.0f;
        } else if (player1->speed[1] == 7.0f || player1->speed[1] == -7.0f) 
          player1->speed[0] = 7.0f;
        else player1->speed[0] = 10.0f;
        break;

      case S_BREAK:
        player1->speed[1] = 0.0f;
        if (player1->speed[0] == 7.0f) player1->speed[0] = 10.0f;
        else if (player1->speed[0] == -7.0f) player1->speed[0] = -10.0f;
        break;

      case W_BREAK:
        player1->speed[1] = 0.0f;
        if (player1->speed[0] == 7.0f) player1->speed[0] = 10.0f;
        else if (player1->speed[0] == -7.0f) player1->speed[0] = -10.0f;
        break;

      case A_BREAK:
        player1->speed[0] = 0.0f;
        if (player1->speed[1] == 7.0f) player1->speed[1] = 10.0f;
        else if (player1->speed[1] == -7.0f) player1->speed[1] = -10.0f;
        break;

      case D_BREAK:
        player1->speed[0] = 0.0f;
        if (player1->speed[1] == 7.0f) player1->speed[1] = 10.0f;
        else if (player1->speed[1] == -7.0f) player1->speed[1] = -10.0f;
        break;

      case Q_BREAK:
        // shot- create new bullet
        if(player1->hasWeapon){
          float bullet_x = player1->x; 
          float bullet_y = player1->y; 
          float bullet_speed_x;
          float bullet_speed_y;
          int16_t damage = 10;
          //so the bullet goes horizontal or vertical
          if(player1->current_direction == UP ||player1->current_direction == UP_RIGHT ||
          player1->current_direction == UP_LEFT ||
          player1->current_direction == UP_IDLE) {
            bullet_speed_y = -1; 
            bullet_speed_x = 0;
          }
          else if(player1->current_direction == DOWN || 
          player1->current_direction == DOWN_RIGHT ||
          player1->current_direction == DOWN_LEFT ||
          player1->current_direction == DOWN_IDLE){
            bullet_speed_y = 1; 
            bullet_speed_x = 0;
          }
          else if(player1->current_direction == LEFT || player1->current_direction == LEFT_IDLE){
            bullet_speed_y = 0; 
            bullet_speed_x = -1;
          }
          else{
            bullet_speed_y = 0; 
            bullet_speed_x = 1;
          }

          Bullet* new_bullet = initializeBullet(bullet_x, bullet_y, 0, 0, bullet_speed_x, bullet_speed_y, damage);

          pushBulletArray(&bullets, new_bullet);
        }

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
            mountTurret(tower, CANNON);
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
        if (pause_current_selection < 0) pause_current_selection = 1;
        break;

      case DOWN_ARROW_BREAK:
        pause_current_selection++;
        if (pause_current_selection > 1) pause_current_selection = 0;
        break;

      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        printf("Pressed Esc to exit paused menu\n");
        state = GAME;
        pause_background->sprite->is_visible = false;
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

        updateGameObjectSprite(buttonObject, button->hovering);
        pause_current_selection = i;

        if (mouse_device->left_button_is_pressed) {
          pressed_pause_button = true;
          break;
        }

      } else if (!last_pressed_was_mouse && pause_current_selection == i) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
      }
    }
  }

  if (pressed_pause_button) {
    pressed_pause_button = false;
    switch (pause_current_selection)
    {
    case -1:
      break;
    
    case 0:
      state = GAME;
      pause_background->sprite->is_visible = false;
      break;

    case 1:
      state = MAIN_MENU;
      exitGame();
      enterMenu();
      break;

    default:
      break;
    }

    pause_current_selection = -1;
  }
}

static bool checkCollision(Bullet* bullet, Enemy* enemy) {
    int16_t bullet_left = bullet->x + bullet->origin_offset_x;
    int16_t bullet_right = bullet->x + bullet->origin_offset_x + bullet->sprite->width;
    int16_t bullet_top = bullet->y + bullet->origin_offset_y;
    int16_t bullet_bottom = bullet->y + bullet->origin_offset_y + bullet->sprite->height;

    int16_t enemy_left = enemy->x + enemy->origin_offset_x;
    int16_t enemy_right = enemy->x + enemy->origin_offset_x + enemy->enemy->sprite->width;
    int16_t enemy_top = enemy->y + enemy->origin_offset_y;
    int16_t enemy_bottom = enemy->y + enemy->origin_offset_y + enemy->enemy->sprite->height;

    return !(bullet_right < enemy_left || bullet_left > enemy_right || bullet_bottom < enemy_top || bullet_top > enemy_bottom);
}

static void updateGamePlay() {
  if (!first_time_paused) {
    hideButtons(&pause_buttons);
    first_time_paused = !first_time_paused;
  }
  checkGameKeyboardInput(&keyboard_device->keyPresses);
  checkGameHovered(&towers);
  if (playing) {
    updatePlayerPosition(player1, *current_arena);
    updatePlayerSpriteBasedOnPosition(player1);
    updateAllEnemyPositions(&enemies);
    updatePlayerBaseHealthBar(player_base);
    updateAllBulletPositions(&bullets);

    for (uint32_t i = 0; i < bullets.length; i++) {
      Bullet* bullet = getBulletArray(&bullets, i);
        if (bullet->active) {
          for (uint32_t j = 0; j < enemies.length; j++) {
            Enemy* enemy = getEnemyArray(&enemies, j);
            if (checkCollision(bullet, enemy)) {
              bullet->active = false;
              enemy->hit_points -= 1;
            }
          }
        }
    }

    if (multiplayer) {
      updatePlayerPosition(player2, *current_arena);
      updatePlayerSpriteBasedOnPosition(player2);
    }

    rotateTowersTowardsTarget(&towers, &enemies); 
  }
}

static void updatePause() {
  if (first_time_paused) {
    showButtons(&pause_buttons);
    first_time_paused = !first_time_paused;
  }
  checkPauseKeyboardInput(&keyboard_device->keyPresses);
  checkPauseHovered(&pause_buttons);
}

void initializeGameplay() {
  initializeDifferentTowerSprites();
  arenas = initializeArenas();
  player1 = initializePlayer(32, 28, -16, -29, 100);
  player2 = initializePlayer(32, 28, -16, -29, 100);
  towers = newTowerArray(20);
  enemies = newEnemyArray(100);
  bullets = newBulletArray(100);
  pause_buttons = newButtonArray(20);

  pushButtonArray(&pause_buttons, initializeButton((xpm_map_t)ResumeButtonHovered, (xpm_map_t)ResumeButton, screen.xres/2, screen.yres/2 - 100, -224, -25, 0xFFFE, true, true));

  pushButtonArray(&pause_buttons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2, screen.yres/2 + 100, -225, -25, 0xFFFE, true, true));

  hideButtons(&pause_buttons);

  game_background = create_spriteless_gameobject(0, 0, 0, 0, 0);
  pause_background = create_gameobject((xpm_map_t)PauseBackground, screen.xres/2, screen.yres/2, -300, -300, 0xFFFE, true, false);
}

void enterGame(bool multi, uint8_t arena) {
  resetDevicesChangingScreens();

  // Temporary but efficient fix. Should be replaced but works for now
  hideSprites(&player1->player->animatedSprite->sprites);
  hideSprites(&player2->player->animatedSprite->sprites);
  //

  multiplayer = multi;
  current_arena = &arenas[arena];
  add_sprite_to_spriteless_gameobject(game_background, current_arena->background);
  playing = true;
  showSprites(&player1->player->animatedSprite->sprites);

  towers = current_arena->towers;
  showTowers(&towers);

  if (multi) showSprites(&player2->player->animatedSprite->sprites);

  player_base = initializePlayerBase(current_arena->targert_coordinates[(current_arena->num_targets - 1) * 2], current_arena->targert_coordinates[(current_arena->num_targets - 1) * 2 + 1], 1000);
  
  shop = initializeShop(current_arena->shop_x, current_arena->shop_y);

  // Wont be needed when the pushing of turrets is moved to here.
  showTowers(&towers);
  //
}

void updateGame() {
  if (rtc_time->just_updated && state == GAME) {
    if (to_spawn_enemy) {
      to_spawn_enemy = false;
      pushEnemyArray(&enemies, initializeEnemy((float)current_arena->spawn_x, (float)current_arena->spawn_y, 0, 0, multiplayer ? 1500 : 1000, current_arena->targert_coordinates, current_arena->num_targets));
    } else {
      to_spawn_enemy = true;
    }
  }

  if (state == GAME) {
    updateGamePlay();
  } else if (state == PAUSE) {
    updatePause();
  }
}

void exitGame() {
  playing = false;
  hideSprites(&player1->player->animatedSprite->sprites);
  hideSprites(&player2->player->animatedSprite->sprites);

  hideTowers(&towers);
  towers = (TowerArray){NULL, 0, 0};

  hideButtons(&pause_buttons);
  destroyEnemyArray(&enemies);
  destroyPlayerBase(player_base);
  destroyShop(shop);
  remove_sprite_from_spriteless_gameobject(game_background);
  pause_background->sprite->is_visible = false;
}

void destroyGame() {
  destroy_gameobject(game_background);
  destroyPlayer(player1);
  destroyPlayer(player2);
  destroyTurretArray(&towers);
  destroyArenas(arenas);
  destroyEnemyArray(&enemies);

  //destroyBulletArray(&bullets)
}

