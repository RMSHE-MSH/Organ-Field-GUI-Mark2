#pragma once
#include "Universal.h"
class Window {
public:
	HWND Handle;             //���崰�ھ��;
	RECT Rect;               //���崰�ڱ߾�;
	static int GetWidth();   //��ȡ���ڿ��;
	static int GetHeight();  //��ȡ���ڸ߶�;
	void GetWindowRect();    //��ȡ���ڱ�Ե����;

	void Initialize_Window(int Width, int Height, int Flag);          //��ʼ������;
	void Reset_Window(int Width, int Height, COLORREF Window_Color);  //�����ô���;
	void SetWindowTilte(const string &Title);                       //���ô��ڱ���;
	void MoveWindow(int X, int Y);                                  //���ô���λ��;

	POINT Mouse_Screen;          //�����Ļ���������;
	void GetScreenMouse();       //��ȡ�������Ļ���������;
	ExMessage GetWindowMouse();  //��ȡ����ڴ��ڵ������Ϣ,���û����Ϣ��һֱ�ȴ�;
	ExMessage PeekWindowMouse(); //��ȡ����ڴ��ڵ������Ϣ,����������;
};
