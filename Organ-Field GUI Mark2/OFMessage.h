//�µļ��������̴�����Ϣ��������,��������Windows���ͬ�ຯ��;
#pragma once
#include "Universal.h"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)  //���������״̬;

class OFMessage {
private:
	//��ȡ����ڴ��ڵ������Ϣ��ѭ������;
	void PeekOFMessage_Tree();
public:
	//������ȡ����ڴ��ڵ������Ϣѭ�������߳�;
	void PeekOFMessageUP();
	//������������ڴ��ڵ������Ϣ;
	ExMessage PeekOFMessage();
};
