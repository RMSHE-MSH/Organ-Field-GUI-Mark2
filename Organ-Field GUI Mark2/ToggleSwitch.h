#pragma once
#include "Universal.h"
#include "Window.h"

#define close 4    //拨动开关关闭状态;
#define open 5     //拨动开关开启状态;

class ToggleSwitch {
private:
	int ToggleNum = 0;                                   //动态变化的ToggleSwitch最大个数;
	short *ToggleState = new short[ToggleNum] { NULL };	 //保存ToggleSwitch状态的数组,数组大小为能够创建ToggleSwitch的最大个数;

	short ToggleRim[6]{ NULL };			              //开关6种状态边框粗细储存;
	COLORREF ToggleRimColor[6]{ NULL };               //开关6种状态边框颜色储存;

	short ToggleFillStyle[6]{ NULL };                 //开关6种状态填充样式储存;
	COLORREF ToggleFillColor[6]{ NULL };              //开关6种状态填充颜色储存;

	COLORREF SliderColor[6]{ NULL };                  //开关6种状态的滑块颜色储存;
private:
	//开关渲染(指定按钮矩形,指定按钮状态);
	void ToggleRender(int ToggleName, int *DiagonalPoints, short state);
	//检测按钮对鼠标活动的状态(按钮矩形,鼠标坐标);
	short ToggleDetec(int ToggleName, int *DiagonalPoints, ExMessage Mouse_Window);
public:
	//创建一个int类型返回值的拨动开关(开关名称,开关坐标,设置开关初始状态,打开事件);
	int CreateToggle(int ToggleName, int x, int y, short SetInitialState, int (*EventFunc)());
public:
	//设置关闭状态的样式(开关边框线粗细,边框线颜色,滑块颜色);
	void SetCloseStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);
	//设置打开状态的样式(填充颜色,滑块颜色);
	void SetOpenStyle(COLORREF fillcolor, COLORREF slidercolor);
	//设置禁用状态的样式(开关边框线粗细,边框线颜色,滑块颜色);
	void SetDisableStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);
	//设置鼠标划过状态的样式(开关边框线粗细,开关主题色颜色);
	void SetHoverStyle(short Togglerim, COLORREF MainColor);
	//设置加载状态的样式(开关边框线粗细,边框线颜色,滑块颜色);
	void SetLoadingStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor);

	//开关样式默认快速设置;
	void ToggleDefaultStyle();

	//获取开关当前状态;
	short GetToggleState(int ToggleName);
	//设置开关的状态;
	void SetToggleState(int ToggleName, short SetState);
};
