#pragma once

#include "../../Devices/device_controller.h"

/**
 * @brief Represents a button in the game.
 */
typedef struct Button {
  GameObject* button; /*The game object associated with the button*/
  Sprite* hovering; /*The sprite for the button when the cursor is hovering over it*/
  Sprite* no_hovering; /*The sprite for the button when the cursor is not hovering over it*/
  int16_t x, y; /*The x and y positions of the button*/
  int16_t origin_offset_x, origin_offset_y; /*The x and y offsets of the button's origin*/
} Button;

/**
 * @brief Represents an array of Buttons.
 */
typedef struct ButtonArray {
  Button** buttons; /*Pointer to an array of Button pointers*/
  uint32_t capacity; /*The total capacity of the array*/
  uint32_t length; /*The current number of Buttons in the array*/
} ButtonArray;

/**
 * @brief Initializes a new ButtonArray.
 *
 * @param capacity The initial capacity of the ButtonArray.
 * @return A new ButtonArray.
 */
ButtonArray newButtonArray(uint32_t capacity);

/**
 * @brief Adds a Button to a ButtonArray.
 *
 * @param array The ButtonArray to add the Button to.
 * @param button The Button to add.
 */
void pushButtonArray(ButtonArray* array, Button* button);

/**
 * @brief Retrieves a Button from a ButtonArray.
 *
 * @param array The ButtonArray to retrieve the Button from.
 * @param index The index of the Button to retrieve.
 * @return The Button at the specified index.
 */
Button* getButtonArray(ButtonArray* array, uint32_t index);

/**
 * @brief Removes a Button from a ButtonArray.
 *
 * @param array The ButtonArray to remove the Button from.
 * @param index The index of the Button to remove.
 */
void removeButtonArray(ButtonArray* array, uint32_t index);

/**
 * @brief Destroys a ButtonArray and frees associated memory.
 *
 * @param array The ButtonArray to destroy.
 */
void destroyButtonArray(ButtonArray* array);

/**
 * @brief Initializes a new Button.
 *
 * @param hovered The sprite for the button when the cursor is hovering over it.
 * @param no_hovered The sprite for the button when the cursor is not hovering over it.
 * @param x The x position of the button.
 * @param y The y position of the button.
 * @param z The z index of the button (its priority in sprite layers).
 * @param square Whether the button is square or not.
 * @param visible Whether the button is visible or not.
 * @return A new Button.
 */
Button* initializeButton(xpm_map_t hovered, xpm_map_t no_hovered, int16_t x, int16_t y,
                              uint16_t z, bool square, bool visible);


/**
 * @brief Turns off the visibility of all the Button's Sprites in a ButtonArray.
 * 
 * @param array The ButtonArray.
 */
void hideButtons(ButtonArray* array);

/**
 * @brief Turns on the visibility of all the Button's Sprites in a ButtonArray.
 * 
 * @param array The ButtonArray.
 */
void showButtons(ButtonArray* array);
