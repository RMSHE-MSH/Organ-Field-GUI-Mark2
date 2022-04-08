#pragma once
#include "Universal.h"
#include "Window.h"

#define normal 4   //��ť��������;

class OFButton {
private:
	short ButtonRim[5]{ NULL };           //��ť5��״̬�߿��ϸ����;
	COLORREF ButtonRimColor[5]{ NULL };   //��ť5��״̬�߿���ɫ����;

	short ButtonFillStyle[5]{ NULL };     //��ť5��״̬�����ʽ����;
	short ButtonFillHatch[5]{ NULL };     //��ť5��״̬���ͼ������;
	COLORREF ButtonFillColor[5]{ NULL };  //��ť5��״̬�����ɫ����;'

	COLORREF ButtonTextColor[5]{ NULL };  //��ť5��״̬������ɫ����;
private:
	//��ť��Ⱦ(ָ����ť����,ָ����ť״̬,ָ����ťͼ��);
	void ButtonRender(int *DiagonalPoints, short state, IMAGE img);

	//����������ʽ(�ָ�,����,������ɫ,����);
	void SetButtonTextStyle(LONG height, LONG weight, COLORREF textcolor, LPCTSTR font);
	//��ӡ��ť����(ָ���ַ���,ָ���ַ�������);
	void DrawButtonText(LPCTSTR str, COLORREF textcolor, int *DiagonalPoints);
	//��ⰴť�������״̬(��ť����,�������);
	short ButtonDetec(int *DiagonalPoints, ExMessage Mouse_Window);
public:
	//����һ���޷���ֵ�İ�ť(��ť����,��ť����,��ť��,��ť��,���ð�ť״̬,����¼�);
	void CreateButton(string ButtonName, int x, int y, int width, int height, short SetState, void ClickEvent());
	//����һ��int���ͷ���ֵ�İ�ť(��ť����,��ť����,��ť��,��ť��,���ð�ť״̬,����¼�);
	int CreateRintButton(string ButtonName, int x, int y, int width, int height, short SetState, int ClickEvent());
public:
	//���ø���ť״̬����ʽ(��ť�߿��ߴ�ϸ,�߿�����ɫ,ָ�������ʽ,ָ�����ͼ��,ָ�������ɫ,ָ����ť������ɫ);
	void SetNormalStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetDisableStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetHoverStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetClickStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetLoadingStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);

	//��ť��ʽĬ�Ͽ�������;
	void ButtonDefaultStyle();
};
