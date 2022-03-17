#pragma once
#include "Universal.h"

#define normal 0   //按钮正常激活;
#define disable 1  //按钮禁用;
#define hover 2    //鼠标划过;
#define click 3    //鼠标点击;
#define loading 4  //按钮加载态;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)  //定义鼠标点击状态;

class OFButton {
private:
	short ButtonRim[5]{ NULL };           //按钮5种状态边框粗细储存;
	COLORREF ButtonRimColor[5]{ NULL };   //按钮5种状态边框颜色储存;

	short ButtonFillStyle[5]{ NULL };     //按钮5种状态填充样式储存;
	short ButtonFillHatch[5]{ NULL };     //按钮5种状态填充图案储存;
	COLORREF ButtonFillColor[5]{ NULL };  //按钮5种状态填充颜色储存;
public:
	string ButtonName;
	void CreateButton(string ButtonName);
public:
	//设置各按钮状态的样式(按钮边框线粗细,边框线颜色,指定填充样式,指定填充图案,指定填充颜色);
	void SetNormalStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor);
	void SetDisableStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor);
	void SetHoverStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor);
	void SetClickStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor);
	void SetLoadingStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor);

	//按钮渲染(指定按钮矩形,指定按钮状态,指定按钮图标);
	void ButtonRender(int *DiagonalPoints, short state, IMAGE img);
	//设置字体样式(字高,字重,字体颜色,字体);
	void SetButtonTextStyle(LONG height, LONG weight, COLORREF textcolor, LPCTSTR font);
	//打印按钮文字(指定字符串,指定字符串矩形);
	void DrawButtonText(LPCTSTR str, COLORREF textcolor, int *DiagonalPoints);
	//检测按钮对鼠标活动的状态(按钮矩形,鼠标坐标);
	short ButtonDetec(int *DiagonalPoints, ExMessage Mouse_Window);
};
