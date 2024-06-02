#pragma once

#include <stdint.h>

/**
 * @brief Initializes the graphics in the given mode
 * 
 * @param mode mode to set the graphics to
 */
void* (vg_init)(uint16_t mode);

/**
 * @brief Draws a line of pixels
 * 
 * @param x x coordinate of the first pixel of the line
 * @param y y coordinate of the first pixel of the line
 * @param len length of the line
 * @param color color of the pixels of the line
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle
 * 
 * @param x x coordinate of the top left corner of the rectangle
 * @param y y coordinate of the top left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color color of the pixels of the rectangle
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a sprite
 * 
 * @param x x coordinate of the top left corner of the rectangle
 * @param y y coordinate of the top left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param bytes array of bytes that represent the color of the pixels of the rectangle
 * @param square_shape if the rectangle is a square
 */
int vg_draw_xpm(int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t *bytes, bool square_shape);

/**
 * @brief Draws a pixel in the screen
 * 
 * @param x x coordinate of the pixel
 * @param y y coordinate of the pixel
 * @param color color of the pixel
 */
int proj_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Uses vbe function to page flip
 * @return 0 if successful, 1 otherwise
 */
int vg_replace_buffer();

/**
 * @brief Frees the memory allocated for the graphics
 * @return 0 if successful, 1 otherwise
 */
int vg_free();

/**
 * @brief Gets the screen information of the graphics
 * @return array with the information of the graphics
 */
uint16_t* getInfo();
