#include "mouse_event.h"
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "type_def.h"

#define UINPUT_DEVICVE "/dev/uinput"
#define MOUSE_NAME "linux_virtual_mouse"
#define MOUSE_VENDOR_ID 0x51

static int mouse_fd = -1;

bool mouse_enable = false;

int WriteInput(int uinput_fd, uint16_t type, uint16_t code, int32_t value) {
    struct input_event event;
    memset(&event, 0, sizeof(event));
    // This should not be able to fail ever..
    gettimeofday(&event.time, 0);
    event.type = type;
    event.code = code;
    event.value = value;
    if (write(uinput_fd, &event, sizeof(event)) != sizeof(event)) {
        return -1;
    }
    return 0;
}

static int ClickKey(int fd, int key) {
    if (WriteInput(fd, EV_KEY, key, 1) == -1) {
        return -1;
    }
    if (WriteInput(fd, EV_SYN, SYN_REPORT, 0) == -1) {
        return -1;
    }
    if (WriteInput(fd, EV_KEY, key, 0) == -1) {
        return -1;
    }
    if (WriteInput(fd, EV_SYN, SYN_REPORT, 0) == -1) {
        return -1;
    }
    return 0;
}

static int MouseClickEvent(int fd, int mask, int x, int y) {
    if (mask & MOUSE_MASK_LEFT) {
        if (ClickKey(fd, BTN_LEFT) == -1) return -1;
    }
    if (mask & MOUSE_MASK_MID) {
        if (ClickKey(fd, BTN_MIDDLE) == -1) return -1;
    }
    if (mask & MOUSE_MASK_RIGHT) {
        if (ClickKey(fd, BTN_RIGHT) == -1) return -1;
    }
    return 0;
}

static int KeyDown(int fd, int key, int x, int y) {
    if (WriteInput(fd, EV_ABS, ABS_X, x) == -1) return -1;
    if (WriteInput(fd, EV_ABS, ABS_Y, y) == -1) return -1;
    if (WriteInput(fd, EV_KEY, key, 1) == -1) return -1;
    if (WriteInput(fd, EV_SYN, SYN_REPORT, 0) == -1) return -1;
    return 0;
}

static int KeyUp(int fd, int key, int x, int y) {
    if (WriteInput(fd, EV_ABS, ABS_X, x) == -1) return -1;
    if (WriteInput(fd, EV_ABS, ABS_Y, y) == -1) return -1;
    if (WriteInput(fd, EV_ABS, key, 0) == -1) return -1;
    if (WriteInput(fd, EV_SYN, SYN_REPORT, 0) == -1) return -1;
    return 0;
}

static int MouseDownEvent(int fd, int mask, int x, int y) {
    if (mask & MOUSE_MASK_LEFT) {
        if (KeyDown(fd, BTN_LEFT, x, y) == -1) {
            return -1;
        }
    }
    if (mask & MOUSE_MASK_MID) {
        if (KeyDown(fd, BTN_MIDDLE, x, y) == -1) {
            return -1;
        }
    }
    if (mask & MOUSE_MASK_RIGHT) {
        if (KeyDown(fd, BTN_RIGHT, x, y) == -1) {
            return -1;
        }
    }
    return 0;
}

static int MouseUpEvent(int fd, int mask, int x, int y) {
    if (mask & MOUSE_MASK_LEFT) {
        if (KeyUp(fd, BTN_LEFT, x, y) == -1) {
            return -1;
        }
    }
    if (mask & MOUSE_MASK_MID) {
        if (KeyUp(fd, BTN_MIDDLE, x, y) == -1) {
            return -1;
        }
    }
    if (mask & MOUSE_MASK_RIGHT) {
        if (KeyUp(fd, BTN_RIGHT, x, y) == -1) {
            return -1;
        }
    }
    return 0;
}
static int MouseDragEvent(int fd, int mask, int x, int y) {
    if (WriteInput(fd, EV_REL, REL_X, x) == -1) {
        return -1;
    }
    if (WriteInput(fd, EV_REL, REL_Y, y) == -1) {
        return -1;
    }
    if (WriteInput(fd, EV_SYN, SYN_REPORT, 0) == -1) {
        return -1;
    }
    return 0;
}

