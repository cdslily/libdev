#include "keyboard_event.h"
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "type_def.h"

#define UINPUT_DEVICVE "/dev/uinput"
#define KEYPAD_NAME "linux_virtual_keyboard"
#define KEYPAD_VENDOR_ID 0x51
static int keyboard_fd = -1;
bool keyboard_enable = false;

int SetupKeyboardDevice() {
    struct uinput_user_dev uinp;  // uInput device structure
    int i;

    // Open the input device
    keyboard_fd = open(UINPUT_DEVICVE, O_WRONLY | O_NDELAY);
    if (keyboard_fd <= 0) {
        printf("Unable to open /dev/uinput\n");
        return -1;
    }

    // Intialize the uInput device to NULL
    memset(&uinp, 0x00, sizeof(uinp));
    strncpy(uinp.name, KEYPAD_NAME, sizeof(uinp.name) - 1);
    uinp.id.vendor = KEYPAD_VENDOR_ID;
    uinp.id.product = 0x0002;
    uinp.id.bustype = BUS_VIRTUAL;

    // Keyboard
    ioctl(keyboard_fd, UI_SET_EVBIT, EV_SYN);
    ioctl(keyboard_fd, UI_SET_EVBIT, EV_KEY);

    for (i = 0; i < KEY_MAX; i++) {
        if (ioctl(keyboard_fd, UI_SET_KEYBIT, i) == -1) {
            return -1;
        }
    }
    if (ioctl(keyboard_fd, UI_SET_EVBIT, EV_REP) == -1) {
        return -1;
    }

    // Create input device into input sub-system
    if (write(keyboard_fd, &uinp, sizeof(uinp)) != sizeof(uinp)) {
        printf("First write returned fail.\n");
        return -1;
    }

    if (ioctl(keyboard_fd, UI_DEV_CREATE)) {
        printf("ioctl UI_DEV_CREATE returned fail.\n");
        close(keyboard_fd);
        return -1;
    }
    keyboard_enable = true;
    return 1;
}

int RemoveKeyboardDevice() {
    printf("mouse,RemoveKeyboardDevice\n");
    if (keyboard_enable == false) return 1;

    if (ioctl(keyboard_fd, UI_DEV_DESTROY)) {
        printf("mouse,ioctl UI_DEV_DESTROY returned fail.\n");
        close(keyboard_fd);
        return -1;
    }
    keyboard_fd = 0;
    keyboard_enable = false;
    return 1;
}

int IKeyEvent(int action, int keyCode) {
    struct input_event key_event;
    int ret;

    memset(&key_event, 0, sizeof(struct input_event));

    gettimeofday(&key_event.time, NULL);
    key_event.type = EV_KEY;
    key_event.code = keyCode;
    key_event.value = action;
    ret = write(keyboard_fd, &key_event, sizeof(struct input_event));
    if (ret < 0) {
        printf("%s:%d\n", __func__, __LINE__);
        return ret;  // error process.
    }

    // event state sync
    gettimeofday(&key_event.time, NULL);
    key_event.type = EV_SYN;
    key_event.code = SYN_REPORT;
    key_event.value = 0;
    ret = write(keyboard_fd, &key_event, sizeof(struct input_event));
    if (ret < 0) {
        printf("%s:%d\n", __func__, __LINE__);
        return ret;  // error process.
    }

    return 0;
}
