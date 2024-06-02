#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "towers.h"

#include "../../../ImageAssets/TowerBase.xpm"
#include "../../../ImageAssets/Towers.xpm"

Sprite* crossbowTurret;
Sprite* canonTurret;
Sprite* laserTurret;
int rotations = 0;

void initializeDifferentTowerSprites() {
  crossbowTurret = create_rotation_abled_sprite((xpm_map_t)Crossbow, 0, 0, false, false, 
  &rotations);
  canonTurret = create_rotation_abled_sprite((xpm_map_t)Cannon, 0, 0, false, false, &rotations);
  laserTurret = create_rotation_abled_sprite((xpm_map_t)LaserGun, 0, 0, false, false, &rotations);
}

TowerBase* initializeTower(int16_t x, int16_t y) {
  TowerBase* new_tower = (TowerBase*)malloc(sizeof(TowerBase));

  uint16_t z_indexing = y * Z_INDEX_PER_LAYER + MEDIUM_PRIORITY_Z_INDEX;

  Sprite* base = create_sprite((xpm_map_t)Base, x, y, false, false);
  Sprite* baseHovered = create_sprite((xpm_map_t)BaseHovered, x, y, false, true);

  new_tower->baseNormal = base;
  new_tower->baseHovered = baseHovered;

  new_tower->base = create_gameobject_from_sprite(new_tower->baseNormal, x, y, -(base->width/2), 
  -(base->height/2), z_indexing);
  new_tower->turret = create_spriteless_gameobject(x, y, -(base->width/2), -(base->height/2), z_indexing + 1);

  new_tower->turretType = -1;
  new_tower->x = x;
  new_tower->y = y;
  new_tower->origin_offset_x = -(base->width/2);
  new_tower->origin_offset_y = -(base->height/2);
  new_tower->turretSprite = NULL;
  new_tower->range = 0;
  new_tower->targetting = FIRST;
  new_tower->damage = 100;
  new_tower->turret_radius = 0;
  new_tower->target = NULL;
  new_tower->timeWithoutShooting = 0;
  new_tower->cooldown = 1; // test for cooldown
  new_tower->level = 0;


  return new_tower;
}

// Set capacity to 0 for default capacity
TowerArray newTowerArray(uint32_t capacity) {

  TowerArray new_array;
  new_array.length = 0;

  if (capacity) {
    new_array.towers = (TowerBase**)malloc(capacity * sizeof(TowerBase*));
    new_array.capacity = capacity;
  } else { // Default capacity
    new_array.towers = (TowerBase**)malloc(10 * sizeof(TowerBase*));
    new_array.capacity = 10;
  }

  return new_array;
}

void pushTowerArray(TowerArray* array, TowerBase* tower) {
  if (array->capacity != array->length) {
    array->towers[array->length] = tower;
  } else {
    uint32_t newCapacity = array->capacity * 2;
    TowerBase** oldPointer = array->towers;
    TowerBase** newPointer = (TowerBase**)malloc(newCapacity * sizeof(TowerBase*));
    array->towers = newPointer;
    for (uint32_t i = 0; i < array->length; i++) {
      newPointer[i] = oldPointer[i];
    }
    free(oldPointer);
    array->towers[array->length] = tower;
  }
  array->length++;
}

TowerBase* getTowerArray(TowerArray* array, uint32_t index) {
  if (index < array->length) {
    return array->towers[index];
  } else {
    return NULL;
  }
}

void removeTowerArray(TowerArray* array, uint32_t index) {
  if (index < array->length) {
    destroyTower(array->towers[index]);
    for (uint32_t i = index; i < array->length - 1; i++) {
        array->towers[i] = array->towers[i + 1];
    }
    array->length--;
  }
}

