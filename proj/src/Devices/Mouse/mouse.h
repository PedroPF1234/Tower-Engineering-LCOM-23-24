#pragma once

#include<stdint.h>

/**
 * @brief Subscribes and enables Mouse interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 if successful, 1 otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Mouse interrupts
 * 
 * @return 0 if successful, 1 otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Writes a command to the Mouse
 * 
 * @param cmd command to write
 * @return 0 if successful, 1 otherwise
 */
int (mouse_write_cmdb)(uint8_t cmd);

/**
 * @brief Reads a command from the Mouse
 * 
 * @param info address of memory to be initialized with the command read
 * @return 0 if successful, 1 otherwise
 */
int (mouse_read_cmdb)(uint8_t *info);

/**
 * @brief Mouse Interrupt Handler
 * 
 */
void (mouse_ih)();

/**
 * @brief Mouse Event Handler
 * 
 * @param pp packet to be processed
 * @param x_len length of the x movement
 * @param tolerance tolerance of the movement
 * @return 0 if successful, 1 otherwise
 */
bool mouse_gesture_event(struct packet* pp, uint8_t x_len, uint8_t tolerance);

/**
 * @brief Fetches the information of the mouse
 * 
 * @param target packet to be processed
 */
int mouse_get_info(struct packet *target);

/**
 * @brief Sets the mouse in remote mode
 * 
 * @param remote_byte byte to be sent to the mouse
 * @return 0 if successful, 1 otherwise
 */
int mouse_remote(uint8_t remote_byte);

/**
 * @brief Encapsulates the subscription and initialization of the mouse
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 if successful, 1 otherwise
 */
int mouse_initiate(uint8_t *bit_no);

/**
 * @brief Encapsulates the unsubscription and termination of the mouse
 * 
 * @return 0 if successful, 1 otherwise
 */
int mouse_terminate();
