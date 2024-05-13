#include <lcom/lcf.h>

#include "keyboardkeys.h"

void addKeyPressAtEnd(KeyPresses** head, uint8_t key, bool special) {
  KeyPresses* newKeyPress = (KeyPresses*)malloc(sizeof(KeyPresses));
  newKeyPress->key = key;
  newKeyPress->special = special;
  newKeyPress->next = NULL;

  if (*head == NULL) {
      *head = newKeyPress;
      return;
  }

  KeyPresses* last = *head;
  while (last->next != NULL) {
      last = last->next;
  }

  last->next = newKeyPress;
}

uint16_t* processPressedKeys(KeyPresses** head) {

  if (*head == NULL) {
    return NULL;
  }
  
  uint16_t* keys = (uint16_t*)malloc(128 * sizeof(uint16_t));
  memset(keys, 0, 128 * sizeof(uint16_t));

  KeyPresses* current = *head;
  int i = 0;

  while (current != NULL) {
    keys[i] = current->key | (current->special ? 0x0100 : 0);
    KeyPresses* next = current->next;
    free(current);
    current = next;
    i++;
  }

  *head = NULL;
  return keys;
}
