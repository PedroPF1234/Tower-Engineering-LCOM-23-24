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

TowerArray newTowerArray(uint32_t capacity) {

  TowerArray new_array;
  new_array.length = 0;

  if (capacity) {
    new_array.towers = (TowerBase*)malloc(capacity * sizeof(TowerBase));
    new_array.capacity = capacity;
  } else { // Default capacity
    new_array.towers = (TowerBase*)malloc(10 * sizeof(TowerBase));
    new_array.capacity = 10;
  }

  return new_array;
}

void pushTowerArray(TowerArray* array, TowerBase* tower) {
    if (array->capacity != array->length) {
        array->towers[array->length] = *tower;
    } else {
      uint32_t newCapacity = array->capacity * 2;
      TowerBase* oldPointer = array->towers;
      TowerBase* newPointer = (TowerBase*)malloc(newCapacity * sizeof(TowerBase));
      array->towers = newPointer;
      for (uint32_t i = 0; i < array->length; i++) {
        newPointer[i] = oldPointer[i];
      }
      free(oldPointer);
      array->towers[array->length] = *tower;
    }
    array->length++;
}

TowerBase* getTowerArray(TowerArray* array, uint32_t index) {
    if (index < array->length) {
        return &array->towers[index];
    } else {
        return NULL;
    }
}

void removeTowerArray(TowerArray* array, uint32_t index) {
    if (index < array->length) {
        for (uint32_t i = index; i < array->length - 1; i++) {
            array->towers[i] = array->towers[i + 1];
        }
        memset(&array->towers[array->length - 1], 0, sizeof(TowerBase));
        array->length--;
    }
}

void emptyArray(TowerArray* array) {

    for (uint32_t i = 0; i < array->length; i++) {
        destroyTower(&array->towers[i]);
    }
    array->length = 0;

    memset(array->towers, 0, array->capacity * sizeof(TowerBase));
}

void destroyTower(TowerBase* tower) {
  destroy_sprite(tower->baseNormal);
  destroy_sprite(tower->baseHovered);
  // Destroy sprites for tower turrets.
  destroy_gameobject_after_sprite_destroyed(tower->base);
  // Destroy gameobjects for tower turrets.
  free(tower);
}

void setTowerHovered(TowerBase* tower, bool hovered) {
  if (hovered) {
    updateGameObjectSprite(tower->base, tower->baseHovered);
  } else {
    updateGameObjectSprite(tower->base, tower->baseNormal);
  }
}
