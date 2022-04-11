#include "OFSlider.h"
OFMessage OFM_OFS;

void OFSlider::SliderRender(int SliderName, int *DiagonalPoints, short state) {
	if (state == normal) {//正常激活状态的滑动条;
		clearrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, SliderRim[normal]); setlinecolor(SliderRimColor[normal]);
		setfillstyle(BS_SOLID); setfillcolor(SliderBackColor[normal]);
		fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		setfillstyle(BS_SOLID); setfillcolor(SliderFillColor[normal]);
		solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[0] + int((float(DiagonalPoints[2] - DiagonalPoints[0])) / float(100) * Slider.LocationValue[SliderName]), DiagonalPoints[3]);
	} else if (state == disable) {//禁用的滑动条;
		clearrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, SliderRim[disable]); setlinecolor(SliderRimColor[disable]);
		setfillstyle(BS_SOLID); setfillcolor(SliderBackColor[disable]);
		fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		setfillstyle(BS_SOLID); setfillcolor(SliderFillColor[disable]);
		solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[0] + int((float(DiagonalPoints[2] - DiagonalPoints[0])) / float(100) * Slider.LocationValue[SliderName]), DiagonalPoints[3]);
	} else if (state == hover) {//鼠标划过的滑动条;
		clearrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		setfillstyle(BS_SOLID); setfillcolor(SliderBackColor[hover]);
		solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		setfillstyle(BS_SOLID); setfillcolor(SliderFillColor[hover]);
		solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[0] + int((float(DiagonalPoints[2] - DiagonalPoints[0])) / float(100) * Slider.LocationValue[SliderName]), DiagonalPoints[3]);

		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, SliderRim[hover]); setlinecolor(SliderRimColor[hover]);
		rectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
	} else { cout << "RMSHE: Error! non-existent Slider state." << endl; }//错误:不存在的按钮状态;
}

short OFSlider::SliderDetec(int SliderName, int *DiagonalPoints, ExMessage Mouse_Window) {
	if (Slider.SliderState[SliderName] != disable) {
		if (Mouse_Window.x > DiagonalPoints[0] && Mouse_Window.y > DiagonalPoints[1] && Mouse_Window.x < DiagonalPoints[2] && Mouse_Window.y < DiagonalPoints[3]) {
			if (KEY_DOWN(VK_LBUTTON)) {
				while (1) {
					ExMessage Mouse_Window = OFM_OFS.PeekOFMessage();
					BeginBatchDraw(); SliderDrag(SliderName, DiagonalPoints, Mouse_Window); EndBatchDraw();
					SetSliderLocation(SliderName, ((float)Mouse_Window.x - (float)DiagonalPoints[0]) * (100 / ((float)DiagonalPoints[2] - (float)DiagonalPoints[0])));
					if (!KEY_DOWN(VK_LBUTTON)) { break; }
				}
				Slider.SliderState[SliderName] = normal; return normal;
			} else { Slider.SliderState[SliderName] = hover; return hover; }
		}
		Slider.SliderState[SliderName] = normal; return normal;
	} else { return disable; }
}

void OFSlider::SliderDrag(int SliderName, int *DiagonalPoints, ExMessage Mouse_Window) {
	if (Mouse_Window.x > DiagonalPoints[0] && Mouse_Window.x < DiagonalPoints[2]) {
		clearrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		setfillstyle(BS_SOLID); setfillcolor(SliderBackColor[hover]);
		solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		setfillstyle(BS_SOLID); setfillcolor(SliderFillColor[Drag]);
		solidrectangle(DiagonalPoints[0], DiagonalPoints[1], Mouse_Window.x, DiagonalPoints[3]);

		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, SliderRim[Drag]); setlinecolor(SliderRimColor[Drag]);
		rectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);

		DrawSliderText(SliderName, to_string(int(Slider.LocationValue[SliderName])).c_str(), DiagonalPoints);
	}
}

void OFSlider::SetSliderLocation(int SliderName, float LocationValue) {
	if (1 <= LocationValue && LocationValue <= 100) {
		Slider.LocationValue[SliderName] = LocationValue;
	} else if (1 > LocationValue) {
		Slider.LocationValue[SliderName] = 0;
	} else if (100 < LocationValue) {
		Slider.LocationValue[SliderName] = 100;
	}
}

