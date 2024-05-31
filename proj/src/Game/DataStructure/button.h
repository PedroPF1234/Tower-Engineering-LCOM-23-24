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

ButtonArray newButtonArray(uint32_t capacity);
void pushButtonArray(ButtonArray* array, Button* button);
Button* getButtonArray(ButtonArray* array, uint32_t index);
void removeButtonArray(ButtonArray* array, uint32_t index);
void destroyButtonArray(ButtonArray* array);
Button* initializeButton(xpm_map_t hovered, xpm_map_t no_hovered, int16_t x, int16_t y,
                              uint16_t z, bool square, bool visible);
void hideButtons(ButtonArray* array);
void showButtons(ButtonArray* array);
