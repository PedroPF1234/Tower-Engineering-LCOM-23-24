#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

int boot_devices(uint32_t freq);

int stop_devices();

int interrupt_handler(uint32_t interrupt_mask);
