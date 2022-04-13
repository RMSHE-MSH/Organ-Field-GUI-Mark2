#pragma once
#include "Universal.h"
#include "Window.h"

#define close 4    //�������عر�״̬;
#define open 5     //�������ؿ���״̬;

class ToggleSwitch {
private:
	int ToggleNum = 0;                                   //��̬�仯��ToggleSwitch������;
	short *ToggleState = new short[ToggleNum] { NULL };	 //����ToggleSwitch״̬������,�����СΪ�ܹ�����ToggleSwitch��������;

	short ToggleRim[6]{ NULL };			              //����6��״̬�߿��ϸ����;
	COLORREF ToggleRimColor[6]{ NULL };               //����6��״̬�߿���ɫ����;

	short ToggleFillStyle[6]{ NULL };                 //����6��״̬�����ʽ����;
	COLORREF ToggleFillColor[6]{ NULL };              //����6��״̬�����ɫ����;

	COLORREF SliderColor[6]{ NULL };                  //����6��״̬�Ļ�����ɫ����;
private:
	//������Ⱦ(ָ����ť����,ָ����ť״̬);
	void ToggleRender(int ToggleName, int *DiagonalPoints, short state);
	//��ⰴť�������״̬(��ť����,�������);
	short ToggleDetec(int ToggleName, int *DiagonalPoints, ExMessage Mouse_Window);
public:
	//����һ��int���ͷ���ֵ�Ĳ�������(��������,��������,���ÿ��س�ʼ״̬,���¼�);
	int CreateToggle(int ToggleName, int x, int y, short SetInitialState, int (*EventFunc)());
public:
	//���ùر�״̬����ʽ(���ر߿��ߴ�ϸ,�߿�����ɫ,������ɫ);
	void SetCloseStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);
	//���ô�״̬����ʽ(�����ɫ,������ɫ);
	void SetOpenStyle(COLORREF fillcolor, COLORREF slidercolor);
	//���ý���״̬����ʽ(���ر߿��ߴ�ϸ,�߿�����ɫ,������ɫ);
	void SetDisableStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);
	//������껮��״̬����ʽ(���ر߿��ߴ�ϸ,��������ɫ��ɫ);
	void SetHoverStyle(short Togglerim, COLORREF MainColor);
	//���ü���״̬����ʽ(���ر߿��ߴ�ϸ,�߿�����ɫ,������ɫ);
	void SetLoadingStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);

	//������ʽĬ�Ͽ�������;
	void ToggleDefaultStyle();

	//��ȡ���ص�ǰ״̬;
	short GetToggleState(int ToggleName);
	//���ÿ��ص�״̬;
	void SetToggleState(int ToggleName, short SetState);
};
