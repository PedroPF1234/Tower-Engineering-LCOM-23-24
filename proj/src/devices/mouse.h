#pragma once

#include <stddef.h>
#include <lcom/lcf.h>
#include <stdint.h>

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int (mouse_write_cmdb)(uint8_t cmd);

int (mouse_read_cmdb)(uint8_t *info);

void (mouse_ih)();

bool mouse_gesture_event(struct packet* pp, uint8_t x_len, uint8_t tolerance);


/* XPM */
static char * temp_cursor[] = {
"12 19 3 1",
" 	c #000000",
".	c #F3F1F3",
"+	c #FFFFFE",
".           ",
"..          ",
".+.         ",
".++.        ",
".+++.       ",
".++++.      ",
".+++++.     ",
".++++++.    ",
".+++++++.   ",
".++++++++.  ",
".+++++++++. ",
".++++++++++.",
".++++++.....",
".+++.++.    ",
".++. .++.   ",
".+.  .++.   ",
"..    .++.  ",
"      .++.  ",
"       ..   "};
