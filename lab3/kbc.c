#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"

int kbc_hook_id = KBC_IRQ;
uint8_t scancode[2] = {0, 0};
gid_t ctr = 0;

int (kbc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = kbc_hook_id;

  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbc_hook_id)) return 1;

  return 0;
}

int (kbc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&kbc_hook_id)) return 1;
  return 0;
}

void (kbc_ih)() {

  uint8_t status = 0;
  uint8_t retries = 3;

  for (size_t i = 0; i < retries; i++) {

    if (util_sys_inb(KBC_STATUS_RG, &status)) return;
    #ifdef LAB3
    ctr++;
    #endif

    //tickdelay(micros_to_ticks(DELAY_US));

    if (status & (KBC_TRANSMIT_TIMEOUT_ERR | KBC_RECEIVE_TIMEOUT_ERR | KBC_PARITY_ERR)) return;

    if (status & KBC_OUT_BUFFER_FULL) {
      if (util_sys_inb(KBC_DATA_REG, &scancode[1])) return;
      #ifdef LAB3
      ctr++;
      #endif
      break;
    }
  }

  return;
}

int (kbd_write_cmdb)(uint8_t port, uint8_t cmd) {
  uint8_t status = 0;
  uint8_t retries = 3;

  for (size_t i = 0; i < retries; i++) {

    if (util_sys_inb(KBC_STATUS_RG, &status)) return 1;
    #ifdef LAB3
    ctr++;
    #endif

    if (status & (KBC_TRANSMIT_TIMEOUT_ERR | KBC_RECEIVE_TIMEOUT_ERR | KBC_PARITY_ERR)) return 1;

    if (status & KBC_IN_BUFFER_FULL) {
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }

    if (sys_outb(port, cmd)) return 1;
    break;
  }

  return 0;
}

int (kbd_read_cmdb)(uint8_t port, uint8_t *cmd) {
  uint8_t status = 0;
  uint8_t retries = 3;

  for (size_t i = 0; i < retries; i++) {

    if (util_sys_inb(KBC_STATUS_RG, &status)) return 1;
    #ifdef LAB3
    ctr++;
    #endif

    if (status & (KBC_TRANSMIT_TIMEOUT_ERR | KBC_RECEIVE_TIMEOUT_ERR | KBC_PARITY_ERR)) return 1;

    if (status & KBC_OUT_BUFFER_FULL) {
      if (util_sys_inb(port, cmd)) return 1;
      #ifdef LAB3
      ctr++;
      #endif
      break;
    }
  }

  return 0;
}

int (kbd_reenable_interrupts)() {

  uint8_t cmd;

  if (kbd_write_cmdb(KBC_COMMAND_REG, KBC_READ_COMMAND_BYTE)) return 1;

  if (kbd_read_cmdb(KBC_OUTPUT_BUFFER, &cmd)) return 1;

  cmd |= KBC_ENABLE_KBD_INT;

  if (kbd_write_cmdb(KBC_COMMAND_REG, KBC_WRITE_COMMAND_BYTE)) return 1;

  if (kbd_write_cmdb(KBC_INPUT_BUFFER, cmd)) return 1;

  if (kbd_print_no_sysinb(ctr)) return 1;

  return 0;
}
