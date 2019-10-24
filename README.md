# libdev
这个一个用于linux的虚拟键盘鼠标库
使用方法 
在代码中引用virsual_input.h 这个头文件以及type_def.# 

VirsualInput* vi = new VisualInput();
 
 接口说明：
 鼠标事件：
 1、按键
    int MouseEvent(int mask, int x, int y);
    使用示例： vi->MouseEvent(MOUSE_MASK_LEFT | MOUSE_MASK_CLICK,x y);
    这里的mask表示鼠标左键click一次，完整动作是一次down事件以及一次up事件，这里合成一次事件。
    x，y:是位置坐标。
    
 2、移动位置
 int MouseMove(int mask, int x, int y);
    使用示例：vi->(EV_ABS,x,y);
    mask ：固定EV_ABS 因为使用的是绝对坐标
    x，y 是屏幕位置坐标。

 3、滚轮事件
    int MouseWheelEvent(int direction);
    使用示例:vi->MouseWheelEvent(MOUSE_MASK_WHEEL_UP);
    direction:滚轮滚动方向
 4、键盘事件：

 int KeyEvent(int action, int keyCode);
    action：按键动作 down或者up事件
    keyCode：按键键值



