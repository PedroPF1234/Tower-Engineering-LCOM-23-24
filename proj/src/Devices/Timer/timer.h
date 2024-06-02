#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Subscribes and enables Timer 0 interrupts with a given frequency
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @param freq frequency of the timer
 * @return 0 if successful, 1 otherwise
 */
int timer_initiate_and_subscribe(uint8_t *bit_no, uint32_t freq);
