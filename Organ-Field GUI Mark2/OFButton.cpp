#include "OFButton.h"

void OFButton::SetNormalStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor) {
	ButtonRim[normal] = buttonrim;
	ButtonRimColor[normal] = rimcolor;
	ButtonFillStyle[normal] = fillstyle;
	ButtonFillHatch[normal] = fillhatch;
	ButtonFillColor[normal] = fillcolor;
}
void OFButton::SetDisableStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor) {
	ButtonRim[disable] = buttonrim;
	ButtonRimColor[disable] = rimcolor;

	ButtonFillStyle[disable] = fillstyle;
	ButtonFillHatch[disable] = fillhatch;
	ButtonFillColor[disable] = fillcolor;
}
void OFButton::SetHoverStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor) {
	ButtonRim[hover] = buttonrim;
	ButtonRimColor[hover] = rimcolor;

	ButtonFillStyle[hover] = fillstyle;
	ButtonFillHatch[hover] = fillhatch;
	ButtonFillColor[hover] = fillcolor;
}
void OFButton::SetClickStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor) {
	ButtonRim[click] = buttonrim;
	ButtonRimColor[click] = rimcolor;

	ButtonFillStyle[click] = fillstyle;
	ButtonFillHatch[click] = fillhatch;
	ButtonFillColor[click] = fillcolor;
}
void OFButton::SetLoadingStyle(short buttonrim, COLORREF rimcolor, short fillstyle, short fillhatch, COLORREF fillcolor) {
	ButtonRim[loading] = buttonrim;
	ButtonRimColor[loading] = rimcolor;

	ButtonFillStyle[loading] = fillstyle;
	ButtonFillHatch[loading] = fillhatch;
	ButtonFillColor[loading] = fillcolor;
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
		if (KEY_DOWN(VK_LBUTTON)) { cout << "Button Click" << endl; return click; } else { return hover; }
	}
	return normal;
}