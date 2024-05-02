// Make and Break codes for the keyboard's ESC key

#define MAKE 0x01 // Make Boolean
#define BREAK 0x00 // Break Boolean

#define SPECIAL_KEY 0xE0 // Special Key Scancode Byte

// Keyboard Scancodes
#define ESC_MAKE 0x01 // Make Code for ESC Key
#define ESC_BREAK 0x81 // Break Code for ESC Key

#define UP_ARROW_MAKE 0x48 // Make Code for UP Arrow Key
#define UP_ARROW_BREAK 0xC8 // Break Code for UP Arrow Key
#define DOWN_ARROW_MAKE 0x50 // Make Code for DOWN Arrow Key
#define DOWN_ARROW_BREAK 0xD0 // Break Code for DOWN Arrow Key
#define LEFT_ARROW_MAKE 0x4B // Make Code for LEFT Arrow Key
#define LEFT_ARROW_BREAK 0xCB // Break Code for LEFT Arrow Key
#define RIGHT_ARROW_MAKE 0x4D // Make Code for RIGHT Arrow Key
#define RIGHT_ARROW_BREAK 0xCD // Break Code for RIGHT Arrow Key

#define ENTER_MAKE 0x1C // Make Code for ENTER Key
#define ENTER_BREAK 0x9C // Break Code for ENTER Key


// Keyboard Controller Commands and Interrupt Identifiers

#define MAX_RETRIES 5 // Maximum Number of Tries to Read/Write to KBC

#define KBC_ACK 0xFA // Acknowledgment Byte
#define KBC_RESEND 0xFE // Resend Byte

#define KBC_STATUS_RG 0x64 // Read-Only
#define KBC_COMMAND_REG 0x64 // Write-Only
#define KBC_DATA_REG 0x60 // Read-Only
#define KBC_OUTPUT_BUFFER 0x60 // Read-Only
#define KBC_INPUT_BUFFER 0x60 // Write-Only

#define KBC_READ_COMMAND_BYTE 0x20 // To write to KBC_COMMAND_REG
#define KBC_WRITE_COMMAND_BYTE 0x60 // To write to KBC_COMMAND_REG
#define KBC_WRITE_TO_MOUSE 0xD4 // To write to KBC_COMMAND_REG

#define KBC_MOUSE_DISABLE_STREAM_MODE_REPORTING 0xF5 // To write to KBC_INPUT_BUFFER
#define KBC_MOUSE_ENABLE_STREAM_MODE_REPORTING 0xF4 // To write to KBC_INPUT_BUFFER
#define KBC_MOUSE_SET_SAMPLE_RATE 0xF3 // To write to KBC_INPUT_BUFFER
#define KBC_MOUSE_SET_STREAM_MODE 0xEA // To write to KBC_INPUT_BUFFER
#define KBC_MOUSE_SET_REMOTE_MODE 0xF0 // To write to KBC_INPUT_BUFFER
#define KBC_MOUSE_READ_DATA 0xEB // To write to KBC_INPUT_BUFFER


#define KBC_IRQ 1 // Keyboard IRQ
#define KBC_MOUSE_IRQ 12 // Mouse IRQ

#define KBC_OUT_BUFFER_FULL BIT(0) // Status Register Bit 0
#define KBC_IN_BUFFER_FULL BIT(1) // Status Register Bit 1
#define MOUSE_DATA BIT(5) // Status Register Bit 5

#define KBC_TRANSMIT_TIMEOUT_ERR BIT(5) // Status Register Bit 5
#define KBC_RECEIVE_TIMEOUT_ERR BIT(6) // Status Register Bit 6
#define KBC_PARITY_ERR BIT(7) // Status Register Bit 7

#define KBC_ENABLE_KBD_INT BIT(0) // Command Byte Bit 0


// Miscellanous Keyboard Related Constants

#define DELAY_US 20000
