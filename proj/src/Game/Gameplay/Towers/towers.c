#include <lcom/lcf.h>
#include <stdint.h>

#include "towers.h"

#include "../../../ImageAssets/TowerBase.xpm"
#include "../../../ImageAssets/Towers.xpm"

TowerBase* initializeTower(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp) {
  TowerBase* new_tower = (TowerBase*)malloc(sizeof(TowerBase));

  new_tower->baseNormal = create_sprite((xpm_map_t)CrossbowBase, x, y, 1000, false, false);
  new_tower->baseHovered = create_sprite((xpm_map_t)CrossbowBaseHovered, x, y, 1000, false, true);
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

void addTowerToList(TowerNode** head, TowerBase* tower) {
    TowerNode* new_node = (TowerNode*)malloc(sizeof(TowerNode));
    new_node->tower = tower;
    new_node->next = *head;
    *head = new_node;
}

void deleteListGame(TowerNode** head) {
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

void destroyTower(TowerBase* tower) {
  destroy_sprite(tower->baseNormal);
  destroy_sprite(tower->baseHovered);
  // Destroy sprites for tower turrets.
  destroy_gameobject_after_sprite_destroyed(tower->base);
  // Destroy gameobjects for tower turrets.
  free(tower);
}
