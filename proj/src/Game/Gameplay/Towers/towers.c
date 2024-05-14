#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "towers.h"

#include "../../../ImageAssets/TowerBase.xpm"
#include "../../../ImageAssets/Towers.xpm"

Sprite* crossbowTurret;
Sprite* canonTurret;
int rotations = 0;

void initializeDifferentTowerSprited() {
  crossbowTurret = create_rotation_abled_sprite((xpm_map_t)Crossbow, 0, 0, false, false, 
  &rotations);
  canonTurret = create_rotation_abled_sprite((xpm_map_t)Cannon, 0, 0, false, false, &rotations);
}

TowerBase* initializeTower(int16_t x, int16_t y, int16_t ox, int16_t oy, int16_t hp) {
  TowerBase* new_tower = (TowerBase*)malloc(sizeof(TowerBase));

  uint16_t z_indexing = y * Z_INDEX_PER_LAYER + MEDIUM_PRIORITY_Z_INDEX;

  new_tower->baseNormal = create_sprite((xpm_map_t)CrossbowBase, x - ox, y - oy, false, false);
  new_tower->baseHovered = create_sprite((xpm_map_t)CrossbowBaseHovered, x - ox, y - oy, false, true);

  new_tower->base = create_gameobject_from_sprite(new_tower->baseNormal, x, y, ox, oy, z_indexing);
  new_tower->turret = create_spriteless_gameobject(x, y, ox, oy, z_indexing + 1);

  new_tower->x = x;
  new_tower->y = y;
  new_tower->origin_offset_x = ox;
  new_tower->origin_offset_y = oy;
  new_tower->hit_points = hp;
  new_tower->turretSprite = NULL;
  new_tower->range = 0;

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
        destroyTower(&array->towers[index]);
        for (uint32_t i = index; i < array->length - 1; i++) {
            array->towers[i] = array->towers[i + 1];
        }
        memset(&array->towers[array->length - 1], 0, sizeof(TowerBase));
        array->length--;
    }
}

void destroyArray(TowerArray* array) {

    for (uint32_t i = 0; i < array->length; i++) {
        destroyTower(&array->towers[i]);
    }
    array->length = 0;

    memset(array->towers, 0, array->capacity * sizeof(TowerBase));
}

void destroyTower(TowerBase* tower) {
  destroy_sprite(tower->baseNormal);
  destroy_sprite(tower->baseHovered);
  if (tower->turret->sprite != NULL)
    destroy_sprite(tower->turret->sprite);
  tower->base->sprite = NULL;
  destroy_gameobject(tower->base);
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

void mountTowers(TowerBase* tower, uint32_t type) {
  Sprite* temp = NULL;
  int16_t x = tower->x + tower->origin_offset_x;
  int16_t y = tower->y + tower->origin_offset_y;

  switch (type) {
    case 0:
      if (tower->turret->sprite == NULL) {
        temp = create_sprite_from_sprite(crossbowTurret, x, y, true, 360);
        tower->turret->origin_offset_x = -(temp->width / 2);
        tower->turret->origin_offset_y = -(temp->height / 2);
        tower->turretSprite = temp;
      }
      add_sprite_to_spriteless_gameobject(tower->turret, temp);
      tower->range = 100;

      break;
    case 1:
      if (tower->turret->sprite == NULL) {
        temp = create_sprite_from_sprite(canonTurret, x, y, true, 360);
        tower->turret->origin_offset_x = -(temp->width / 2);
        tower->turret->origin_offset_y = -(temp->height / 2);
        tower->turretSprite = temp;
      }
      add_sprite_to_spriteless_gameobject(tower->turret, temp);
      tower->range = 200;
      break;
    default:
      break;
  }
}

void hideTowers(TowerArray* array) {
  for (uint32_t i = 0; i < array->length; i++) {
    TowerBase* tower = getTowerArray(array, i);
    tower->base->sprite->is_visible = false;
    if (tower->turret->sprite != NULL)
      free(tower->turret->sprite);
      remove_sprite_from_spriteless_gameobject(tower->turret);
  }
}

void showTowers(TowerArray* array) {
  for (uint32_t i = 0; i < array->length; i++) {
    TowerBase* tower = getTowerArray(array, i);
    tower->base->sprite->is_visible = true;
  }
}

void rotateTowersTowardsTarget(TowerArray* array, GameObject* target) {
  for (uint32_t i = 0; i < array->length; i++) {
    TowerBase* tower = getTowerArray(array, i);
    if (tower->turret->sprite != NULL) {
      int16_t turret_center_x = tower->x;
      int16_t turret_center_y = tower->y;
      int16_t x_target = target->x;
      int16_t y_target = target->y;
      int16_t x_diff = x_target - turret_center_x;
      int16_t y_diff = y_target - turret_center_y;

      int16_t distance = (int16_t)(sqrt(x_diff * x_diff + y_diff * y_diff));

      if (distance > tower->range) {
        continue;
      }

      int angle = (int)((atan2(y_diff, x_diff) * 180 / M_PI));
      
      if (angle < 0) {
        angle = 360 - abs(angle);
      }
      
      tower->turret->sprite = &tower->turretSprite[angle];
      tower->turret->origin_offset_x = -(tower->turret->sprite->width / 2);
      tower->turret->origin_offset_y = -(tower->turret->sprite->height / 2);
    }
  }
}
