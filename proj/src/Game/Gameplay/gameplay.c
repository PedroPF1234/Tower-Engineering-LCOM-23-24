#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>


#include "gameplay.h"
#include "economy.h"
#include "../DataStructure/button.h"
#include "../Menu/menu.h"
#include "Player/player.h"
#include "Tower/towers.h"
#include "Arena/arena.h"
#include "Enemy/enemy.h"
#include "Bullet/bullet.h"
#include "PlayerBase/playerbase.h"
#include "Shop/shop.h"
#include "Money/money.h"
#include "Weapon/weapon.h"

#include "../gamestates.h"

#include "../../ImageAssets/Background.xpm"
#include "../../ImageAssets/Pause.xpm"
#include "../../ImageAssets/Button.xpm"
#include "../../ImageAssets/Towers.xpm"
#include "../../ImageAssets/Money.xpm"

extern GameState state;

extern ScreenInfo screen;

extern MouseDevice* mouse_device;
extern KeyboardDevice* keyboard_device;
extern RTC_Time* rtc_time;
extern bool last_pressed_was_mouse;

extern bool playing;

extern bool can_shop;
extern bool can_tower;

extern int16_t tower_index;

TurretType current_turret = CROSSBOW;

static bool pressed_game_button = false;
static bool to_spawn_enemy = false;

static bool first_time_paused = true;
static bool pressed_pause_button = false;

static bool first_time_shop = true;
static bool pressed_shop_button = false;

static bool first_time_tower = true;
static bool pressed_tower_button = false;
static bool type_tower_menu = false; // false for base, true for tower
TowerBase* selected_tower_base = NULL;
int8_t selected_tower_index = -1;

static int8_t game_current_selection = -1;
static int8_t pause_current_selection = -1;
static int8_t shop_current_selection = -1;
static int8_t tower_current_selection = -1;

bool multiplayer = false;

static int8_t unlocked_turrets[] = {1, 0, 0};

Economy* economy;

// Game Objects
// Arena
Arena* arenas;
Arena* current_arena;

// Backgrounds
GameObject* game_background;
GameObject* pause_background;
GameObject* shop_background;
GameObject* tower_background;

// Players
Player* player1;
Player* player2;

// Towers
TowerArray towers;

// Enemies
EnemyArray enemies;
BulletArray bullets;

// PlayerBase
PlayerBase player_base;

// Shop
Shop shop;

// Buttons
ButtonArray pause_buttons;
ButtonArray shop_buttons;
ButtonArray tower_buttons;

// Money
Money* money;

