
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


// Video Modes                          (Resolution, Color Count)
// Indexed Modes (1 byte per pixel)       
#define MODE_INDEXED_DEFAULT 0x105      //  1024x768 256
#define MODE_INDEXED_1 0x100            //   640x400 256
#define MODE_INDEXED_2 0x101            //   640x480 256
#define MODE_INDEXED_3 0x102            //   800x600 16 
#define MODE_INDEXED_4 0x103            //   800x600 256
#define MODE_INDEXED_5 0x104            //  1024x768 16
#define MODE_INDEXED_6 0x106            // 1280x1024 16
#define MODE_INDEXED_7 0x107            // 1280x1024 256

// Direct Modes
// (1:5:5:5) (2 bytes per pixel)
#define MODE_DIRECT_15_DEFAULT 0x110    //   640x480 32K
#define MODE_DIRECT_15_1 0x10D          //   320x200 32K
#define MODE_DIRECT_15_2 0x113          //   800x600 32K
#define MODE_DIRECT_15_3 0x116          //  1024x768 32K
#define MODE_DIRECT_15_4 0x119          // 1280x1024 32K

// (5:6:5) (2 bytes per pixel)
#define MODE_DIRECT_16_DEFAULT 0x11A    // 1280x1024 64K
#define MODE_DIRECT_16_1 0x10E          //   320x200 64K
#define MODE_DIRECT_16_2 0x111          //   640x480 64K
#define MODE_DIRECT_16_3 0x114          //   800x600 64K
#define MODE_DIRECT_16_4 0x117          //  1024x768 64K

// (8:8:8) (3 bytes per pixel)
#define MODE_DIRECT_24_DEFAULT 0x115    //  1024x768 16.8M
#define MODE_DIRECT_24_1 0x10F          //   320x200 16.8M
#define MODE_DIRECT_24_2 0x112          //   640x480 16.8M
#define MODE_DIRECT_24_3 0x118          //   800x600 16.8M
#define MODE_DIRECT_24_4 0x11B          // 1280x1024 16.8M

// (8:8:8:8) (4 bytes per pixel)
#define MODE_DIRECT_32_DEFAULT 0x14C    //  1024x768 16.8M

// VBE Mode
#define SET_VBE_MODE 0x02
#define SET_TEXT_MODE 0x03
