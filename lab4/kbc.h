#pragma once

#include<stdint.h>

int (kbc_subscribe_int)(uint8_t* bit_no);

int (kbc_unsubscribe_int)();

int (kbd_reenable_interrupts)();

void (kbc_ih)();

int (kbc_write_cmdb)(uint8_t port, uint8_t cmd);

int (kbc_read_cmdb)(uint8_t *cmd);

int (kbd_reenable_interrupts)();
