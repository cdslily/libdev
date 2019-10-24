#include "virsual_input.h"
#include "keyboard_event.h"
#include "linux/fb.h"
#include "mouse_event.h"
#include "stdio.h"
#include "sys/ioctl.h"

void VirsualInput::InitVirtualDev() {
    SetupKeyboardDevice();
    int x = 0;
    int y = 0;
    LoadScreenInfo(&x, &y);
    InitVirtualDev(x, y);
}

void VirsualInput::InitVirtualDev(int screen_width, int screen_height) {
    SetupKeyboardDevice();
    if (screen_width > 0 && screen_height > 0) {
        SetInputMouseDevice(screen_width, screen_height);
        return;
    }
    SetInputMouseDevice(1920, 1080);
    printf(
        "init dev,error screen width %d and height %d \n use default config "
        "1920 * 1080 \n",
        screen_width, screen_height);
}

void VirsualInput::ReleaseVirtualDev() {
    RemoveKeyboardDevice();
    RemoveMouseDevice();
}

int VirsualInput::MouseEvent(int mask, int x, int y) {
    return MouseEvent(mask, x, y);
}

int VirsualInput::MouseMove(int mask, int x, int y) {
    return MouseMove(mask, x, y);
}

int VirsualInput::KeyEvent(int action, int keyCode) {
    return KeyEvent(action, keyCode);
}

int VirsualInput::MouseWheelEvent(int direction)  {
    return IMouseWheelEvent(direction);
}