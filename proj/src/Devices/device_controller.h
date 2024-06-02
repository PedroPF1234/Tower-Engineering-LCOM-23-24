#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

#include "../Devices/KeyBoard/keyboardkeys.h"
#include "../Devices/RTC/rtc_time.h"

#include "../GameObjects/gameobject.h"
#include "../ImageAssets/sprite.h"

/**
 * @brief Struct that represents the mouse device
 * 
 */
typedef struct MouseDevice {
  GameObject* mouse; // GameObject that represents the mouse
  bool left_button_is_pressed; // If the left button is pressed
  bool left_button_released; // If the left button was released
  bool right_button_is_pressed; // If the right button is pressed
  bool right_button_released; // If the right button was released
  bool middle_button_is_pressed; // If the middle button is pressed
  bool middle_button_released; // If the middle button was released
  bool mouse_just_updated; // If any mouse button just updated
} MouseDevice;

/**
 * @brief Struct that represents the Keyboard device
 * 
 */
typedef struct KeyboardDevice {
  KeyPresses* keyPresses; // Head of linked list of key presses
} KeyboardDevice;

/**
 * @brief Struct that represents the Screen resolution
 * 
 */
typedef struct ScreenInfo {
  uint16_t xres, yres; // Resolution of the screen
} ScreenInfo;

/**
 * @brief Encapsulates the initialization of all the devices
 * 
 * @param freq frequency of the timer
 * @param framespersecond frames per second of the graphics
 * @param mode mode to set the graphics to
 * @return 0 if successful, 1 otherwise
 */
int boot_devices(uint32_t freq, uint16_t framespersecond, uint16_t mode);

/**
 * @brief Encapsulates the termination of all the devices
 * 
 * @return 0 if successful, 1 otherwise
 */
int stop_devices();

/**
 * @brief Calls all device's interrupt handlers
 * 
 * @param interrupt_mask mask of the interrupts
 * @return 0 if successful, 1 otherwise
 */
int interrupt_handler(uint32_t interrupt_mask);

/**
 * @brief Updates the frequency of the timer
 * 
 * @param freq frequency of the timer
 * @return 0 if successful, 1 otherwise
 */
int update_timer_freq(uint32_t freq);

/**
 * @brief Fetches the screen resolution and stores that information in the ScreenInfo struct
 * 
 * @return ScreenInfo struct with the screen resolution
 */
ScreenInfo getScreenInfo();

/**
 * @brief Resets the devices when changing screens
 * 
 */
void resetDevicesChangingScreens();
