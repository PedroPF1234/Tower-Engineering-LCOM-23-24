#include <lcom/lcf.h>
#include <stdint.h>

#include "arena.h"

#include "../../../ImageAssets/Arena.xpm"
#include "../../../ImageAssets/Decorations/Drill_Decor.xpm"
#include "../../../ImageAssets/Decorations/Factory_Decor.xpm"
#include "../../../ImageAssets/Decorations/Furnace_Decor.xpm"

AnimatedSprite furnaceDecor;
AnimatedSprite drillDecor;
AnimatedSprite factoryDecor;

static Arena* read_arena_info(char*** arena_info);

Arena* initializeArenas() {
  AnimatedSprite* temp1 = create_animated_sprite(Furnace_Decor, 5, 300);
  furnaceDecor = *temp1;
  
  AnimatedSprite* temp2 = create_animated_sprite(Drill_Decor, 3, 300);
  drillDecor = *temp2;

  AnimatedSprite* temp3 = create_animated_sprite(Factory_Decor, 4, 284);
  factoryDecor = *temp3;
  
  free(temp1);
  free(temp2);
  free(temp3);

  Arena* new_arenas = (Arena*)malloc(sizeof(Arena) * 3);

  char*** arena1_info[] = {Arena1Info};
  new_arenas[0] = *read_arena_info(arena1_info[0]);
  new_arenas[1] = *read_arena_info(arena1_info[0]);
  new_arenas[2] = *read_arena_info(arena1_info[0]);

  return new_arenas;
}

void destroyArenas(Arena* arena) {
  for (int i = 0; i < 3; i++) {
    destroy_sprite(arena[i].background);
    free(arena[i].targert_coordinates);
  }

  free(arena);
}

static Arena* read_arena_info(char*** arena_info) {
  Arena* new_arena = (Arena*)malloc(sizeof(Arena) * 3);
  uint16_t target_counter = 0;
  uint16_t decorations_counter = 0;
  uint16_t towers_counter = 0;

  Sprite* background = create_sprite((xpm_map_t)arena_info[0], 0, 0, true, true);
  new_arena->background = background;

  char** info = arena_info[1];
  uint16_t current_line = 0;

  char* header = info[current_line++];

  uint16_t pseudo_state = 0;
  uint16_t accumulator = 0;
  while (*header != '\0') {
    char character = *header;

    if (character == ' ') {
      switch (pseudo_state)
      {
      case 0:
        target_counter = accumulator;
        break;

      case 1:
        decorations_counter = accumulator;
        break;
      
      default:
        break;
      }
      accumulator = 0;
      pseudo_state++;
    } else if (character >= '0' && character <= '9') {
      accumulator = accumulator * 10 + (character - '0');
    } else {
      printf("Invalid character in header\n");
      return NULL;
    }
    header++;
  }

  towers_counter = accumulator;

  TowerArray towers = newTowerArray(towers_counter);
  new_arena->towers = towers;

  int16_t* target_coordinates = (int16_t*)malloc(sizeof(int16_t) * (target_counter - 1) * 2);

  new_arena->decorations = (AnimatedGameObject**)malloc(sizeof(AnimatedGameObject*) * decorations_counter);

  for (uint16_t i = 0; i < target_counter; i++) {
    bool negative = false;
    char* target = info[current_line++];
    int16_t target_x = 0;
    int16_t target_y = 0;
    while (*target != '\0') {
      char character = *target;

      if (character == ' ') {
        target_x = target_y;
        if (negative) {
          target_x *= -1;
          negative = false;
        }
        target_y = 0;
      } else if (character >= '0' && character <= '9') {
        target_y = target_y * 10 + (character - '0');
      } else if (character == '-') {
        negative = true;
      } else {
        printf("Invalid character in target\n");
        return NULL;
      }
      target++;
    }

    if (negative) {
      target_y *= -1;
      negative = false;
    }

    if (i != 0) {
      if (i == target_counter - 1) {
        new_arena->base_x = target_x;
        new_arena->base_y = target_y;
      }
      target_coordinates[(i - 1) * 2] = target_x;
      target_coordinates[(i - 1) * 2 + 1] = target_y;
    } else {
      new_arena->spawn_x = target_x;
      new_arena->spawn_y = target_y;
    }
  }
  
  new_arena->targert_coordinates = target_coordinates;
  new_arena->num_targets = target_counter - 1;

  for (uint16_t i = 0; i < decorations_counter; i++) {
    char* decoration = info[current_line++];
    int16_t decoration_x = 0;
    int16_t decoration_y = 0;
    int16_t decoration_id = -1;
    while (*decoration != '\0') {
      char character = *decoration;

      if (character == ' ') {
        if (decoration_id == -1) {
          decoration_id = decoration_y;
          decoration_y = 0;
        } else {
          decoration_x = decoration_y;
          decoration_y = 0;
        }
      } else if (character >= '0' && character <= '9') {
        decoration_y = decoration_y * 10 + (character - '0');
      } else {
        printf("Invalid character in decoration\n");
        return NULL;
      }
      decoration++;
    }

    switch (decoration_id)
    {
    case 0:
      new_arena->decorations[i] = create_animated_gameobject(&furnaceDecor, decoration_x, decoration_y, decoration_y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);
      break;

    case 1:
      new_arena->decorations[i] = create_animated_gameobject(&drillDecor, decoration_x, decoration_y, decoration_y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);
      break;

    case 2:
      new_arena->decorations[i] = create_animated_gameobject(&factoryDecor, decoration_x, decoration_y, decoration_y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);
      break;
    
    default:
      break;
    }
  }

  new_arena->num_decorations = decorations_counter;

  for (uint16_t i = 0; i < towers_counter; i++) {
    char* tower = info[current_line++];
    int16_t tower_x = 0;
    int16_t tower_y = 0;
    while (*tower != '\0') {
      char character = *tower;

      if (character == ' ') {
        tower_x = tower_y;
        tower_y = 0;
      } else if (character >= '0' && character <= '9') {
        tower_y = tower_y * 10 + (character - '0');
      } else {
        printf("Invalid character in tower\n");
        return NULL;
      }
      tower++;
    }

    TowerBase* new_tower = initializeTower(tower_x, tower_y);
    pushTowerArray(&new_arena->towers, new_tower);
  }

  hideTowers(&new_arena->towers);

  new_arena->num_towers = towers_counter;

  char* shop_coordinates = info[current_line++];
  int16_t shop_x = 0;
  int16_t shop_y = 0;

  while (*shop_coordinates != '\0') {
    char character = *shop_coordinates;

    if (character == ' ') {
      shop_x = shop_y;
      shop_y = 0;
    } else if (character >= '0' && character <= '9') {
      shop_y = shop_y * 10 + (character - '0');
    } else {
      printf("Invalid character in coordinates\n");
      return NULL;
    }
    shop_coordinates++;
  }
  new_arena->shop_x = shop_x;
  new_arena->shop_y = shop_y;

  return new_arena;
}
