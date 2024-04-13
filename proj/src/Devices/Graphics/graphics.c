#include <lcom/lcf.h>
#include <stdint.h>

#include "vbe_interface.h"

static reg86_t reg86;
static vbe_mode_info_t vmi_p;
//static vg_vbe_contr_info_t vci_p;

static char *video_mem;

static char *secondary_buffer;

static unsigned bytes_per_pixel;

static uint8_t red_field_position;
static uint8_t green_field_position;
static uint8_t blue_field_position;

unsigned bits_per_pixel;

unsigned h_res;
unsigned v_res;

uint8_t red_pixel_mask;
uint8_t green_pixel_mask;
uint8_t blue_pixel_mask;

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

  vram_base = vmi_p.PhysBasePtr;
  vram_size = vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel + 7) / 8);

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  }

  video_addr = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_addr == MAP_FAILED) {
    panic("couldn't map video memory");
  }

  reg86.intno = 0x10;
  reg86.ax = 0x4F02;
  reg86.bx = BIT(14) | mode;
  if (sys_int86(&reg86)) return NULL;

  video_mem = (char *) video_addr;
  secondary_buffer = (char *) malloc(vram_size);
  memset(secondary_buffer, 0, vram_size);

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;

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

  normalized_color |= (color & BIT_MASK(red_pixel_mask)) << red_field_position;
  normalized_color |= ((color >> 8) & BIT_MASK(green_field_position)) << green_field_position;
  normalized_color |= ((color >> 16) & BIT_MASK(blue_pixel_mask)) << blue_field_position;

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

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x + len > h_res || y > v_res) {
    printf("vg_draw_hline: invalid coordinates\n");
    return 0;
  }

  for (unsigned i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color)) return 1;
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (x + width > h_res || y + height > v_res) {
    printf("vg_draw_rectangle: invalid coordinates\n");
    return 0;
  }

  for (unsigned i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color)) return 1;
  }

  return 0;
}

int vg_draw_xpm(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *bytes) {
  if (x > h_res || y > v_res) {
    printf("vg_draw_xpm: invalid coordinates\n");
    return 0;
  }

  uint32_t color = 0;

  for (unsigned i = 0; i < height; i++) {
    for (unsigned j = 0; j < width; j++) {

      memcpy(&color, bytes + (i * width + j) * bytes_per_pixel, bytes_per_pixel);
      if (vg_draw_pixel(x + j, y + i, color)) return 1;
      color = 0;
    }
  }

  return 0;
}

int vg_clean_buffer() {
  memset(secondary_buffer, 0, (h_res * v_res * bytes_per_pixel));
  return 0;
}

int vg_replace_buffer() {
  memcpy(video_mem, secondary_buffer, (h_res * v_res * bytes_per_pixel));
  if (vg_clean_buffer()) return 1;
  return 0; 
}

int vg_free() {
  if (vg_replace_buffer()) return 1;
  free(secondary_buffer);
  return 0;
}

