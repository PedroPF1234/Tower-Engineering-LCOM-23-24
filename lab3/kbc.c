#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"

int hook_id = 0;

int (kbc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = KBC_IRQ;
  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, bit_no)) return 1;
  return 0;
}

int (kbs_unsubscribe_int)() {
  if (sys_irqrmpolicy(&bit_no)) return 1;
  return 0;
}