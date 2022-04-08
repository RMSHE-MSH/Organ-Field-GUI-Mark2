#pragma once
#include "Universal.h"
#include "Window.h"

#define normal 4   //按钮正常激活;

class OFButton {
private:
	short ButtonRim[5]{ NULL };           //按钮5种状态边框粗细储存;
	COLORREF ButtonRimColor[5]{ NULL };   //按钮5种状态边框颜色储存;

	short ButtonFillStyle[5]{ NULL };     //按钮5种状态填充样式储存;
	short ButtonFillHatch[5]{ NULL };     //按钮5种状态填充图案储存;
	COLORREF ButtonFillColor[5]{ NULL };  //按钮5种状态填充颜色储存;'

	COLORREF ButtonTextColor[5]{ NULL };  //按钮5种状态字体颜色储存;
private:
	//按钮渲染(指定按钮矩形,指定按钮状态,指定按钮图标);
	void ButtonRender(int *DiagonalPoints, short state, IMAGE img);

	//设置字体样式(字高,字重,字体颜色,字体);
	void SetButtonTextStyle(LONG height, LONG weight, COLORREF textcolor, LPCTSTR font);
	//打印按钮文字(指定字符串,指定字符串矩形);
	void DrawButtonText(LPCTSTR str, COLORREF textcolor, int *DiagonalPoints);
	//检测按钮对鼠标活动的状态(按钮矩形,鼠标坐标);
	short ButtonDetec(int *DiagonalPoints, ExMessage Mouse_Window);
public:
	//创建一个无返回值的按钮(按钮名称,按钮坐标,按钮宽,按钮长,设置按钮状态,点击事件);
	void CreateButton(string ButtonName, int x, int y, int width, int height, short SetState, void ClickEvent());
	//创建一个int类型返回值的按钮(按钮名称,按钮坐标,按钮宽,按钮长,设置按钮状态,点击事件);
	int CreateRintButton(string ButtonName, int x, int y, int width, int height, short SetState, int ClickEvent());
public:
	//设置各按钮状态的样式(按钮边框线粗细,边框线颜色,指定填充样式,指定填充图案,指定填充颜色,指定按钮字体颜色);
	void SetNormalStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetDisableStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetHoverStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetClickStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);
	void SetLoadingStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor);

	//按钮样式默认快速设置;
	void ButtonDefaultStyle();
};
