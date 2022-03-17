#pragma once
#include "Universal.h"
class Window {
public:
	HWND Handle;             //定义窗口句柄;
	RECT Rect;               //定义窗口边距;
	static int GetWidth();   //获取窗口宽度;
	static int GetHeight();  //获取窗口高度;
	void GetWindowRect();    //获取窗口边缘坐标;

	void Initialize_Window(int Width, int Height, int Flag);          //初始化窗口;
	void Reset_Window(int Width, int Height, COLORREF Window_Color);  //重设置窗口;
	void SetWindowTilte(const string &Title);                       //设置窗口标题;
	void MoveWindow(int X, int Y);                                  //设置窗口位置;

	POINT Mouse_Screen;          //相对屏幕的鼠标坐标;
	void GetScreenMouse();       //获取相对于屏幕的鼠标坐标;
	ExMessage GetWindowMouse();  //获取相对于窗口的鼠标消息,如果没有消息则一直等待;
	ExMessage PeekWindowMouse(); //获取相对于窗口的鼠标消息,并立即返回;
};
