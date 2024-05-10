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

void initializeMenu();
void enterMenu();
void updateMenu();
void exitMenu();
void destroyMenu();
