
#define BIT_MASK(n) ((1 << (n + 1)) - 1)

// VBE function status and register values

// Invoking VBE functions
#define SET_AH 0x4F

// Return codes
#define AL_FUNC_SUPPORTED 0x4F
#define AH_SUCCESS 0x00
#define AH_FUNC_FAILED_CALL 0x01
#define AH_FUNC_FAILED_HW_SUPPORT 0x02
#define AH_FUNC_FAILED_INVALID_MODE 0x03


// Video Modes
#define MODE_INDEXED 0x105
#define MODE_DIRECT_15 0x110
#define MODE_DIRECT_24 0x115
#define MODE_DIRECT_16 0x11A
#define MODE_DIRECT_32 0x14C

// VBE Mode
#define SET_VBE_MODE 0x02
#define SET_TEXT_MODE 0x03
