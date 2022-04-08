//新的集成鼠标键盘窗口消息调度中心,将逐渐弃用Windows类的同类函数;
#pragma once
#include "Universal.h"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)  //定义鼠标点击状态;

class OFMessage {
private:
	void PeekWindowMouse_Tree();
public:
	void PeekWindowMouse();
	ExMessage GetMouseMessage();
};
