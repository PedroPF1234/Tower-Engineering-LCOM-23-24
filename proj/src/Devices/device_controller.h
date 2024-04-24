#pragma once

#include <lcom/lcf.h>

#include <stdint.h>

#include "../ImageAssets/sprite.h"

int boot_devices(uint32_t freq, uint16_t mode);

int stop_devices();

int interrupt_handler(uint32_t interrupt_mask);
