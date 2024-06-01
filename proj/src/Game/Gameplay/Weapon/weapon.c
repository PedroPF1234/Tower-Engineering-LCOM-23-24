#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "weapon.h"

#include "../../../ImageAssets/Weapon.xpm"


Weapon* initializeWeapon(int16_t x, int16_t y) {
  Weapon* new_weapon = (Weapon*)malloc(sizeof(Weapon));

  Sprite* spriteL = create_sprite((xpm_map_t)Weapon_L, x, y, false, false);
  Sprite* spriteR = create_sprite((xpm_map_t)Weapon_R, x, y, false, false);

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
  if (mouse_x > player->x) { //If the mouse is to the right of the player, show the right weapon sprite
    weapon->weaponL->sprite->is_visible = false;
    weapon->weaponR->sprite->is_visible = true;

    weapon->weaponR->x = (int16_t) player->x + player->origin_offset_x + 10;
    weapon->weaponR->y = (int16_t) player->y + player->origin_offset_y;
  } 
  else { //If the mouse is to the left of the player, show the left weapon sprite
    weapon->weaponL->sprite->is_visible = true;
    weapon->weaponR->sprite->is_visible = false;

    weapon->weaponL->x = (int16_t) player->x + player->origin_offset_x + 10;
    weapon->weaponL->y = (int16_t) player->y + player->origin_offset_y;
  }


  //Calculate the angle between the player and the mouse
  if (mouse_x == player->x) {
    if (mouse_y > player->y) {
      //angle = 90 degrees
    } else {
      //angle = -90 degrees
    }
  } else {
    //double angle = atan2((double) mouse_y - player->y, (double) mouse_x - player->x);
  }

  //Rotate the weapon sprite
  
}
