// Make and Break codes for the keyboard's ESC key

#define SPECIAL_KEY 0xE0

#define ESC_MAKE 0x01
#define ESC_BREAK 0x81


// Keyboard Controller Commands and Interrupt Identifiers

#define KBC_COMMAND_REG 0x64
#define KBC_DATA_REG 0x60

#define KBC_READ_COMMAND_BYTE 0x20
#define KBC_WRITE_COMMAND_BYTE 0x60

#define KBC_IRQ 0x01 // Keyboard IRQ


// Miscellanous Keyboard Related Constants

#define DELAY_US 20000