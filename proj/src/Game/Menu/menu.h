#pragma once

#include "../../Devices/device_controller.h"

typedef struct Button {
  GameObject* button;
  Sprite* hovering;
  Sprite* no_hovering;
  int16_t x, y;
  int16_t origin_offset_x, origin_offset_y;
} Button;

typedef struct ButtonArray {
    Button** buttons;
    uint32_t capacity;
    uint32_t length;
} ButtonArray;

void initializeMenu();
void enterMenu();
void updateMenu();
void exitMenu();
void destroyMenu();
