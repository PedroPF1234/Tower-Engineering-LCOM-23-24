#include <lcom/lcf.h>
#include <stdint.h>

#include "vbe_interface.h"

static reg86_t reg86;
static vbe_mode_info_t vmi_p;
//static vg_vbe_contr_info_t vci_p;

static char *video_mem;

static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;

static uint16_t current_mode;

void* (vg_init)(uint16_t mode) {

  struct minix_mem_range mr;

  unsigned int vram_base;
  unsigned int vram_size;

  void* video_addr;

  int r;

  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ax = 0x4F02;
  reg86.bx = BIT(14) | mode;
  if (sys_int86(&reg86)) return NULL;

  if (vbe_get_mode_info(mode, &vmi_p)) return NULL;

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

  video_mem = (char *) video_addr;

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;

  current_mode = mode;

  return video_addr;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x + len > h_res || y > v_res) return 1;

  for (unsigned i = 0; i < len; i++) {
    *(video_mem + (y * h_res + x + i) * (bits_per_pixel / 8)) = color;
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (x + width > h_res || y + height > v_res) return 1;

  for (unsigned i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color)) return 1;
  }

  return 0;
}
