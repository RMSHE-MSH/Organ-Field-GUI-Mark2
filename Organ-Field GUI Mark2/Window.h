#pragma once
#include "Universal.h"

class Window {
private:
	HWND hWnd;                      //窗口句柄;
	RECT Rect;                      //窗口边缘矩形;
	POINT Mouse_Screen;             //储存相对屏幕的鼠标坐标;
public:
	//获取窗口宽度;
	static int GetWidth();
	//获取窗口高度;
	static int GetHeight();
	//获取窗口边缘矩形;
	RECT GetWindowRect();

	//获取窗口句柄;
	HWND GetWindowHWND();
	//使窗口置顶;
	void WindowOnTop();

	//初始化窗口(窗口宽度,窗口高度,绘图窗口的样式);
	void Initialize_Window(int Width, int Height, int Flag);
	//重设置窗口(窗口宽度,窗口高度);
	void Reset_Window(int Width, int Height);
	//设置窗口标题(标题名);
	void SetWindowTilte(const string &Title);
	//设置窗口位置(坐标);
	void MoveWindow(int X, int Y);
	//设置窗口样式(窗口背景颜色,窗口绘图缩放因子);
	void SetWindowStyle(COLORREF backdrop_color, float xasp, float yasp);

	//新的集成鼠标键盘窗口消息调度中心已经在OFMessage中,将逐渐弃用Windows类的同类函数;

	//获取相对于屏幕的鼠标坐标;
	void GetScreenMouse();
public:// Windows API;
};
