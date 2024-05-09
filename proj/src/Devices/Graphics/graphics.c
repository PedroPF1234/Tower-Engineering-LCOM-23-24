#include <lcom/lcf.h>
#include <stdint.h>

#include "vbe_interface.h"

static reg86_t reg86;
static vbe_mode_info_t vmi_p;

// VRAM buffers
static char *current_buffer;

static char *primary_buffer;
static char *secondary_buffer;
//

// Mode Window info
static unsigned bytes_per_pixel;
static unsigned bits_per_pixel;

static int h_res;
static int v_res;
//

// Mode Color info
static uint8_t red_field_position;
static uint8_t green_field_position;
static uint8_t blue_field_position;

uint8_t red_pixel_mask;
uint8_t green_pixel_mask;
uint8_t blue_pixel_mask;
//

uint16_t current_mode;

int (vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *vmi_p) {
  mmap_t map;

  lm_alloc(sizeof(vbe_mode_info_t), &map);

  phys_bytes buf = map.phys;

  struct reg86 r86;
  memset(&r86, 0, sizeof(r86));
  r86.ax = 0x4F01;
  r86.cx = mode;
  r86.es = PB2BASE(buf);
  r86.di = PB2OFF(buf);
  r86.intno = 0x10;

  if (sys_int86(&r86)) return 1;

  *vmi_p = *((vbe_mode_info_t *) map.virt);

  lm_free(&map);

  return 0;
}

void* (vg_init)(uint16_t mode) {

  struct minix_mem_range mr;

  unsigned int vram_base;
  unsigned int vram_size;

  void* video_addr;

  int r;

  memset(&reg86, 0, sizeof(reg86));

  if (vbe_get_mode_information(mode, &vmi_p)) return NULL;

  bytes_per_pixel = ((vmi_p.BitsPerPixel+7)/8);
  bits_per_pixel = vmi_p.BitsPerPixel;

  vram_base = vmi_p.PhysBasePtr;
  vram_size = vmi_p.XResolution * vmi_p.YResolution * bytes_per_pixel;

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size*2;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  }

  video_addr = vm_map_phys(SELF, (void *)mr.mr_base, vram_size*2);

  if(video_addr == MAP_FAILED) {
    panic("couldn't map video memory");
  }

  reg86.intno = 0x10;
  reg86.ax = 0x4F02;
  reg86.bx = BIT(14) | mode;
  if (sys_int86(&reg86)) return NULL;

  primary_buffer = (char *) video_addr;
  secondary_buffer = (char *) video_addr + vram_size;
  current_buffer = primary_buffer;
  memset(primary_buffer, 0, vram_size);
  memset(secondary_buffer, 0, vram_size);

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;

  red_pixel_mask = vmi_p.RedMaskSize;
  green_pixel_mask = vmi_p.GreenMaskSize;
  blue_pixel_mask = vmi_p.BlueMaskSize;

  red_field_position = vmi_p.RedFieldPosition;
  green_field_position = vmi_p.GreenFieldPosition;
  blue_field_position = vmi_p.BlueFieldPosition;

  current_mode = mode;

  return video_addr;
}

uint32_t (normalize_color)(uint32_t color) {

  uint32_t normalized_color = 0;

  if (current_mode == MODE_INDEXED_DEFAULT) {
    normalized_color = (uint8_t)(color % 256);
    return normalized_color;
  }

  uint8_t redOriginalComponent = (uint8_t) (color >> 16);
  uint8_t greenOriginalComponent = (uint8_t) (color >> 8);
  uint8_t blueOriginalComponent = (uint8_t) color;

  uint8_t redNewValue = BIT_MASK(red_pixel_mask) * redOriginalComponent / 255;
  uint8_t greenNewValue = BIT_MASK(green_pixel_mask) * greenOriginalComponent / 255;
  uint8_t blueNewValue = BIT_MASK(blue_pixel_mask) * blueOriginalComponent / 255;

  uint32_t redComponent = ((uint32_t) redNewValue) << red_field_position;
  uint32_t greenComponent = ((uint32_t) greenNewValue) << green_field_position;
  uint32_t blueComponent = ((uint32_t) blueNewValue) << blue_field_position;

  normalized_color = redComponent | greenComponent | blueComponent;

  return normalized_color;

}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x > h_res - 1 || y > v_res - 1) {
    return 0;
  }

  uint32_t normalized_color = normalize_color(color);

  memcpy((secondary_buffer + (y * h_res + x) * bytes_per_pixel), &normalized_color, bytes_per_pixel);

  return 0;
}

int proj_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (color == 0 || x >= h_res || y >= v_res || x < 0 || y < 0) {
    return 0;
  }

  memcpy((current_buffer + (y * h_res + x) * bytes_per_pixel), &color, bytes_per_pixel);

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x + len > h_res || y > v_res) {
    //printf("vg_draw_hline: invalid coordinates\n");
    return 0;
  }

  for (unsigned i = 0; i < len; i++) {
    #if 0
    if (proj_draw_pixel(x + i, y, color)) return 1;
    #else
    if (vg_draw_pixel(x + i, y, color)) return 1;
    #endif
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (x + width > h_res || y + height > v_res) {
    //printf("vg_draw_rectangle: invalid coordinates\n");
    return 0;
  }

  for (unsigned i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color)) return 1;
  }

  return 0;
}

int vg_draw_xpm(int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t *bytes, bool square_shape) {

  uint32_t color = 0;

  for (int i = 0; i < height; i++) {

    if (square_shape) {

      uint16_t draw_line_len = x + width >= h_res ? h_res - x : width;

      memcpy(current_buffer + ((y + i) * h_res * bytes_per_pixel) + (x * bytes_per_pixel),
       bytes + i * width * bytes_per_pixel, draw_line_len * bytes_per_pixel);
    } else {
      for (int j = 0; j < width; j++) {  

        memcpy(&color, bytes + (i * width + j) * bytes_per_pixel, bytes_per_pixel);
        if (y + i > v_res || x + j > h_res || y + i < 0 || x + j < 0 || color == 0) {
          continue;
        }
        memcpy((current_buffer+((y+i) * h_res + (x+j)) * bytes_per_pixel), &color, bytes_per_pixel);
        color = 0;
      }
    }
  }

  return 0;
}

int vg_clean_buffer() {
  memset(current_buffer, 0, (h_res * v_res * bytes_per_pixel));
  return 0;
}

int vg_replace_buffer() {
  // Page flipping
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ax = 0x4F07;
  reg86.bx = 0x0080;
  reg86.cx = 0;
  reg86.dx = current_buffer == primary_buffer ? 0 :  v_res;
 

  if (sys_int86(&reg86)) return 1;

  if (reg86.ah != 0) return 1;

  current_buffer = current_buffer == primary_buffer ? secondary_buffer : primary_buffer;

  // Double buffering 
  /*
  memcpy(primary_buffer, secondary_buffer, (h_res * v_res * bytes_per_pixel));
  */
  if (vg_clean_buffer()) return 1;
  return 0; 
}

int vg_free() {
  free(primary_buffer);
  return 0;
}

uint16_t* getInfo() {

  uint16_t* info = malloc(sizeof(uint16_t)*2);

  *info = (uint16_t)h_res;
  *(info+1) = (uint16_t)v_res;

  return info;
}
