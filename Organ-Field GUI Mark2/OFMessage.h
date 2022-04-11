//新的集成鼠标键盘窗口消息调度中心,将逐渐弃用Windows类的同类函数;
#pragma once
#include "Universal.h"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)  //定义鼠标点击状态;

class OFMessage {
private:
	//获取相对于窗口的鼠标消息的循环进程;
	void PeekOFMessage_Tree();
public:
	//开启获取相对于窗口的鼠标消息循环函数线程;
	void PeekOFMessageUP();
	//立即返回相对于窗口的鼠标消息;
	ExMessage PeekOFMessage();
};
