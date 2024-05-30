#include <lcom/lcf.h>
#include <stdint.h>

#include "playerbase.h"

#include "../../../ImageAssets/Base.xpm"

#define BASE_RED_HEALTH_COLOR {0x00, 0x00, 0xFF};
#define BASE_GREEN_HEALTH_COLOR {0x49, 0xFF, 0x15};

PlayerBase* initializePlayerBase(int16_t x, int16_t y,int16_t hp) {
  PlayerBase* new_playerbase = (PlayerBase*)malloc(sizeof(PlayerBase));

  Sprite* base = create_sprite((xpm_map_t)Base, x, y, false, true);
  new_playerbase->baseObject = create_gameobject_from_sprite(base, x, y, -(base->width/2), -(base->height/2), y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);

  Sprite* healthBar = create_sprite((xpm_map_t)BigHealthBar, x, y, false, true);
  new_playerbase->health_bar = create_gameobject_from_sprite(healthBar, x, y, -(healthBar->width/2), -(base->height/2) - (base->height/2) - 20, y * Z_INDEX_PER_LAYER + HIGH_PRIORITY_Z_INDEX);

  new_playerbase->hit_points = hp;
  new_playerbase->max_hit_points = hp;

  return new_playerbase;
}

void destroyPlayerBase(PlayerBase* playerbase) {
  destroy_gameobject(playerbase->baseObject);
  destroy_gameobject(playerbase->health_bar);
  free(playerbase);
}

void updatePlayerBaseHealthBar(PlayerBase* playerbase) {
  if (playerbase->hit_points != playerbase->max_hit_points) {
    if (playerbase->hit_points < 0) playerbase->hit_points = 0;
    int16_t healt_width = playerbase->health_bar->sprite->width;
    int16_t health_height = playerbase->health_bar->sprite->height;

    int16_t health_current_width = (healt_width * playerbase->hit_points) / playerbase->max_hit_points;

    Sprite* health_bar = playerbase->health_bar->sprite;

    for (int i = 0; i < health_current_width; i++) {
      for (int j = 0; j < health_height; j++) {
        uint8_t color[3] = BASE_GREEN_HEALTH_COLOR;
        memcpy(health_bar->map + (i + j * healt_width) * 3, &color, 3);
      }
    }

    for (int i = health_current_width; i < healt_width; i++) {
      for (int j = 0; j < health_height; j++) {
        uint8_t color[3] = BASE_RED_HEALTH_COLOR;
        memcpy(health_bar->map + (i + j * healt_width) * 3, color, 3);
      }
    }
  }
}
