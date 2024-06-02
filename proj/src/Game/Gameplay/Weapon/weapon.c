#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "weapon.h"

#include "../../../ImageAssets/Weapon.xpm"

Sprite* rotatedSpriteL;
Sprite* rotatedSpriteR;
static int rotationsL = 0;
static int rotationsR = 0;


Weapon* initializeWeapon(int16_t x, int16_t y) {
  Weapon* new_weapon = (Weapon*)malloc(sizeof(Weapon));

  rotatedSpriteL = create_rotation_abled_sprite((xpm_map_t)Weapon_L, 0, 0, false, true, &rotationsL);

  rotatedSpriteR = create_rotation_abled_sprite((xpm_map_t)Weapon_R, 0, 0, false, true, &rotationsR);

  Sprite* spriteL = /*create_sprite((xpm_map_t)Weapon_L, x, y, false, false);*/ getSpriteFromAngle(rotatedSpriteL, 0);
  Sprite* spriteR = /*create_sprite((xpm_map_t)Weapon_R, x, y, false, false);*/ getSpriteFromAngle(rotatedSpriteR, 0);

  new_weapon->weaponL = create_gameobject_from_sprite(spriteL, x, y, -(spriteL->width/2), -(spriteL->height/2), HIGH_PRIORITY_Z_INDEX);

  new_weapon->weaponR = create_gameobject_from_sprite(spriteR, x, y, -(spriteR->width/2), -(spriteR->height/2), HIGH_PRIORITY_Z_INDEX);

  return new_weapon;
}

void destroyWeapon(Weapon* weapon) {
  destroy_gameobject(weapon->weaponL);
  destroy_gameobject(weapon->weaponR);
  free(weapon);
}

void showWeapon(Weapon* weapon) {
  weapon->weaponL->sprite->is_visible = true;
  weapon->weaponR->sprite->is_visible = true;
}

void hideWeapon(Weapon* weapon) {
  weapon->weaponL->sprite->is_visible = false;
  weapon->weaponR->sprite->is_visible = false;
}

void updateWeapon(Weapon* weapon, Player* player, int16_t mouse_x, int16_t mouse_y) {
  //Calculate the z_index of the weapon (wether it will appear in front or behind the player sprite)
  int z_index = player->player->gameObject->y * Z_INDEX_PER_LAYER;
  Direction direction = player->current_direction;

  if (direction == UP || direction == UP_LEFT || direction == UP_RIGHT || direction == UP_IDLE) {
    z_index += LOW_PRIORITY_Z_INDEX;
  } else {
    z_index += HIGH_PRIORITY_Z_INDEX;
  }
  
  //Calculate the angle between the player and the mouse
  int angle = calculate_angle((float)player->player->gameObject->x, 
  (float)player->player->gameObject->y + player->origin_offset_y, mouse_x, mouse_y);
  
  if (mouse_x > player->x) { //If the mouse is to the right of the player, show the right weapon sprite
    weapon->weaponL->sprite->is_visible = false;
    weapon->weaponR->sprite->is_visible = true;

    weapon->weaponR->sprite = getSpriteFromAngle(rotatedSpriteR, angle);

    weapon->weaponR->x = (int16_t) player->x + player->origin_offset_x + 20;
    weapon->weaponR->y = (int16_t) player->y + player->origin_offset_y;

    weapon->weaponR->origin_offset_x = -(weapon->weaponR->sprite->width/2);
    weapon->weaponR->origin_offset_y = -(weapon->weaponR->sprite->height/2);

    updateGameObjectZIndex(weapon->weaponR, z_index);
  } 
  else { //If the mouse is to the left of the player, show the left weapon sprite
    weapon->weaponL->sprite->is_visible = true;
    weapon->weaponR->sprite->is_visible = false;

    weapon->weaponL->sprite = getSpriteFromAngle(rotatedSpriteL, angle + 180);

    weapon->weaponL->x = (int16_t) player->x + player->origin_offset_x + 20;
    weapon->weaponL->y = (int16_t) player->y + player->origin_offset_y;

    weapon->weaponL->origin_offset_x = -(weapon->weaponL->sprite->width/2);
    weapon->weaponL->origin_offset_y = -(weapon->weaponL->sprite->height/2);

    updateGameObjectZIndex(weapon->weaponL, z_index);
  }
}