// Weapons
Weapon* player_weapon;

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
      case E_BREAK:
        if(state == GAME && can_shop) {
          state = SHOP_MENU;
          shop_background->sprite->is_visible = true;
        } else if (state == GAME && can_tower) {
          state = TOWER_MENU;
          tower_background->sprite->is_visible = true;
        }
        break;

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
        break;
        
      case SPACE_BREAK:
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

  int16_t mouse_x = mouse_device->mouse->x;
  int16_t mouse_y = mouse_device->mouse->y;

  if (!pressed_game_button) {
    if (mouse_device->left_button_is_pressed) {
          pressed_game_button = true;
    }
  }

  if (pressed_game_button) {
    pressed_game_button = false;
    if(player1->hasWeapon){
      float bullet_x = player1->x; 
      float bullet_y = player1->y+player1->origin_offset_y - 10; 
      int16_t damage = 200;

      Bullet* new_bullet = initializeBullet(bullet_x, bullet_y, mouse_x, mouse_y, damage);

      pushBulletArray(&bullets, new_bullet);
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
        if (pause_current_selection < 0) pause_current_selection = (int8_t)pause_buttons.length -1;
        break;

      case DOWN_ARROW_BREAK:
        pause_current_selection++;
        if (pause_current_selection > (int8_t)pause_buttons.length -1) pause_current_selection = 0;
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
        if (pause_current_selection > (int8_t)pause_buttons.length -1) pause_current_selection = 0;
        break;

      case W_BREAK:
        pause_current_selection--;
        if (pause_current_selection < 0) pause_current_selection = (int8_t)pause_buttons.length -1;
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

static void checkShopKeyboardInput(KeyPresses** head) {
  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      
      case UP_ARROW_BREAK:
        shop_current_selection--;
        if (shop_current_selection < 0) shop_current_selection = 1;
        break;

      case DOWN_ARROW_BREAK:
        shop_current_selection++;
        if (shop_current_selection > 1) shop_current_selection = 0;
        break;

      default:
        break;
      }
    } else {
      switch (current->key)
      {
      case ESC_BREAK:
        printf("Pressed Esc to exit shop menu\n");
        state = GAME;
        shop_background->sprite->is_visible = false;
        break;

      case S_BREAK:
        shop_current_selection++;
        if (shop_current_selection > 1) shop_current_selection = 0;
        break;

      case W_BREAK:
        shop_current_selection--;
        if (shop_current_selection < 0) shop_current_selection = 1;
        break;

      case ENTER_BREAK:
      case SPACE_BREAK:
        pressed_shop_button = true;
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

static void checkShopHovered(ButtonArray* array) {
    for (int32_t i = 0; i < (int32_t)array->length; i++) {

    Button* button = getButtonArray(array, i);
    GameObject* buttonObject = button->button;

    int16_t mouse_x = mouse_device->mouse->x;
    int16_t mouse_y = mouse_device->mouse->y;
    int16_t leftMostBound = button->x + button->origin_offset_x;
    int16_t rightMostBound = button->x + button->origin_offset_x + button->button->sprite->width;
    int16_t upMostBound = button->y + button->origin_offset_y;
    int16_t downMostBound = button->y + button->origin_offset_y + button->button->sprite->height;

    if (!pressed_shop_button) {
      if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
          mouse_y > upMostBound && mouse_y < downMostBound && last_pressed_was_mouse) {

        updateGameObjectSprite(buttonObject, button->hovering);
        shop_current_selection = i;

        if (mouse_device->left_button_is_pressed) {
          pressed_shop_button = true;
          break;
        }

      } else if (!last_pressed_was_mouse && shop_current_selection == i) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
      }
    }
  }

  if (pressed_shop_button) {
    pressed_shop_button = false;
    switch (shop_current_selection)
    {
    case -1:
      break;
    
    case 0:
      state = GAME;
      shop_background->sprite->is_visible = false;
      break;

    case 1:
      current_turret = CANNON;
      unlocked_turrets[1]=1;
      break;

    case 2:
      current_turret = LASER;
      unlocked_turrets[2]=1;
      break;    

    default:
      break;
    }

    shop_current_selection = -1;
  }
}

static void checkTowerMenuKeyboardInput(KeyPresses** head) {
  KeyPresses* current = *head;

  while (current != NULL) {
    if (current->special) {
      switch (current->key)
      {
      case UP_ARROW_BREAK:
        tower_current_selection--;
        if (tower_current_selection < 0) tower_current_selection = (int8_t)tower_buttons.length - 1;
        break;

      case DOWN_ARROW_BREAK:
        tower_current_selection++;
        if (tower_current_selection > (int8_t)tower_buttons.length - 1) tower_current_selection = 0;
        break;

        default:
        break;
      }
    } else {
      switch (current->key)
      {
        case ESC_BREAK:
          printf("Pressed Esc to exit tower menu\n");
          state = GAME;
          tower_background->sprite->is_visible = false;
          selected_tower_base = NULL;
          selected_tower_index = -1;
          tower_current_selection = -1;
          pressed_tower_button = false;
          type_tower_menu = false;
          break;

        case S_BREAK:
          tower_current_selection++;
          if (tower_current_selection > (int8_t)tower_buttons.length-1) tower_current_selection = 0;
          break;

        case W_BREAK:
          tower_current_selection--;
          if (tower_current_selection < 0) tower_current_selection = (int8_t)tower_buttons.length-1;
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

static void checkTowerMenuHovered(ButtonArray* array) {
  for (int32_t i = 0; i < (int32_t)array->length; i++) {

    Button* button = getButtonArray(array, i);
    GameObject* buttonObject = button->button;

    int16_t mouse_x = mouse_device->mouse->x;
    int16_t mouse_y = mouse_device->mouse->y;
    int16_t leftMostBound = button->x + button->origin_offset_x;
    int16_t rightMostBound = button->x + button->origin_offset_x + button->button->sprite->width;
    int16_t upMostBound = button->y + button->origin_offset_y;
    int16_t downMostBound = button->y + button->origin_offset_y + button->button->sprite->height;

    if (!pressed_shop_button) {
      if (mouse_x > leftMostBound && mouse_x < rightMostBound &&
          mouse_y > upMostBound && mouse_y < downMostBound && last_pressed_was_mouse) {

        updateGameObjectSprite(buttonObject, button->hovering);
        tower_current_selection = i;

        if (mouse_device->left_button_is_pressed) {
          pressed_tower_button = true;
          break;
        }

      } else if (!last_pressed_was_mouse && tower_current_selection == i) {
        updateGameObjectSprite(buttonObject, button->hovering);
      } else {
        updateGameObjectSprite(buttonObject, button->no_hovering);
      }
    }
  }

  if (pressed_tower_button) {
    pressed_tower_button = false;
    bool to_exit = false;
    switch (tower_current_selection)
    {
    case -1:
      break;
    
    case 0:
      state = GAME;
      tower_background->sprite->is_visible = false;
      selected_tower_base = NULL;
      selected_tower_index = -1;
      tower_current_selection = -1;
      pressed_tower_button = false;
      type_tower_menu = false;
      break;

    case 1:
      if (type_tower_menu) {
        // Switching tower target type to previous
        switch (selected_tower_base->targetting)
        {
        case FIRST:
          selected_tower_base->targetting = LAST;
          break;

        case CLOSEST:
          selected_tower_base->targetting = FIRST;
          break;

        case LAST:
          selected_tower_base->targetting = CLOSEST;
          break;
        
        default:
          break;
        }
      } else {
        selected_tower_index = 0;
      }
      break;

    case 2:
      if (type_tower_menu) {
        // Switching tower target type to next
        switch (selected_tower_base->targetting)
        {
        case FIRST:
          selected_tower_base->targetting = CLOSEST;
          break;

        case CLOSEST:
          selected_tower_base->targetting = LAST;
          break;

        case LAST:
          selected_tower_base->targetting = FIRST;
          break;
        
        default:
          break;
        }
      } else {
        selected_tower_index = 1;
      }
      break;

    case 3: 
      if (type_tower_menu) {
        // Upgrade turret damage
        selected_tower_base->damage *= 1.1;
      } else {
        selected_tower_index = 2;
      }
      break;  

    case 4:
  
      if (type_tower_menu) {
        // Unmount turret
        unmountTurret(selected_tower_base);
        to_exit = true;
      } else {
        switch (selected_tower_index)
        {
        case 0:
          mountTurret(selected_tower_base, CROSSBOW);
          to_exit = true;
          break;

        case 1:
          if (unlocked_turrets[1])
          {
            mountTurret(selected_tower_base, CANNON);
            to_exit = true;
          }
          break;

        case 2:
          if (unlocked_turrets[2])
            mountTurret(selected_tower_base, LASER);
            to_exit = true;
          {
          break;
        default:
          break;
        }
      }
      break;

    default:
      break;
    }

    if (to_exit) {
        state = GAME;
        tower_background->sprite->is_visible = false;
        selected_tower_base = NULL;
        selected_tower_index = -1;
        tower_current_selection = -1;
        pressed_tower_button = false;
        type_tower_menu = false;
      }

    shop_current_selection = -1;
  }
  }
}


static bool checkCollision(Bullet* bullet, Enemy* enemy) {
    int16_t bullet_right = bullet->x + bullet->origin_offset_x + bullet->bullet->sprite->width;
    int16_t bullet_left = bullet->x + bullet->origin_offset_x;
    int16_t bullet_top = bullet->y + bullet->origin_offset_y;
    int16_t bullet_bottom = bullet->y + bullet->origin_offset_y + bullet->bullet->sprite->height;
 

    int16_t enemy_left = enemy->x + enemy->origin_offset_x;
    int16_t enemy_right = enemy->x + enemy->origin_offset_x + enemy->enemy->sprite->width;
    int16_t enemy_top = enemy->y + enemy->origin_offset_y;
    int16_t enemy_bottom = enemy->y + enemy->origin_offset_y + enemy->enemy->sprite->height;

  return bullet_right >= enemy_left && bullet_left <= enemy_right && 
  bullet_top <= enemy_bottom && bullet_bottom >= enemy_top;

}

static void updateGamePlay() {
  if (!first_time_paused) {
    hideButtons(&pause_buttons);
    first_time_paused = !first_time_paused;
  }

  if(!first_time_shop) {
    hideButtons(&shop_buttons);
    first_time_shop = !first_time_shop;
  }

  if (!first_time_tower) {
    destroyButtonArray(&tower_buttons);
    printf("Destroyed tower buttons\n");
    printf("Tower buttons length: %d\n", tower_buttons.length);
    first_time_tower = !first_time_tower;
  }

  if (can_shop) {
    current_arena->shop.shopButton->sprite->is_visible = true;
  } else {
    current_arena->shop.shopButton->sprite->is_visible = false;
  }

  checkGameKeyboardInput(&keyboard_device->keyPresses);
  checkGameHovered(&towers);
  if (playing) {
    updatePlayerPosition(player1, *current_arena);
    updatePlayerSpriteBasedOnPosition(player1);
    player_base.hit_points -= updateAllEnemyPositions(&enemies);
    updatePlayerBaseHealthBar(&player_base);
    updateAllBulletPositions(&bullets);

    if (player_base.hit_points <= 0) {
      state = MAIN_MENU;
      exitGame();
      enterMenu();
    }

    //Towers Shot Update
    if (towers.length != 0) {
      for (uint32_t towerNum = 0; towerNum < towers.length; towerNum++) {
        TowerBase* tower = getTowerArray(&towers, towerNum);

        //1 second of time added
        if (rtc_time->just_updated) {
          tower->timeWithoutShooting++;
        }
        
        if (tower->target != NULL && tower->cooldown <= tower->timeWithoutShooting) {
            float bullet_x = tower->x;
            float bullet_y = tower->y;
            int16_t damage = tower->damage;

            Bullet* new_bullet = initializeBullet(bullet_x, bullet_y, tower->target->x, tower->target->y, damage);

            pushBulletArray(&bullets, new_bullet);

            tower->timeWithoutShooting = 0;
        }
      }
    }

    //Update enemy array
    for (uint32_t j = 0; j < enemies.length; j++) {
      Enemy* enemy = getEnemyArray(&enemies, j);

      //Collision with the bullet
      for (uint32_t i = 0; i < bullets.length; i++) {
        Bullet* bullet = getBulletArray(&bullets, i);
        if (bullet->active != false && checkCollision(bullet, enemy)) {
          bullet->active = false;
          enemy->hit_points -= bullet->damage;
        }
      }

      //death of enemy
      if(enemy->hit_points <= 0){
        removeEnemyArray(&enemies, j);
        money->money_amount += 10;
        updateGameObjectSprites(money, 0);
        j--;
      }
    }

    if (multiplayer) {
      updatePlayerPosition(player2, *current_arena);
      updatePlayerSpriteBasedOnPosition(player2);
    }

    rotateTowersTowardsTarget(&towers, &enemies);

    updateWeapon(player_weapon, player1, mouse_device->mouse->x, mouse_device->mouse->y);
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

static void updateShop() {
  if(first_time_shop) {
    showButtons(&shop_buttons);
    first_time_shop = !first_time_shop;
  }
  checkShopKeyboardInput(&keyboard_device->keyPresses);
  checkShopHovered(&shop_buttons);
}

static void updateTowerMenu() {

  if (first_time_tower) {

    selected_tower_base = getTowerArray(&towers, tower_index);

    printf("Tower Array length: %d\n", towers.length);

    pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)ExitButtonHovered, (xpm_map_t)ExitButton, screen.xres/2 + 300, screen.yres/2 - 300, 0xFFFE, false, true));
    
    if (getTowerArray(&towers, tower_index)->turret->sprite == NULL) {
      type_tower_menu = false;

      pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)ResumeButtonHovered, (xpm_map_t)ResumeButton, screen.xres/2 - 300, screen.yres/2 - 250, 0xFFFE, false, true));
      pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)ResumeButtonHovered, (xpm_map_t)ResumeButton, screen.xres/2 - 300, screen.yres/2 - 100, 0xFFFE, false, true));
      pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)ResumeButtonHovered, (xpm_map_t)ResumeButton, screen.xres/2 - 300, screen.yres/2 + 50, 0xFFFE, false, true));
      pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)MountButtonHovered, (xpm_map_t)MountButton, screen.xres/2 - 300, screen.yres/2 + 200, 0xFFFE, false, true));

    } else {
      type_tower_menu = true;

      pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)LeftButton, (xpm_map_t) LeftButton, screen.xres/2 - 300, screen.yres/2 - 250, 0xFFFE, false, true));
      pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)RightButton, (xpm_map_t)RightButton, screen.xres/2 - 300, screen.yres/2 - 100, 0xFFFE, false, true));
      pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)UpgradeButtonHovered, (xpm_map_t)UpgradeButton, screen.xres/2 - 300, screen.yres/2 + 50, 0xFFFE, false, true));
      pushButtonArray(&tower_buttons, initializeButton((xpm_map_t)UnmountButtonHovered, (xpm_map_t)UnmountButton, screen.xres/2 - 300, screen.yres/2 + 200, 0xFFFE, false, true));
    }

    first_time_tower = !first_time_tower;
  }

  checkTowerMenuKeyboardInput(&keyboard_device->keyPresses);
  checkTowerMenuHovered(&tower_buttons);
}

