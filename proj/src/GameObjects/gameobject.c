#include <lcom/lcf.h>
#include <stdint.h>

#include "gameobject.h"

#include "../ImageAssets/MouseCursor.xpm"
#include "../ImageAssets/Background.xpm"

Node *head = NULL;


GameObject* mouse;
GameObject* background;

static void insertSorted(Node **head, GameObject *newGameObject) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->gameObject = newGameObject;
    newNode->next = NULL;

    // Special case for the head end
    if (*head == NULL || (*head)->gameObject->sprite->z_index >= newGameObject->sprite->z_index) {
        newNode->next = *head;
        *head = newNode;
    } else {
        // Locate the node before the point of insertion
        Node *current = *head;
        while (current->next != NULL && current->next->gameObject->sprite->z_index < newGameObject->sprite->z_index) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

static void deleteNode(Node **head, GameObject *gameObject) {
    Node *temp = *head, *prev = NULL;

    // If the node to be deleted is the head node
    if (temp != NULL && temp->gameObject == gameObject) {
        *head = temp->next;
        free(temp);
        return;
    }

    // Find the node to be deleted
    while (temp != NULL && temp->gameObject != gameObject) {
        prev = temp;
        temp = temp->next;
    }

    // If not found
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp);
}

GameObject* create_gameobject(xpm_map_t pic, uint16_t x, uint16_t y, uint16_t origin_offset_x, uint16_t origin_offset_y, uint16_t z_index, bool square_shape) {

  GameObject* gameObject = (GameObject*) malloc(sizeof(GameObject));
  Sprite* sprite = create_sprite(pic, x, y, z_index, square_shape);

  gameObject->sprite = sprite;
  gameObject->x = x;
  gameObject->y = y;
  gameObject->origin_offset_x = origin_offset_x;
  gameObject->origin_offset_y = origin_offset_y;

  insertSorted(&head, gameObject);

  return gameObject;
}

void destroy_gameobject(GameObject* gameObject) {
  deleteNode(&head, gameObject);
  destroy_sprite(gameObject->sprite);
  free(gameObject);
}

static int draw_gameObject(GameObject* gameObject) {
  gameObject->sprite->x = gameObject->x + gameObject->origin_offset_x;
  gameObject->sprite->y = gameObject->y + gameObject->origin_offset_y;

  return draw_sprite(gameObject->sprite);
}

void renderGameObjects() {
  Node *current = head;
  while (current != NULL) {
    draw_gameObject(current->gameObject);
    current = current->next;
  }
}

int create_gameobjects() {
  mouse = create_gameobject((xpm_map_t) MouseCursor, 0, 0, 0, 0, BIT_MASK(16), false);
  background = create_gameobject((xpm_map_t) Background, 0, 0, 0, 0, 0, true);

  return 0;
}

int destroy_gameobjects() {
  destroy_gameobject(mouse);
  destroy_gameobject(background);

  return 0;
}

