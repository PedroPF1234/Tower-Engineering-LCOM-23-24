#include <lcom/lcf.h>
#include <stdint.h>

#include "../lab3/i8042.h"
#include "kbc.h"

int mouse_hook_id = 2;

struct packet pp;
uint8_t packet[3] = {0, 0, 0};
uint8_t packet_to_read = 0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = mouse_hook_id;

  if (sys_irqsetpolicy(KBC_MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)) return 1;

  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&mouse_hook_id)) return 1;
  return 0;
}

int (mouse_write_cmdb)(uint8_t cmd) {
  uint8_t ack = 0;

  if (kbc_write_cmdb(KBC_COMMAND_REG, KBC_WRITE_TO_MOUSE)) return 1;

  if (kbc_write_cmdb(KBC_INPUT_BUFFER, cmd)) return 1;

  tickdelay(micros_to_ticks(DELAY_US));

  if (util_sys_inb(KBC_OUTPUT_BUFFER, &ack)) return 1;

  if (ack != KBC_ACK) return 1;

  return 0;
}

int (mouse_read_cmdb)(uint8_t *info) {
  if (kbc_read_cmdb(info)) return 1;
  return 0;
}

void (mouse_ih)() {
  uint8_t status = 0;
  uint8_t retries = MAX_RETRIES;

  for (size_t i = 0; i < retries; i++) {
    if (util_sys_inb(KBC_STATUS_RG, &status)) return;

    if (status & (KBC_RECEIVE_TIMEOUT_ERR | KBC_PARITY_ERR)) return;

    if (status & KBC_OUT_BUFFER_FULL) {
      if (util_sys_inb(KBC_DATA_REG, &packet[packet_to_read])) return;
      break;
    }

    tickdelay(micros_to_ticks(DELAY_US));

    if (i == retries -1) return;
  }

  if (packet_to_read == 0) {
    memset(&pp, 0, sizeof(pp));
    if (packet[0] & BIT(3)) {
      packet_to_read++;
    }
  } else if (packet_to_read == 1) {
    packet_to_read++;
  } else {
    packet_to_read = 0;
    pp.bytes[0] = packet[0];
    pp.bytes[1] = packet[1];
    pp.bytes[2] = packet[2];

    pp.lb = packet[0] & BIT(0);
    pp.rb = packet[0] & BIT(1);
    pp.mb = packet[0] & BIT(2);
    pp.x_ov = packet[0] & BIT(6);
    pp.y_ov = packet[0] & BIT(7);

    if (packet[0] & BIT(4)) {
      pp.delta_x = packet[1] - 256;
    } else {
      pp.delta_x = packet[1];
    }

    if (packet[0] & BIT(5)) {
      pp.delta_y = packet[2] - 256;
    } else {
      pp.delta_y = packet[2];
    }
    memset(&packet, 0, sizeof(packet));
  }
}