float OFSlider::CreateSlider(int SliderName, int x, int y, int width, int height, short SetState) {
	if (Slider.SliderNum <= SliderName) { if (Slider.SliderNum == SliderName) { Slider.SliderState[Slider.SliderNum] = SetState; } ++Slider.SliderNum; }

	int rec[] = { x, y ,x + width ,y + height };//转换为矩形;

	SliderDetec(SliderName, rec, OFM_OFS.PeekOFMessage());//检测按钮状态;

	BeginBatchDraw();
	SliderRender(SliderName, rec, Slider.SliderState[SliderName]);
	DrawSliderText(SliderName, to_string(int(Slider.LocationValue[SliderName])).c_str(), rec);
	EndBatchDraw();

	return Slider.LocationValue[SliderName];
}

float OFSlider::PeekSliderLocation(int SliderName) { return Slider.LocationValue[SliderName]; }

void OFSlider::DrawSliderText(int SliderName, LPCTSTR str, int *DiagonalPoints) {
	RECT r; int height = DiagonalPoints[3] - DiagonalPoints[1];
	if (DiagonalPoints[0] + int((float(DiagonalPoints[2] - DiagonalPoints[0])) / float(100) * Slider.LocationValue[SliderName]) - height > DiagonalPoints[0]) {
		SetSliderTextStyle(LONG(height / 1), FW_BLACK, SliderTextColor[normal], "微软雅黑");//设置按钮字体样式;
		r = { DiagonalPoints[0] + int((float(DiagonalPoints[2] - DiagonalPoints[0])) / float(100) * Slider.LocationValue[SliderName]) - height, DiagonalPoints[1], DiagonalPoints[0] + int((float(DiagonalPoints[2] - DiagonalPoints[0])) / float(100) * Slider.LocationValue[SliderName]), DiagonalPoints[3] };
	} else {
		SetSliderTextStyle(LONG(height / 1), FW_BLACK, SliderTextColor[Drag], "微软雅黑");//设置按钮字体样式;
		r = { DiagonalPoints[0] + int((float(DiagonalPoints[2] - DiagonalPoints[0])) / float(100) * Slider.LocationValue[SliderName]), DiagonalPoints[1], DiagonalPoints[0] + int((float(DiagonalPoints[2] - DiagonalPoints[0])) / float(100) * Slider.LocationValue[SliderName]) + height, DiagonalPoints[3] };
	}

	if (Slider.LocationValue[SliderName] != 0 && Slider.LocationValue[SliderName] != 100)drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

void OFSlider::SetSliderTextStyle(LONG height, LONG weight, COLORREF textcolor, LPCTSTR font) {
	setbkmode(TRANSPARENT);
	LOGFONT f; gettextstyle(&f);
	f.lfHeight = height;
	f.lfQuality = ANTIALIASED_QUALITY | PROOF_QUALITY, f.lfCharSet = CHINESEBIG5_CHARSET;
	_tcscpy_s(f.lfFaceName, font);
	settextstyle(&f); settextcolor(textcolor);
}

void OFSlider::SetNormalStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor) {
	SliderRim[normal] = 1;
	SliderRimColor[normal] = rimcolor;
	SliderFillColor[normal] = fillcolor;
	SliderBackColor[normal] = bkcolor;
	SliderTextColor[normal] = TextColor;
}

void OFSlider::SetDisableStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor) {
	SliderRim[disable] = 1;
	SliderRimColor[disable] = rimcolor;
	SliderFillColor[disable] = fillcolor;
	SliderBackColor[disable] = bkcolor;
	SliderTextColor[disable] = TextColor;
}

void OFSlider::SetHoverStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor) {
	SliderRim[hover] = 1;
	SliderRimColor[hover] = rimcolor;
	SliderFillColor[hover] = fillcolor;
	SliderBackColor[hover] = bkcolor;
	SliderTextColor[hover] = TextColor;
}

void OFSlider::SetDragStyle(COLORREF rimcolor, COLORREF fillcolor, COLORREF bkcolor, COLORREF TextColor) {
	SliderRim[Drag] = 1;
	SliderRimColor[Drag] = rimcolor;
	SliderFillColor[Drag] = fillcolor;
	SliderBackColor[Drag] = bkcolor;
	SliderTextColor[Drag] = TextColor;
}

void OFSlider::SliderDefaultStyle() {
	SetNormalStyle(RGB(225, 223, 221), RGB(85, 177, 85), RGB(225, 223, 221), RGB(255, 255, 255));
	SetDisableStyle(RGB(225, 223, 221), RGB(85, 177, 85), RGB(225, 223, 221), NULL);
	SetHoverStyle(RGB(10, 89, 247), RGB(85, 177, 85), RGB(229, 229, 229), NULL);
	SetDragStyle(RGB(10, 89, 247), RGB(10, 89, 247), RGB(229, 229, 229), RGB(30, 30, 30));
}