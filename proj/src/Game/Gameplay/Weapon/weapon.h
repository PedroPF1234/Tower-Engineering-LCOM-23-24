#include "../../../GameObjects/gameobject.h"
#include "../../Gameplay/Player/player.h"

typedef struct Weapon
{
  GameObject* weaponL;
  GameObject* weaponR;
  //uint16_t damage; 
  //uint16_t range;
  //uint16_t max_bullets;
  //uint16_t current_bullets; //Remove comment if implemented
} Weapon;

Weapon* initializeWeapon(int16_t x, int16_t y);
void destroyWeapon(Weapon* weapon);
void updateWeapon(Weapon* weapon, Player* player, int16_t mouse_x, int16_t mouse_y);

void showWeapon(Weapon* weapon);
void hideWeapon(Weapon* weapon);
