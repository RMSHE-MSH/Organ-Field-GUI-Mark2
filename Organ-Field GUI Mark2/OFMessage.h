//�µļ��������̴�����Ϣ��������,��������Windows���ͬ�ຯ��;
#pragma once
#include "Universal.h"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)  //���������״̬;

class OFMessage {
private:
	void PeekWindowMouse_Tree();
public:
	void PeekWindowMouse();
	ExMessage GetMouseMessage();
};
