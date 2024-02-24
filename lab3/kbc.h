#pragma once

#include<stdint.h>

int (kbc_subscribe_int)(uint8_t* bit_no);

int (kbc_unsubscribe_int)();

int (kbd_reenable_interrupts)();
