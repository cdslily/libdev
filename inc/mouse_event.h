#ifndef _LINUX_MOUSE_EVENT_
#define _LINUX_MOUSE_EVENT_
int SetInputMouseDevice(int screen_width, int screen_height);
int RemoveMouseDevice();

int IMouseEvent(int mask, int x, int y);
int IMouseMove(int mask, int x, int y);
int IMouseWheelEvent(int direction);
#endif