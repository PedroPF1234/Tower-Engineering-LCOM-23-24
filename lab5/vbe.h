#pragma once
#include <stdint.h>

int vg_set_mode(uint16_t mode);
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
uint32_t normalizer(uint32_t color, bool isIndexed);

