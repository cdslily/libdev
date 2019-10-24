#ifndef _LINUX_TYPE_DEF_
#define _LINUX_TYPE_DEF_
#include <stdint.h>

#define BUS_VIRTUAL 0x06

#define MOUSE_MASK_LEFT 0x1
#define MOUSE_MASK_MID 0x2
#define MOUSE_MASK_RIGHT 0x4

#define MOUSE_MASK_DOWN 0x10
#define MOUSE_MASK_UP 0x20
#define MOUSE_MASK_DRAG 0x40

#define MOUSE_MASK_CLICK 0x100

#define MOUSE_MASK_WHEEL_DOWN 0x00000001
#define MOUSE_MASK_WHEEL_UP 0xFFFFFFFF

typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
#endif