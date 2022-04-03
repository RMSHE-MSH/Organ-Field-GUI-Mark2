#include "Window.h"

int Window::GetWidth() { return ::getwidth(); }
int Window::GetHeight() { return ::getheight(); }
RECT Window::GetWindowRect() { ::GetWindowRect(hWnd, &Rect); return Rect; }

HWND Window::GetWindowHWND() { HWND hWnd = ::GetForegroundWindow(); return hWnd; }
void Window::WindowOnTop() { ::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE); }

//初始化 EasyX Graphics Library 画版;
void Window::Initialize_Window(int Width, int Height, int Flag) {
	hWnd = ::initgraph(Width, Height, Flag);
	setorigin(0, 0);
	setbkcolor(RGB(241, 243, 245));
	cleardevice();
	setaspectratio(1, 1);
	::setbkmode(TRANSPARENT);
}

void Window::Reset_Window(int Width, int Height) {
	Resize(NULL, Width, Height);
	cleardevice();
}

void Window::SetWindowTilte(const string &Title) { ::SetWindowText(hWnd, Title.c_str()); }

void Window::MoveWindow(int X, int Y) { ::MoveWindow(hWnd, X, Y, GetWidth(), GetHeight(), 0); }

void Window::SetWindowStyle(COLORREF backdrop_color, float xasp, float yasp) {
	setbkcolor(backdrop_color); setaspectratio(xasp, yasp); cleardevice();
}

//获取相对于屏幕的鼠标坐标;
void Window::GetScreenMouse() { GetCursorPos(&Mouse_Screen); }

//获取相对于窗口的鼠标消息;
ExMessage Window::GetWindowMouse() { return getmessage(EM_MOUSE); }
ExMessage Window::PeekWindowMouse() { peekmessage(&Mouse_Window); return Mouse_Window; }