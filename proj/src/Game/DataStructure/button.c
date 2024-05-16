#include <lcom/lcf.h>
#include <stdint.h>

#include "button.h"


// Set capacity to 0 for default capacity
ButtonArray newButtonArray(uint32_t capacity) {
  ButtonArray array;
  array.length = 0;

  if (capacity) {
    array.buttons = (Button**)malloc(capacity * sizeof(Button*));
    array.capacity = capacity;
  } else {
    array.buttons = (Button**)malloc(sizeof(Button*) * 10);
    array.capacity = 10;
  }
  return array;
}

void pushButtonArray(ButtonArray* array, Button* button) {

  if (array->capacity != array->length) {
        array->buttons[array->length] = button;
    } else {
      uint32_t newCapacity = array->capacity * 2;
      Button** oldPointer = array->buttons;
      Button** newPointer = (Button**)malloc(newCapacity * sizeof(Button*));
      array->buttons = newPointer;
      for (uint32_t i = 0; i < array->length; i++) {
        newPointer[i] = oldPointer[i];
      }
      free(oldPointer);
      array->buttons[array->length] = button;
    }
    array->length++;
}

Button* getButtonArray(ButtonArray* array, uint32_t index) {
    if (index < array->length) {
        return array->buttons[index];
    } else {
        return NULL;
    }
}

void removeButtonArray(ButtonArray* array, uint32_t index) {
    if (index < array->length) {
      Button* button = getButtonArray(array, index);
      destroy_sprite(button->hovering);
      destroy_sprite(button->no_hovering);
      button->button->sprite = NULL;
      destroy_gameobject(button->button);
      for (uint32_t i = index; i < array->length - 1; i++) {
          array->buttons[i] = array->buttons[i + 1];
      }
      array->length--;
    }
}

void destroyButtonArray(ButtonArray* array) {

    for (int32_t i = 0; i < (int32_t)array->length; i++) {
        Button* button = getButtonArray(array, i);
        removeButtonArray(array, i);
        free(button);
    }
}

Button* initializeButton(xpm_map_t hovered, xpm_map_t no_hovered, int16_t x, int16_t y,
                             int16_t ox, int16_t oy, uint16_t z, bool square) {

  Button* button = (Button*)malloc(sizeof(Button));
  Sprite* normal = create_sprite((xpm_map_t)no_hovered, x, y, false, true);
  Sprite* hoveredSprite = create_sprite((xpm_map_t)hovered, x, y, false, true);
  GameObject* buttonObject = create_gameobject_from_sprite(normal, x, y, ox, oy, z);

  button->hovering = hoveredSprite;
  button->no_hovering = normal;
  button->button = buttonObject;
  button->x = x;
  button->y = y;
  button->origin_offset_x = ox;
  button->origin_offset_y = oy;

  return button;
}

void hideButtons(ButtonArray* array) {
  for (int32_t i = 0; i < (int32_t)array->length; i++) {
    getButtonArray(array, i)->button->sprite->is_visible = false;
  }
}

void showButtons(ButtonArray* array) {
  for (int32_t i = 0; i < (int32_t)array->length; i++) {
    getButtonArray(array, i)->button->sprite->is_visible = true;
  }
}
