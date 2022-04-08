#pragma once
#include "Universal.h"
#include "Window.h"

#define close 4    //�������عر�״̬;
#define open 5     //�������ؿ���״̬;

class ToggleSwitch {
private:
	bool SliderState[1000]{ 0 };          //����ToggleSwitch����״̬������,�����СΪ�ܹ�����ToggleSwitch��������;

	short ToggleRim[6]{ NULL };           //����6��״̬�߿��ϸ����;
	COLORREF ToggleRimColor[6]{ NULL };   //����6��״̬�߿���ɫ����;

	short ToggleFillStyle[6]{ NULL };     //����6��״̬�����ʽ����;
	COLORREF ToggleFillColor[6]{ NULL };  //����6��״̬�����ɫ����;

	COLORREF SliderColor[6]{ NULL };      //����6��״̬�Ļ�����ɫ����;
private:
	//������Ⱦ(ָ����ť����,ָ����ť״̬);
	void ToggleRender(int *DiagonalPoints, short state);
	//��ⰴť�������״̬(��ť����,�������);
	short ToggleDetec(int ToggleName, int *DiagonalPoints, ExMessage Mouse_Window);
public:
	//����һ���޷���ֵ�Ĳ�������(��������,��������,���ð�ť״̬,���¼�);
	void CreateToggle(int ToggleName, int x, int y, short SetState, void ClickEvent());
public:
	//���ùر�״̬����ʽ(���ر߿��ߴ�ϸ,�߿�����ɫ,������ɫ);
	void SetCloseStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);
	//���ô�״̬����ʽ(�����ɫ,������ɫ);
	void SetOpenStyle(COLORREF fillcolor, COLORREF slidercolor);
	//���ý���״̬����ʽ(���ر߿��ߴ�ϸ,�߿�����ɫ,������ɫ);
	void SetDisableStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);
	//������껮��״̬����ʽ(���ر߿��ߴ�ϸ,�߿�����ɫ);
	void SetHoverStyle(short Togglerim, COLORREF rimcolor);
	//���ü���״̬����ʽ(���ر߿��ߴ�ϸ,�߿�����ɫ,������ɫ);
	void SetLoadingStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);

	//������ʽĬ�Ͽ�������;
	void ToggleDefaultStyle();
};
