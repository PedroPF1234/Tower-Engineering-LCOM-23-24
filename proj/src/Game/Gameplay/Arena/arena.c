#include <lcom/lcf.h>
#include <stdint.h>

#include "arena.h"

#include "../../../ImageAssets/Background.xpm"

Arena* initializeArenas() {
  Arena* new_arena = (Arena*)malloc(sizeof(Arena) * 3);

  int16_t* first_targets = (int16_t*)malloc(sizeof(int16_t) * 10);
  int16_t* second_targets = (int16_t*)malloc(sizeof(int16_t) * 4);
  int16_t* third_targets = (int16_t*)malloc(sizeof(int16_t) * 8);

  first_targets[0] = 200;
  first_targets[1] = 900;

  first_targets[2] = 200;
  first_targets[3] = 200;

  first_targets[4] = 800;
  first_targets[5] = 200;

  first_targets[6] = 800;
  first_targets[7] = 900;

  first_targets[8] = 1100;
  first_targets[9] = 900;

  second_targets[0] = 700;
  second_targets[1] = 200;

  second_targets[2] = 700;
  second_targets[3] = 1000;

  third_targets[0] = 500;
  third_targets[1] = 500;

  third_targets[2] = 500;
  third_targets[3] = 200;

  third_targets[4] = 900;
  third_targets[5] = 200;

  third_targets[6] = 900;
  third_targets[7] = 1000;

  new_arena[0].background = create_sprite((xpm_map_t)Background, 0, 0, true, true);
  new_arena[0].coordinate_targets = first_targets;
  new_arena[0].num_targets = 5;
  new_arena[0].spawn_x = -50;
  new_arena[0].spawn_y = 900;

  new_arena[1].background = create_sprite((xpm_map_t)Background, 0, 0, true, true);
  new_arena[1].coordinate_targets = second_targets;
  new_arena[1].num_targets = 2;
  new_arena[1].spawn_x = -50;
  new_arena[1].spawn_y = 200;

  new_arena[2].background = create_sprite((xpm_map_t)Background, 0, 0, true, true);
  new_arena[2].coordinate_targets = third_targets;
  new_arena[2].num_targets = 4;
  new_arena[2].spawn_x = -50;
  new_arena[2].spawn_y = 500;

  return new_arena;
}

void destroyArenas(Arena* arena) {
  for (int i = 0; i < 3; i++) {
    destroy_sprite(arena[i].background);
    free(arena[i].coordinate_targets);
  }

  free(arena);
}
