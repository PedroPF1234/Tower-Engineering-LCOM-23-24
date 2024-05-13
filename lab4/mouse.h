#pragma once

#include<stdint.h>

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int (mouse_write_cmdb)(uint8_t cmd);

int (mouse_read_cmdb)(uint8_t *info);

void (mouse_ih)();

bool mouse_gesture_event(struct packet* pp, uint8_t x_len, uint8_t tolerance);

int mouse_get_info(struct packet *target);

int mouse_remote(uint8_t remote_byte);
