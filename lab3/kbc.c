#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"

int hook_id = KBC_IRQ;

int (kbc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = hook_id;

  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &bit_no)) return 1;

  return 0;
}

int (kbs_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)) return 1;
  return 0;
}