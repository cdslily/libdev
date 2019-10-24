#ifndef _VIRSUAL_INPUT_
#define _VIRSUAL_INPUT_

class VirsualInput {
   public:
    VirsualInput() { InitVirtualDev(); }
    VirsualInput(int width, int height) { InitVirtualDev(width, height); }
    ~VirsualInput() { ReleaseVirtualDev(); }

    void InitVirtualDev(int screen_width, int screen_height);

    void InitVirtualDev();
    void ReleaseVirtualDev();

    // see type_def.h  MOUSE_MASK
    int MouseEvent(int mask, int x, int y);
    int MouseMove(int mask, int x, int y);
    // action 1 down 0 up  keyCode @see linux/input.h input_event-codes.h
    int KeyEvent(int action, int keyCode);
    int MouseWheelEvent(int direction);
    void LoadScreenInfo(int *width, int *height);
};

#endif