int IMouseEvent(int mask, int x, int y) {
    if (mask & MOUSE_MASK_CLICK) {
        if (MouseClickEvent(mouse_fd, mask, x, y) == -1) {
            return -1;
        }
    }
    if (mask & MOUSE_MASK_DOWN) {
        if (MouseDownEvent(mouse_fd, mask, x, y) == -1) {
            return -1;
        }
    }
    if (mask & MOUSE_MASK_UP) {
        if (MouseUpEvent(mouse_fd, mask, x, y) == -1) {
            return -1;
        }
    }
    if (mask & MOUSE_MASK_DRAG) {
        if (MouseDragEvent(mouse_fd, mask, x, y) == -1) {
            return -1;
        }
    }
    return 0;
}

int IMouseMove(int mask, int x, int y) {
    WriteInput(mouse_fd, mask, ABS_X, x);
    WriteInput(mouse_fd, mask, ABS_Y, y);
    WriteInput(mouse_fd, EV_SYN, SYN_REPORT, 0);
    return 0;
}

int SetInputMouseDevice(int screen_width, int screen_height) {
    // uInput device structure
    struct uinput_user_dev uinp;

    printf("mouse,SetInputMouseDevice\n");
    // Open the input device
    mouse_fd = open(UINPUT_DEVICVE, O_WRONLY | O_NDELAY);
    if (mouse_fd <= 0) {
        printf("mouse,Unable to open /dev/uinput\n");
        return -1;
    }

    // Intialize the uInput device to NULL
    memset(&uinp, 0x00, sizeof(uinp));
    strncpy(uinp.name, MOUSE_NAME, sizeof(uinp.name) - 1);
    uinp.id.bustype = BUS_USB;
    uinp.id.version = 4;
    uinp.id.vendor = MOUSE_VENDOR_ID;
    // mouse abs
    uinp.absmin[ABS_X] = 0;
    uinp.absmax[ABS_X] = screen_width;
    uinp.absmin[ABS_Y] = 0;
    uinp.absmax[ABS_Y] = screen_height;

    // we use abs axis
    ioctl(mouse_fd, UI_SET_EVBIT, EV_SYN);
    ioctl(mouse_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(mouse_fd, UI_SET_EVBIT, EV_ABS);
    ioctl(mouse_fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(mouse_fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(mouse_fd, UI_SET_ABSBIT, ABS_X);
    ioctl(mouse_fd, UI_SET_ABSBIT, ABS_Y);
    ioctl(mouse_fd,UI_SET_EVBIT,EV_REL);
    ioctl(mouse_fd,UI_SET_RELBIT,REL_WHEEL);

    // Create input device into input sub-system
    if (write(mouse_fd, &uinp, sizeof(uinp)) != sizeof(uinp)) {
        printf("mouse,First write returned fail.\n");
        return -1;
    }

    if (ioctl(mouse_fd, UI_DEV_CREATE)) {
        printf("mouse,ioctl UI_DEV_CREATE returned fail.\n");
        return -1;
    }
    mouse_enable = true;

    return 0;
}

int RemoveMouseDevice() {
    printf("mouse,RemoveMouseDevice\n");
    if (mouse_enable == false) return 1;

    if (ioctl(mouse_fd, UI_DEV_DESTROY)) {
        printf("mouse,ioctl UI_DEV_DESTROY returned fail.\n");
        return -1;
    }
    mouse_fd = 0;
    mouse_enable = false;
    return 0;
}

int IMouseWheelEvent(int direction) {
    if (WriteInput(mouse_fd, EV_REL, REL_WHEEL, direction) == -1) {
        return -1;
    }
    if (WriteInput(mouse_fd, EV_SYN, SYN_REPORT, 0) == -1) {
        return -1;
    }
    return 0;
}