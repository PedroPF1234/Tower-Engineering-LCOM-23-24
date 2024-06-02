#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "vbe_macros.h"

static char *video_buf;
static vbe_mode_info_t vmi_p;

int vg_set_mode(uint16_t mode){

  reg86_t r86;

  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10; 
  r86.ah = AH_VBE_FUNC;  
  r86.al = SET_VBE_MODE;
  r86.bx = mode | BIT(14);

  if(sys_int86(&r86) != OK) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }

  return 0;
}

void *(vg_init)(uint16_t mode){

  void *video_mem; 

  memset(&vmi_p, 0, sizeof(vmi_p));

  if (vbe_get_mode_info(mode, &vmi_p)){
    printf("Failed to fetch vbe mode info\n");
    return NULL;
  }

  struct minix_mem_range mr;
  uint32_t vram_size;

  vram_size = vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel+7)/8);

  mr.mr_base = vmi_p.PhysBasePtr;
  mr.mr_limit = mr.mr_base + (phys_bytes) vram_size; 

  int r;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);


  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  video_buf = (char*) video_mem;

  if (vg_set_mode(mode)){
    return NULL;
  }

  return video_buf;  
}

uint32_t normalizer(uint32_t color, bool isIndexed){

  if (isIndexed){
    return (color % 256);
  }

  uint8_t redSize = vmi_p.RedMaskSize;
  uint8_t greenSize = vmi_p.GreenMaskSize;
  uint8_t blueSize = vmi_p.BlueMaskSize;

  uint8_t redPos = vmi_p.RedFieldPosition;
  uint8_t greenPos = vmi_p.GreenFieldPosition;
  uint8_t bluePos = vmi_p.BlueFieldPosition;

  uint32_t normalized_color;

  //uint8_t a = (uint8_t) (color >> 24);
  uint8_t r = (uint8_t) (color >> 16);
  uint8_t g = (uint8_t) (color >> 8);
  uint8_t b = (uint8_t) color;

  uint8_t new_r = BIT_MASK(redSize) * r / 255;
  uint8_t new_g = BIT_MASK(greenSize) * g / 255;
  uint8_t new_b = BIT_MASK(blueSize) * b / 255;

  uint32_t redComponent = ( (uint32_t) new_r) << redPos;
  uint32_t greenComponent = ( (uint32_t) new_g ) << greenPos;
  uint32_t blueComponent = ( (uint32_t) new_b ) << bluePos;

  normalized_color = redComponent | greenComponent | blueComponent;

  return normalized_color;
}


int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color){

  if (x >= vmi_p.XResolution || y >= vmi_p.YResolution || x < 0 || y < 0){
    return 0;
  }

  uint32_t normalized_color = normalizer(color,((vmi_p.BitsPerPixel + 7)/ 8) == 1);

  memcpy(video_buf+(x+(y*vmi_p.XResolution))*((vmi_p.BitsPerPixel + 7)/8),&normalized_color,(vmi_p.BitsPerPixel + 7)/8);

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (int i = 0; i < len; i++) {
    vg_draw_pixel(x+i,y,color);
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

  for (int i = 0; i < height; i ++){
    vg_draw_hline(x,y+i,width,color);
  }

  return 0;
}
