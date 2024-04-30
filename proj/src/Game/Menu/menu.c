#include <lcom/lcf.h>
#include <stdint.h>

#include "menu.h"

void insertAtEndMenu(MenuNode** head, Button* button) {

  MenuNode* newNode = (MenuNode*)malloc(sizeof(MenuNode));

  newNode->button = button;
  newNode->next = NULL;

  MenuNode* current = *head;

  if (*head == NULL) {
    *head = newNode;
    return;
  }

  while (current->next != NULL) {
    current = current->next;  
  }

  current->next = newNode;
}

void deleteNodeMenu(MenuNode** head, Button* button) {
    MenuNode *temp = *head, *prev = NULL;

    // If the node to be deleted is the head node
    if (temp != NULL && temp->button == button) {
        *head = temp->next;
        destroy_gameobject(temp->button->hovering);
        destroy_gameobject(temp->button->no_hovering);
        free(temp->button);
        free(temp);
        return;
    }

    // Find the node to be deleted
    while (temp != NULL && temp->button != button) {
        prev = temp;
        temp = temp->next;
    }

    // If not found
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp);
}

void deleteListMenu(MenuNode** head) {
    MenuNode* current = *head;
    MenuNode* next;

    while (current != NULL) {
        next = current->next;
        destroy_gameobject(current->button->hovering);
        destroy_gameobject(current->button->no_hovering);
        free(current->button);
        free(current);
        current = next;
    }

    *head = NULL;
}

Button* initializeMenuButton(xpm_map_t hovered, xpm_map_t no_hovered, int16_t x, int16_t y,
                             int16_t ox, int16_t oy, uint16_t z, bool square) {

  Button* button = (Button*)malloc(sizeof(Button));
  GameObject* hoveredObject = create_gameobject((xpm_map_t)hovered, x, y, ox, oy, z, square, false);
  GameObject* noHovered = create_gameobject((xpm_map_t)no_hovered, x, y, ox, oy, z, square, true);

  button->hovering = hoveredObject;
  button->no_hovering = noHovered;
  button->x = x;
  button->y = y;
  button->origin_offset_x = ox;
  button->origin_offset_y = oy;

  return button;
}
