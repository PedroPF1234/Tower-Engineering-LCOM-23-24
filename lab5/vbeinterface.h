
// VBE function status and register values

// Invoking VBE functions
#define SET_AH 0x4F

// Return codes
#define AL_FUNC_SUPPORTED 0x4F
#define AH_SUCCESS 0x00
#define AH_FUNC_FAILED_CALL 0x01
#define AH_FUNC_FAILED_HW_SUPPORT 0x02
#define AH_FUNC_FAILED_INVALID_MODE 0x03
