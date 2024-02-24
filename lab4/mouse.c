#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

int hook_id = 12;

struct packet pp;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = hook_id;

  if (sys_irqsetpolicy(KBC_MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) return 1;

  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)) return 1;
  return 0;
}

int (mouse_enable_data_reporting)();

void (mouse_ih)() {

}

int (mouse_write_cmdb)(uint8_t port, uint8_t cmd) {
  uint8_t status = 0;
  uint8_t retries = 3;

  for (size_t i = 0; i < retries; i++) {

    if (util_sys_inb(KBC_STATUS_RG, &status)) return 1;
    #ifdef LAB3
    ctr++;
    #endif

    if (status & (KBC_TRANSMIT_TIMEOUT_ERR | KBC_RECEIVE_TIMEOUT_ERR | KBC_PARITY_ERR)) return 1;

    if (status & KBC_IN_BUFFER_FULL) {
      if (sys_outb(port, cmd)) return 1;
      #ifdef LAB3
      ctr++;
      #endif
      break;
    }
  }

  return 0;
}
