#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

#define GESTURE_START 0
#define GESTURE_LB_PRESS 1
#define GESTURE_LB_MOVE 2
#define GESTURE_LB_RELEASE 3
#define GESTURE_RB_PRESS 4
#define GESTURE_RB_MOV 5

int mouse_hook_id = 2;

struct packet pp;
uint8_t packet[3] = {0, 0, 0};
uint8_t packet_to_read = 0;
uint8_t state = GESTURE_START;
bool previous_lb = false;
bool previous_rb = false;

int16_t min_x_len = 0;

int16_t initial_pos[2] = {0, 0};
int16_t current_pos[2] = {0, 0};

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

  if (kbc_write_cmdb(KBC_COMMAND_REG, KBC_WRITE_TO_MOUSE, true)) return 1;

  if (kbc_write_cmdb(KBC_INPUT_BUFFER, cmd, true)) return 1;

  tickdelay(micros_to_ticks(DELAY_US));

  if (util_sys_inb(KBC_OUTPUT_BUFFER, &ack)) return 1;

  if (ack != KBC_ACK) return 1;

  return 0;
}

int (mouse_read_cmdb)(uint8_t *info) {
  if (kbc_read_cmdb(info, true)) return 1;
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

struct mouse_ev* mouse_detect_event_	(struct packet* pp) {

  static struct mouse_ev me;

  if (pp->lb && !previous_lb) {
    me.type = LB_PRESSED;
    previous_lb = true;
  } else if (!pp->lb && previous_lb) {
    me.type = LB_RELEASED;
    previous_lb = false;
  } else if (pp->rb && !previous_rb) {
    me.type = RB_PRESSED;
    previous_rb = true;
  } else if (!pp->rb && previous_rb) {
    me.type = RB_RELEASED;
    previous_rb = false;
  } else {
    me.type = MOUSE_MOV;
    me.delta_x = pp->delta_x;
    me.delta_y = pp->delta_y;
  }

  return &me;

}

bool mouse_gesture_event(struct packet* pp, uint8_t x_len, uint8_t tolerance) {

  struct mouse_ev* me = mouse_detect_event_(pp);

  switch (state)
  {
  case GESTURE_START:
    if (me->type == LB_PRESSED) {
      state = GESTURE_LB_PRESS;
      min_x_len = x_len;
    }
    break;

  case GESTURE_LB_PRESS:
    if (me->type == MOUSE_MOV) {  
      current_pos[0] += me->delta_x;
      current_pos[1] += me->delta_y;
      float slope = (float)current_pos[1] / (float)current_pos[0];
      if ((slope < 1 && slope > -1) || ((me->delta_y - tolerance) > 0 ||
      (me-> delta_x + tolerance) < 0)) {
        memset(&current_pos, 0, sizeof(current_pos));
        state = GESTURE_START;
        break;
      }
      min_x_len -= abs(me->delta_x);
    }
    else if (me->type == LB_RELEASED) {
      if (min_x_len < 0) state = GESTURE_LB_RELEASE;
      else state = GESTURE_START;
    }
    else {
      state = GESTURE_START;
    }
    break;

  case GESTURE_LB_RELEASE:
    if (me->type == RB_PRESSED) {
      state = GESTURE_RB_PRESS;
      min_x_len = x_len;
    }
    else if (me->type == MOUSE_MOV) {
      break;
    }
    else {
      state = GESTURE_START;
    }
    break;

  case GESTURE_RB_PRESS:
    if (me->type == MOUSE_MOV) { 
      current_pos[0] += me->delta_x;
      current_pos[1] += me->delta_y;
      float slope = (float)current_pos[1] / (float)current_pos[0];
      if ((slope < 1 && slope > -1) || ((me->delta_y + tolerance) < 0 ||
      (me-> delta_x + tolerance) < 0)) {
        memset(&current_pos, 0, sizeof(current_pos));
        state = GESTURE_START;
        break;
      }
      min_x_len -= abs(me->delta_x);
    }
    else if (me->type == RB_RELEASED) {
      if (min_x_len < 0) return true;
      else state = GESTURE_START;
    }
    else {
      state = GESTURE_START;
    }
    break;
  
  default:
    break;
  }


  return false;
}
