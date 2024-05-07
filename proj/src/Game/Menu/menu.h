#pragma once

#include "../../Devices/device_controller.h"

typedef struct Button {
  GameObject* hovering;
  GameObject* no_hovering;
  int16_t x, y;
  int16_t origin_offset_x, origin_offset_y;
} Button;

typedef struct MenuNode {
    Button* button;
    struct MenuNode* next;
} MenuNode;

void insertAtEndMenu(MenuNode** head, Button* button);
void deleteNodeMenu(MenuNode** head, Button* button);
void deleteListMenu(MenuNode** head);
Button* initializeMenuButton(xpm_map_t hovered, xpm_map_t no_hovered, int16_t x, int16_t y,
                             int16_t ox, int16_t oy, uint16_t z, bool square);
void hideMenuButtons(MenuNode** head);
void showMenuButtons(MenuNode** head);
