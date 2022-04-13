#pragma once
#include "Universal.h"
#include "Window.h"
#include "OFMessage.h"

#define normal 4   //滑动条正常激活可被托动;
#define Drag 5     //滑动条被拖动中的状态;

class OFSlider {
private:
	struct SliderStruct {
		int SliderNum = 0;                                     //动态变化的Slider最大个数;
		short *SliderState = new short[SliderNum] { NULL };	   //保存Slider状态的数组,数组大小为能够创建Slider的最大个数;
		float *LocationValue = new float[SliderNum] { NULL };  //储存进度条或滑动块的进度(0~100);
	}Slider, ProgressBar;

	short SliderRim[6]{ NULL };			              //Slider滑动条3种状态边框粗细储存;
	COLORREF SliderRimColor[6]{ NULL };               //Slider滑动条3种状态边框颜色储存;

	COLORREF SliderFillColor[6]{ NULL };              //Slider滑动条3种状态填充颜色储存;
	COLORREF SliderBackColor[6]{ NULL };              //Slider滑动条3种状态背景颜色储存;
	COLORREF SliderTextColor[6]{ NULL };              //Slider滑动条3种状态文字颜色储存;
private:
	//滑动条渲染(指定滑动条矩形,指定滑动条状态,指定滑动条图标);
	void SliderRender(int SliderName, int *DiagonalPoints, short state);
	//检测滑动条对鼠标活动的状态(滑动条名称,滑动条矩形,鼠标坐标);
	short SliderDetec(int SliderName, int *DiagonalPoints, ExMessage Mouse_Window);

	//鼠标拖动滑动条(滑动条名称,滑动条矩形,鼠标坐标);
	void SliderDrag(int SliderName, int *DiagonalPoints, ExMessage Mouse_Window);
	//打印滑条进度文字(指定字符串,指定字符串矩形);
	void DrawSliderText(int SliderName, LPCTSTR str, int *DiagonalPoints);
	//设置字体样式(字高,字重,字体颜色,字体);
	void SetSliderTextStyle(LONG height, LONG weight, COLORREF textcolor, LPCTSTR font);
public:
	//设置滑动条或进度条进度(指定滑动条名称,进度0~100);
	void SetSliderLocation(int SliderName, float LocationValue);
	//创建一个滑动条或进度条(指定滑动条名称,坐标,宽度,高度,指定状态(正常时作为滑动条,禁用时作为进度条));
	float CreateSlider(int SliderName, int x, int y, int width, int height, short SetState);
	//获取滑动条或进度条当前进度(指定滑动条或进度条名称);
	float PeekSliderLocation(int SliderName);
public:
	//设置正常状态的样式(边框线粗细,边框线颜色,滑条填充颜色,进度字体颜色);
	void SetNormalStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor);
	//设置禁用状态的样式(边框线粗细,边框线颜色,滑条填充颜色,进度字体颜色);
	void SetDisableStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor);
	//设置鼠标划过状态的样式(边框线粗细,边框线颜色,滑条填充颜色,进度字体颜色);
	void SetHoverStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor);
	//设置鼠标拖动状态的样式(边框线粗细,边框线颜色,滑条填充颜色,进度字体颜色);
	void SetDragStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor);

	//滑动条样式默认快速设置;
	void SliderDefaultStyle();
};