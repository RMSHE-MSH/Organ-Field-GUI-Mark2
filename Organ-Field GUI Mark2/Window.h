#pragma once
#include "Universal.h"
class Window {
private:
	HWND hWnd;                      //���ھ��;
	RECT Rect;                      //���ڱ�Ե����;
	POINT Mouse_Screen;             //���������Ļ���������;
	ExMessage Mouse_Window{ NULL }; //������Դ��ڵ������Ϣ;
public:
	//��ȡ���ڿ��;
	static int GetWidth();
	//��ȡ���ڸ߶�;
	static int GetHeight();
	//��ȡ���ڱ�Ե����;
	RECT GetWindowRect();

	//��ȡ���ھ��;
	HWND GetWindowHWND();
	//ʹ�����ö�;
	void WindowOnTop();

	//��ʼ������(���ڿ��,���ڸ߶�,��ͼ���ڵ���ʽ);
	void Initialize_Window(int Width, int Height, int Flag);
	//�����ô���(���ڿ��,���ڸ߶�);
	void Reset_Window(int Width, int Height);
	//���ô��ڱ���(������);
	void SetWindowTilte(const string &Title);
	//���ô���λ��(����);
	void MoveWindow(int X, int Y);
	//���ô�����ʽ(���ڱ�����ɫ,���ڻ�ͼ��������);
	void SetWindowStyle(COLORREF backdrop_color, float xasp, float yasp);

	//��ȡ�������Ļ���������;
	void GetScreenMouse();
	//��ȡ����ڴ��ڵ������Ϣ,���û����Ϣ��һֱ�ȴ�;
	ExMessage GetWindowMouse();
	//��ȡ����ڴ��ڵ������Ϣ,����������;
	ExMessage PeekWindowMouse();

public:// Windows API;
};
