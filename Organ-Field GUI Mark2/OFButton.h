#pragma once
#include "Universal.h"
#include "Window.h"

#define normal 0   //��ť��������;
#define disable 1  //��ť����;
#define hover 2    //��껮��;
#define click 3    //�����;
#define loading 4  //��ť����̬;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)  //���������״̬;

class OFButton {
private:
	short ButtonRim[5]{ NULL };           //��ť5��״̬�߿��ϸ����;
	COLORREF ButtonRimColor[5]{ NULL };   //��ť5��״̬�߿���ɫ����;

	short ButtonFillStyle[5]{ NULL };     //��ť5��״̬�����ʽ����;
	short ButtonFillHatch[5]{ NULL };     //��ť5��״̬���ͼ������;
	COLORREF ButtonFillColor[5]{ NULL };  //��ť5��״̬�����ɫ����;

	COLORREF ButtonTextColor[5]{ NULL };  //��ť5��״̬������ɫ����;
public:
	//����һ����ť(��ť����,��ť����,��ť��,��ť��,���ð�ť״̬,����¼�);
	void CreateButton(string ButtonName, int x, int y, int width, int height, short SetState, void ClickEvent());
public:
	//���ø���ť״̬����ʽ(��ť�߿��ߴ�ϸ,�߿�����ɫ,ָ�������ʽ,ָ�����ͼ��,ָ�������ɫ,ָ����ť������ɫ);
	void SetNormalStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetDisableStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetHoverStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetClickStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetLoadingStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);

	//��ť��Ⱦ(ָ����ť����,ָ����ť״̬,ָ����ťͼ��);
	void ButtonRender(int *DiagonalPoints, short state, IMAGE img);
	//����������ʽ(�ָ�,����,������ɫ,����);
	void SetButtonTextStyle(LONG height, LONG weight, COLORREF textcolor, LPCTSTR font);
	//��ӡ��ť����(ָ���ַ���,ָ���ַ�������);
	void DrawButtonText(LPCTSTR str, COLORREF textcolor, int *DiagonalPoints);
	//��ⰴť�������״̬(��ť����,�������);
	short ButtonDetec(int *DiagonalPoints, ExMessage Mouse_Window);

	//��ť��ʽĬ�Ͽ�������;
	void ButtonDefaultStyle();
};