void initializeGameplay() {
  initializeDifferentTowerSprites();
  initializeBulletSprites();
  economy = read_prices_info(Prices);
  arenas = initializeArenas();
  player1 = initializePlayer(32, 28, -16, -29, 100);
  player2 = initializePlayer(32, 28, -16, -29, 100);
  player_weapon = initializeWeapon(32, 28);
  hideWeapon(player_weapon);
  
  money = initializeMoney();
  hideGameObjects(&money->moneyDigitsGameObjects);

  enemies = newEnemyArray(100);
  bullets = newBulletArray(100);
  pause_buttons = newButtonArray(20);
  shop_buttons = newButtonArray(20);
  tower_buttons = newButtonArray(20);

  pushButtonArray(&pause_buttons, initializeButton((xpm_map_t)ResumeButtonHovered, (xpm_map_t)ResumeButton, screen.xres/2, screen.yres/2 - 100, 0xFFFE, false, true));

  pushButtonArray(&pause_buttons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2, screen.yres/2 + 100, 0xFFFE, false, true));

  pushButtonArray(&shop_buttons, initializeButton((xpm_map_t)QuitButtonHovered, (xpm_map_t)QuitButton, screen.xres/2, screen.yres/2 +300, 0xFFFE, false, true));

  pushButtonArray(&shop_buttons, initializeButton((xpm_map_t)Cannon, (xpm_map_t)Cannon, screen.xres/2 -300, screen.yres/2 -250, 0xFFFE, false, true));

  pushButtonArray(&shop_buttons, initializeButton((xpm_map_t)LaserGun, (xpm_map_t)LaserGun, screen.xres/2 -300, screen.yres/2 -100, 0xFFFE, false, true));

  hideButtons(&pause_buttons);
  hideButtons(&shop_buttons);

  game_background = create_spriteless_gameobject(0, 0, 0, 0, 0);
  pause_background = create_gameobject((xpm_map_t)PauseBackground, screen.xres/2, screen.yres/2, -300, -300, 0xFFF0, true, false);

  Sprite* shop_background_sprite = create_sprite((xpm_map_t)SelectGameBackground, screen.xres/2, screen.yres/2, true, false);

  shop_background = create_gameobject_from_sprite(shop_background_sprite, screen.xres/2, screen.yres/2, -(shop_background_sprite->width/2), -(shop_background_sprite->height/2), 0xFFF0);

  Sprite* tower_background_sprite = create_sprite((xpm_map_t)SelectGameBackground, screen.xres/2, screen.yres/2, true, false);

  tower_background = create_gameobject_from_sprite(tower_background_sprite, screen.xres/2, screen.yres/2, -(tower_background_sprite->width/2), -(tower_background_sprite->height/2), 0xFFF0);
}

