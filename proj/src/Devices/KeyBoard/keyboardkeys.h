#pragma once

#include <stdint.h>

// Keyboard Scancodes
#define ESC_MAKE 0x01 // Make Code for ESC Key
#define ESC_BREAK 0x81 // Break Code for ESC Key

#define UP_ARROW_MAKE 0x48 // Make Code for UP Arrow Key
#define UP_ARROW_BREAK 0xC8 // Break Code for UP Arrow Key
#define DOWN_ARROW_MAKE 0x50 // Make Code for DOWN Arrow Key
#define DOWN_ARROW_BREAK 0xD0 // Break Code for DOWN Arrow Key
#define LEFT_ARROW_MAKE 0x4B // Make Code for LEFT Arrow Key
#define LEFT_ARROW_BREAK 0xCB // Break Code for LEFT Arrow Key
#define RIGHT_ARROW_MAKE 0x4D // Make Code for RIGHT Arrow Key
#define RIGHT_ARROW_BREAK 0xCD // Break Code for RIGHT Arrow Key

#define ENTER_MAKE 0x1C // Make Code for ENTER Key
#define ENTER_BREAK 0x9C // Break Code for ENTER Key

#define SPACE_MAKE 0x39 // Make Code for SPACE Key
#define SPACE_BREAK 0xB9 // Break Code for SPACE Key

typedef struct KeyPresses {
  uint8_t key;
  bool special;
  struct KeyPresses* next;
} KeyPresses;

void addKeyPressAtEnd(KeyPresses** head, uint8_t key, bool special);
uint16_t* processPressedKeys(KeyPresses** head);
