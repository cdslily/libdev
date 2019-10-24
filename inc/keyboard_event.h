#ifndef _LINUX_KEYBOARD_EVENT_
#define _LINUX_KEYBOARD_EVENT_

int SetupKeyboardDevice();
int RemoveKeyboardDevice();
int IKeyEvent(int action,int keyCode);
#endif