#include "OFButton.h"
Window Win_m;

void OFButton::CreateButton(string ButtonName, int x, int y, int width, int height, short SetState, void ClickEvent()) {
	int rec[] = { x, y ,x + width ,y + height };//转换为矩形;
	SetButtonTextStyle(height / 2, FW_MEDIUM, ButtonTextColor[SetState], "微软雅黑");//设置按钮字体样式;

	short Event = ButtonDetec(rec, Win_m.GetWindowMouse());//检测按钮状态;

	if (SetState != disable && SetState != loading) {
		ButtonRender(rec, Event, NULL);
		DrawButtonText(ButtonName.c_str(), RGB(0, 0, 0), rec);
	} else {
		ButtonRender(rec, SetState, NULL);
		DrawButtonText(ButtonName.c_str(), RGB(131, 131, 131), rec);
	}

	if (Event == click) { while (1) { if (!KEY_DOWN(VK_LBUTTON))break; }ClickEvent(); }//如果按钮被点击后并且被释放则执行事件;
}

void OFButton::SetNormalStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor) {
	ButtonRim[normal] = buttonrim;
	ButtonRimColor[normal] = rimcolor;
	ButtonFillStyle[normal] = fillstyle;

	ButtonFillHatch[normal] = fillhatch;
	ButtonFillColor[normal] = fillcolor;

	ButtonTextColor[normal] = textcolor;
}
void OFButton::SetDisableStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor) {
	ButtonRim[disable] = buttonrim;
	ButtonRimColor[disable] = rimcolor;

	ButtonFillStyle[disable] = fillstyle;
	ButtonFillHatch[disable] = fillhatch;
	ButtonFillColor[disable] = fillcolor;

	ButtonTextColor[disable] = textcolor;
}
void OFButton::SetHoverStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor) {
	ButtonRim[hover] = buttonrim;
	ButtonRimColor[hover] = rimcolor;

	ButtonFillStyle[hover] = fillstyle;
	ButtonFillHatch[hover] = fillhatch;
	ButtonFillColor[hover] = fillcolor;

	ButtonTextColor[hover] = textcolor;
}
void OFButton::SetClickStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor) {
	ButtonRim[click] = buttonrim;
	ButtonRimColor[click] = rimcolor;

	ButtonFillStyle[click] = fillstyle;
	ButtonFillHatch[click] = fillhatch;
	ButtonFillColor[click] = fillcolor;

	ButtonTextColor[click] = textcolor;
}
void OFButton::SetLoadingStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor, COLORREF textcolor) {
	ButtonRim[loading] = buttonrim;
	ButtonRimColor[loading] = rimcolor;

	ButtonFillStyle[loading] = fillstyle;
	ButtonFillHatch[loading] = fillhatch;
	ButtonFillColor[loading] = fillcolor;

	ButtonTextColor[loading] = textcolor;
}

void OFButton::ButtonRender(int *DiagonalPoints, short state, IMAGE img) {
	if (state == normal) {//激活状态的按钮;
		setfillstyle(ButtonFillStyle[normal], ButtonFillHatch[normal], &img); setfillcolor(ButtonFillColor[normal]);
		if (ButtonRim[normal] != 0) {
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ButtonRim[normal]); setlinecolor(ButtonRimColor[normal]);
			fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		} else {
			solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		}
	} else if (state == disable) {//禁用的按钮;
		setfillstyle(ButtonFillStyle[disable], ButtonFillHatch[disable], &img); setfillcolor(ButtonFillColor[disable]);
		if (ButtonRim[disable] != 0) {
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ButtonRim[disable]); setlinecolor(ButtonRimColor[disable]);
			fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		} else {
			solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		}
	} else if (state == hover) {//鼠标划过的按钮;
		setfillstyle(ButtonFillStyle[hover], ButtonFillHatch[hover], &img); setfillcolor(ButtonFillColor[hover]);
		if (ButtonRim[hover] != 0) {
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ButtonRim[hover]); setlinecolor(ButtonRimColor[hover]);
			fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		} else {
			solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		}
	} else if (state == click) {//鼠标点击的按钮;
		setfillstyle(ButtonFillStyle[click], ButtonFillHatch[click], &img); setfillcolor(ButtonFillColor[click]);
		if (ButtonRim[click] != 0) {
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ButtonRim[click]); setlinecolor(ButtonRimColor[click]);
			fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		} else {
			solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		}
	} else if (state == loading) {//加载状态的按钮;
		setfillstyle(ButtonFillStyle[loading], ButtonFillHatch[loading], &img); setfillcolor(ButtonFillColor[loading]);
		if (ButtonRim[loading] != 0) {
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ButtonRim[loading]); setlinecolor(ButtonRimColor[loading]);
			fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		} else {
			solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		}
	} else { cout << "RMSHE: Error! non-existent button state." << endl; }//错误:不存在的按钮状态;
}

void OFButton::SetButtonTextStyle(LONG height, LONG weight, COLORREF textcolor, LPCTSTR font) {
	setbkmode(TRANSPARENT);
	LOGFONT f; gettextstyle(&f);
	f.lfHeight = height;
	f.lfQuality = ANTIALIASED_QUALITY | PROOF_QUALITY, f.lfCharSet = CHINESEBIG5_CHARSET;
	_tcscpy_s(f.lfFaceName, font);
	settextstyle(&f); settextcolor(textcolor);
}

void OFButton::DrawButtonText(LPCTSTR str, COLORREF textcolor, int *DiagonalPoints) {
	RECT r = { DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3] };
	drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

short OFButton::ButtonDetec(int *DiagonalPoints, ExMessage Mouse_Window) {
	if (Mouse_Window.x > DiagonalPoints[0] && Mouse_Window.y > DiagonalPoints[1] && Mouse_Window.x < DiagonalPoints[2] && Mouse_Window.y < DiagonalPoints[3]) {
		if (KEY_DOWN(VK_LBUTTON)) { return click; } else { return hover; }
	}
	return normal;
}

void OFButton::ButtonDefaultStyle() {
	SetNormalStyle(1, RGB(180, 180, 180), BS_SOLID, NULL, RGB(225, 225, 225), RGB(30, 30, 30));
	SetDisableStyle(1, RGB(191, 191, 191), BS_SOLID, NULL, RGB(204, 204, 204), RGB(130, 130, 130));
	SetHoverStyle(1, RGB(180, 180, 180), BS_SOLID, NULL, RGB(229, 243, 255), RGB(30, 30, 30));
	SetClickStyle(1, RGB(10, 89, 247), BS_SOLID, NULL, RGB(204, 232, 255), RGB(30, 30, 30));
	SetLoadingStyle(1, RGB(10, 89, 247), BS_SOLID, NULL, RGB(204, 204, 204), RGB(130, 130, 130));
}