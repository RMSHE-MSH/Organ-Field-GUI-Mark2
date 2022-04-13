#pragma once
#include "Universal.h"
#include "Window.h"
#include "OFMessage.h"

#define normal 4   //��������������ɱ��ж�;
#define Drag 5     //���������϶��е�״̬;

class OFSlider {
private:
	struct SliderStruct {
		int SliderNum = 0;                                     //��̬�仯��Slider������;
		short *SliderState = new short[SliderNum] { NULL };	   //����Slider״̬������,�����СΪ�ܹ�����Slider��������;
		float *LocationValue = new float[SliderNum] { NULL };  //����������򻬶���Ľ���(0~100);
	}Slider, ProgressBar;

	short SliderRim[6]{ NULL };			              //Slider������3��״̬�߿��ϸ����;
	COLORREF SliderRimColor[6]{ NULL };               //Slider������3��״̬�߿���ɫ����;

	COLORREF SliderFillColor[6]{ NULL };              //Slider������3��״̬�����ɫ����;
	COLORREF SliderBackColor[6]{ NULL };              //Slider������3��״̬������ɫ����;
	COLORREF SliderTextColor[6]{ NULL };              //Slider������3��״̬������ɫ����;
private:
	//��������Ⱦ(ָ������������,ָ��������״̬,ָ��������ͼ��);
	void SliderRender(int SliderName, int *DiagonalPoints, short state);
	//��⻬�����������״̬(����������,����������,�������);
	short SliderDetec(int SliderName, int *DiagonalPoints, ExMessage Mouse_Window);

	//����϶�������(����������,����������,�������);
	void SliderDrag(int SliderName, int *DiagonalPoints, ExMessage Mouse_Window);
	//��ӡ������������(ָ���ַ���,ָ���ַ�������);
	void DrawSliderText(int SliderName, LPCTSTR str, int *DiagonalPoints);
	//����������ʽ(�ָ�,����,������ɫ,����);
	void SetSliderTextStyle(LONG height, LONG weight, COLORREF textcolor, LPCTSTR font);
public:
	//���û����������������(ָ������������,����0~100);
	void SetSliderLocation(int SliderName, float LocationValue);
	//����һ���������������(ָ������������,����,���,�߶�,ָ��״̬(����ʱ��Ϊ������,����ʱ��Ϊ������));
	float CreateSlider(int SliderName, int x, int y, int width, int height, short SetState);
	//��ȡ���������������ǰ����(ָ�������������������);
	float PeekSliderLocation(int SliderName);
public:
	//��������״̬����ʽ(�߿��ߴ�ϸ,�߿�����ɫ,���������ɫ,����������ɫ);
	void SetNormalStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor);
	//���ý���״̬����ʽ(�߿��ߴ�ϸ,�߿�����ɫ,���������ɫ,����������ɫ);
	void SetDisableStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor);
	//������껮��״̬����ʽ(�߿��ߴ�ϸ,�߿�����ɫ,���������ɫ,����������ɫ);
	void SetHoverStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor);
	//��������϶�״̬����ʽ(�߿��ߴ�ϸ,�߿�����ɫ,���������ɫ,����������ɫ);
	void SetDragStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor);

	//��������ʽĬ�Ͽ�������;
	void SliderDefaultStyle();
};