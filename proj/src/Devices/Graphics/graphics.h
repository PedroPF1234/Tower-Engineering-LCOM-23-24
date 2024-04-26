#pragma once

#include <stdint.h>

void* (vg_init)(uint16_t mode);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int vg_draw_xpm(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *bytes, bool square_shape);

int vg_replace_buffer();

int vg_free();
