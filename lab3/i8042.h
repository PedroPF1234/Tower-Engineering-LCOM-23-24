// Make and Break codes for the keyboard's ESC key

#define MAKE 0x01
#define BREAK 0x00

#define SPECIAL_KEY 0xE0

#define ESC_MAKE 0x01
#define ESC_BREAK 0x81


// Keyboard Controller Commands and Interrupt Identifiers

#define KBC_STATUS_RG 0x64
#define KBC_COMMAND_REG 0x64
#define KBC_DATA_REG 0x60

#define KBC_READ_COMMAND_BYTE 0x20
#define KBC_WRITE_COMMAND_BYTE 0x60

#define KBC_IRQ 0x01 // Keyboard IRQ

#define KBC_OUT_BUFFER_FULL BIT(0)

#define KBC_TRANSMIT_TIMEOUT_ERR BIT(5)
#define KBC_RECEIVE_TIMEOUT_ERR BIT(6)
#define KBC_PARITY_ERR BIT(7)


// Miscellanous Keyboard Related Constants

#define DELAY_US 20000
