#include "ToggleSwitch.h"
#include "OFMessage.h"
Window win_m;
OFMessage OFM_TS;

void ToggleSwitch::ToggleRender(int *DiagonalPoints, short state) {
	if (state == close) {//�ر�״̬�Ŀ���;
		setfillstyle(ToggleFillStyle[close], NULL); setfillcolor(ToggleFillColor[close]);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ToggleRim[close]); setlinecolor(ToggleRimColor[close]);
		fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		setfillcolor(SliderColor[close]); solidrectangle(DiagonalPoints[0] + 2, DiagonalPoints[1] + 2, DiagonalPoints[2] - 30, DiagonalPoints[3] - 2);
	} else if (state == open) {//��״̬�Ŀ���;
		setfillstyle(ToggleFillStyle[open], NULL); setfillcolor(ToggleFillColor[open]);
		solidrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		setfillcolor(SliderColor[open]); solidrectangle(DiagonalPoints[0] + 30, DiagonalPoints[1] + 2, DiagonalPoints[2] - 2, DiagonalPoints[3] - 2);
	} else if (state == disable) {//���õĿ���;
		setfillstyle(ToggleFillStyle[disable], NULL); setfillcolor(ToggleFillColor[disable]);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ToggleRim[disable]); setlinecolor(ToggleRimColor[disable]);
		fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		setfillcolor(SliderColor[disable]); solidrectangle(DiagonalPoints[0] + 2, DiagonalPoints[1] + 2, DiagonalPoints[2] - 30, DiagonalPoints[3] - 2);
	} else if (state == hover) {//��껮���Ŀ���;
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ToggleRim[hover]); setlinecolor(ToggleRimColor[hover]);
		rectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
	} else if (state == loading) {//����״̬�Ŀ���;
		setfillstyle(ToggleFillStyle[loading], NULL); setfillcolor(ToggleFillColor[loading]);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, ToggleRim[loading]); setlinecolor(ToggleRimColor[loading]);
		fillrectangle(DiagonalPoints[0], DiagonalPoints[1], DiagonalPoints[2], DiagonalPoints[3]);
		setfillcolor(SliderColor[loading]); solidrectangle(DiagonalPoints[0] + 30, DiagonalPoints[1] + 2, DiagonalPoints[2] - 2, DiagonalPoints[3] - 2);
	} else { cout << "RMSHE: Error! non-existent Toggle state." << endl; }//����:�����ڵİ�ť״̬;
}

short ToggleSwitch::ToggleDetec(int ToggleName, int *DiagonalPoints, ExMessage Mouse_Window) {
	//�����뿪������;
	if (Mouse_Window.x > DiagonalPoints[0] && Mouse_Window.y > DiagonalPoints[1] && Mouse_Window.x < DiagonalPoints[2] && Mouse_Window.y < DiagonalPoints[3]) {
		if (KEY_DOWN(VK_LBUTTON)) {//����������;
			while (1) { if (!KEY_DOWN(VK_LBUTTON))break; }//�������ɿ�;
			//��ת����״̬,����������״̬;
			if (SliderState[ToggleName] == 0) { SliderState[ToggleName] = 1; return open; } else { SliderState[ToggleName] = 0; return close; }
		} else { return hover; }
	}

	if (SliderState[ToggleName] == 0) { return close; } else { return open; }
}

void ToggleSwitch::CreateToggle(int ToggleName, int x, int y, short SetState, void ClickEvent()) {
	int rec[] = { x, y ,x + 44 ,y + 20 };//ת��Ϊ����;

	short Event = ToggleDetec(ToggleName, rec, OFM_TS.GetMouseMessage());//��ⰴť״̬;

	if (SetState != disable && SetState != loading) {
		BeginBatchDraw();
		ToggleRender(rec, Event);
		EndBatchDraw();

		if (Event == open) { ClickEvent(); }
	} else {
		BeginBatchDraw();
		ToggleRender(rec, SetState);
		EndBatchDraw();
	}
}

void ToggleSwitch::SetCloseStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor) {
	ToggleRim[close] = Togglerim;
	ToggleRimColor[close] = rimcolor;
	ToggleFillStyle[close] = BS_SOLID;

	ToggleFillColor[close] = getbkcolor();
	SliderColor[close] = slidercolor;
}
void ToggleSwitch::SetOpenStyle(COLORREF fillcolor, COLORREF slidercolor) {
	ToggleRim[open] = NULL;
	ToggleRimColor[open] = NULL;
	ToggleFillStyle[open] = BS_SOLID;

	ToggleFillColor[open] = fillcolor;
	SliderColor[open] = slidercolor;
}
void ToggleSwitch::SetDisableStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor) {
	ToggleRim[disable] = Togglerim;
	ToggleRimColor[disable] = rimcolor;
	ToggleFillStyle[disable] = BS_SOLID;

	ToggleFillColor[disable] = getbkcolor();
	SliderColor[disable] = slidercolor;
}
void ToggleSwitch::SetHoverStyle(short Togglerim, COLORREF rimcolor) {
	ToggleRim[hover] = Togglerim;
	ToggleRimColor[hover] = rimcolor;
	ToggleFillStyle[hover] = NULL;

	ToggleFillColor[hover] = NULL;
	SliderColor[hover] = NULL;
}
void ToggleSwitch::SetLoadingStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor) {
	ToggleRim[loading] = Togglerim;
	ToggleRimColor[loading] = rimcolor;
	ToggleFillStyle[loading] = BS_SOLID;

	ToggleFillColor[loading] = getbkcolor();;
	SliderColor[loading] = slidercolor;
}

void ToggleSwitch::ToggleDefaultStyle() {
	SetCloseStyle(1, RGB(0, 0, 0), RGB(0, 0, 0));
	SetOpenStyle(RGB(10, 89, 247), RGB(241, 243, 245));
	SetDisableStyle(1, RGB(153, 153, 153), RGB(153, 153, 153));
	SetHoverStyle(1, RGB(113, 96, 232));
	SetLoadingStyle(1, RGB(10, 89, 247), RGB(10, 89, 247));
}