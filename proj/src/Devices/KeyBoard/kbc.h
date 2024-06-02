#pragma once

#include<stdint.h>

/**
 * @brief Subscribes and enables KBC interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 if successful, 1 otherwise
 */
int (kbc_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes KBC interrupts
 * 
 * @return 0 if successful, 1 otherwise
 */
int (kbc_unsubscribe_int)();

/**
 * @brief Reenables KBC interrupts
 * 
 * @return 0 if successful, 1 otherwise
 */
int (kbd_reenable_interrupts)();

/**
 * @brief KBC Interrupt Handler
 * 
 */
void (kbc_ih)();

/**
 * @brief Writes a command to the KBC
 * 
 * @param port port to write the command to
 * @param cmd command to write
 * @param is_mouse_cmd if the command is a mouse command
 * @return 0 if successful, 1 otherwise
 */
int (kbc_write_cmdb)(uint8_t port, uint8_t cmd, bool is_mouse_cmd);

/**
 * @brief Reads a command from the KBC
 * 
 * @param cmd address of memory to be initialized with the command read
 * @param is_mouse_cmd if the command is a mouse command
 * @return 0 if successful, 1 otherwise
 */
int (kbc_read_cmdb)(uint8_t *cmd, bool is_mouse_cmd);

/**
 * @brief Reads the status of the KBC
 * 
 * @param status address of memory to be initialized with the status read
 * @return 0 if successful, 1 otherwise
 */
int kbc_read_status(uint8_t *status);