void destroyTurretArray(TowerArray* array) {

    for (uint32_t i = 0; i < array->length; i++) {
        destroyTower(array->towers[i]);
    }
    array->length = 0;
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

void mountTurret(TowerBase* tower, TurretType type) {

  Sprite* temp = NULL;
  int16_t x = tower->x + tower->origin_offset_x;
  int16_t y = tower->y + tower->origin_offset_y;

  switch (type) {
    case CROSSBOW:
      if (tower->turret->sprite == NULL) {
        temp = create_sprite_from_sprite(crossbowTurret, x, y, true, 360);
        tower->turret->origin_offset_x = -(temp->width / 2);
        tower->turret->origin_offset_y = -(temp->height / 2);
        tower->range =  temp[0].width*3;
        tower->turret_radius = ( temp[0].width / 2);
        tower->turretSprite = temp;
        tower->turretType = CROSSBOW;
      }
      add_sprite_to_spriteless_gameobject(tower->turret, temp);
      tower->damage = 50;

      break;

    case CANNON:
      printf("Cannon\n");
      if (tower->turret->sprite == NULL) {
        temp = create_sprite_from_sprite(canonTurret, x, y, true, 360);
        tower->turret->origin_offset_x = -(temp->width / 2);
        tower->turret->origin_offset_y = -(temp->height / 2);
        tower->turret_radius = (temp[0].width / 2);
        tower->range = temp[0].width*6;
        tower->turretSprite = temp;
        tower->turretType = CANNON;
      }
      add_sprite_to_spriteless_gameobject(tower->turret, temp);
      tower->damage = 100;

      break;

    case LASER:
      if (tower->turret->sprite == NULL) {
        temp = create_sprite_from_sprite(laserTurret, x, y, true, 360);
        tower->turret->origin_offset_x = -(temp->width / 2);
        tower->turret->origin_offset_y = -(temp->height / 2);
        tower->turret_radius = (temp[0].width / 2);
        tower->range = temp[0].width*6;
        tower->turretSprite = temp;
        tower->turretType = LASER;
      }
      add_sprite_to_spriteless_gameobject(tower->turret, temp);
      tower->damage = 200;
      tower->cooldown = 1;

      break;

    default:
      break;
  }
}

void unmountTurret(TowerBase* tower) {
  if (tower->turret->sprite != NULL) {
    destroy_sprite(tower->turret->sprite);
    remove_sprite_from_spriteless_gameobject(tower->turret);
    tower->target = NULL;
    tower->level = 0;
    tower->damage = 0;
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

void rotateTowersTowardsTarget(TowerArray* array, EnemyArray* enemies) {
  for (uint32_t i = 0; i < array->length; i++) {
    TowerBase* tower = getTowerArray(array, i);

    if (tower->turret->sprite != NULL) {

      Enemy* closest = NULL;
      int16_t distance_to_closest = 0;

      for (uint32_t j = 0; j < enemies->length; j++) {
        Enemy* temp = getEnemyArray(enemies, j);

        int16_t x_diff = temp->x - tower->x;
        int16_t y_diff = temp->y - tower->y;

        int16_t distance = (int16_t)(sqrt(x_diff * x_diff + y_diff * y_diff));

        if (distance > tower->range) {
          continue;
        } else {
          if (closest == NULL) {
            closest = temp;
            distance_to_closest = distance;
          } else {
            if (tower->targetting == FIRST) {
              break;
            } else if (tower->targetting == LAST) {
              closest = temp;
              distance_to_closest = distance;
            } else if (tower->targetting == CLOSEST) {
              if (distance < distance_to_closest) {
                closest = temp;
                distance_to_closest = distance;
              }
            }
          }
        }
      }

      tower->target = closest;

      if (closest == NULL) {
        continue;
      }

    
      int16_t turret_center_x = tower->x;
      int16_t turret_center_y = tower->y;
      int16_t x_target = closest->x;
      int16_t y_target = closest->y;

      int angle = calculate_angle(turret_center_x, turret_center_y, x_target, y_target);

      tower->turret->sprite = getSpriteFromAngle(tower->turretSprite, angle);
      tower->turret->origin_offset_x = -(tower->turret->sprite->width / 2);
      tower->turret->origin_offset_y = -(tower->turret->sprite->height / 2);
    }
  }
}
