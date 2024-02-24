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

int (mouse_write_cmdb)(uint8_t cmd) {
  uint8_t ack = 0;

  if (kbc_write_cmdb(KBC_COMMAND_REG, KBC_WRITE_TO_MOUSE)) return 1;

  if (kbc_write_cmdb(KBC_INPUT_BUFFER, cmd)) return 1;

  tickdelay(micros_to_ticks(DELAY_US));

  if (kbc_read_cmdb(&ack)) return 1;

  if (ack != KBC_ACK) return 1;

  return 0;
}

int (mouse_read_cmdb)(uint8_t *info) {
  
}
