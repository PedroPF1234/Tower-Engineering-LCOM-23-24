#pragma once

#include<stdint.h>

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int (mouse_write_cmdb)(uint8_t cmd);

int (mouse_read_cmdb)(uint8_t *info);

int (mouse_enable_data_reporting)();

int (mouse_disable_data_reporting)();

void (mouse_ih)();