void enterGame(bool multi, uint8_t arena) {
  resetDevicesChangingScreens();

  memcpy(unlocked_turrets, (uint8_t[]){1, 0, 0}, sizeof(unlocked_turrets));

  // Temporary but efficient fix. Should be replaced but works for now
  hideSprites(&player1->player->animatedSprite->sprites);
  hideSprites(&player2->player->animatedSprite->sprites);
  //

  showGameObjects(&money->moneyDigitsGameObjects);

  showWeapon(player_weapon);

  money->money_amount = 500;

  multiplayer = multi;
  current_arena = &arenas[arena];
  add_sprite_to_spriteless_gameobject(game_background, current_arena->background);
  playing = true;
  showSprites(&player1->player->animatedSprite->sprites);

  money->coin->sprite->is_visible = true;
  updateGameObjectSprites(money, 0);
  //showSprites(&money->moneyDigits);

  player_base = current_arena->base;
  updatePlayerBaseHealthBar(&player_base);
  shop = current_arena->shop;
  towers = current_arena->towers;

  showArena(current_arena);

  if (multi) showSprites(&player2->player->animatedSprite->sprites);
}

void updateGame() {
  if (rtc_time->just_updated && state == GAME) {
    if (to_spawn_enemy) {
      to_spawn_enemy = false;
      pushEnemyArray(&enemies, initializeEnemy((float)current_arena->spawn_x, (float)current_arena->spawn_y, 0, 0, multiplayer ? 1500 : 1000, current_arena->target_coordinates, current_arena->num_targets));
    } else {
      to_spawn_enemy = true;
    }
  }

  if (state == GAME) {
    updateGamePlay();
    updateAnimatedGameObjects();
  } else if (state == PAUSE) {
    updatePause();
  } else if (state == SHOP_MENU) {
    updateShop();
  } else if (state == TOWER_MENU) {
    updateTowerMenu();
  }
}

void exitGame() {
  playing = false;
  hideSprites(&player1->player->animatedSprite->sprites);
  hideSprites(&player2->player->animatedSprite->sprites);

  hideButtons(&pause_buttons);
  destroyEnemyArray(&enemies);

  money->coin->sprite->is_visible = false;
  hideGameObjects(&money->moneyDigitsGameObjects);
  //updateGameObjectSprites(money);

  hideWeapon(player_weapon);

  towers = (TowerArray){NULL, 0, 0};
  player_base = (PlayerBase){NULL, NULL, 0, 0};
  shop = (Shop){NULL, NULL};
  remove_sprite_from_spriteless_gameobject(game_background);
  pause_background->sprite->is_visible = false;

  hideArena(current_arena);
}

void destroyGame() {
  destroy_gameobject(game_background);
  destroyPlayer(player1);
  destroyPlayer(player2);
  destroyTurretArray(&towers);
  destroyArenas(arenas);
  destroyEnemyArray(&enemies);
  destroyButtonArray(&pause_buttons);
  destroyButtonArray(&shop_buttons);
  destroyButtonArray(&tower_buttons);

  //destroyBulletArray(&bullets)
  destroyBulletArray(&bullets);
}

