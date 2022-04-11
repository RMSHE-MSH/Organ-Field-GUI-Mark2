#include "ToggleSwitch.h"
#include "OFMessage.h"
Window win_m;
OFMessage OFM_TS;

void ToggleSwitch::ToggleRender(int ToggleName, int *DiagonalPoints, short state) {
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

		if (ToggleState[ToggleName] == close) { setfillcolor(SliderColor[hover]); solidrectangle(DiagonalPoints[0] + 2, DiagonalPoints[1] + 2, DiagonalPoints[2] - 30, DiagonalPoints[3] - 2); }
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
			//������ؿ�����ת���������ÿ���״̬;
			if (ToggleState[ToggleName] == close) {
				ToggleState[ToggleName] = open; return open;
			} else if (ToggleState[ToggleName] != disable && ToggleState[ToggleName] != loading) {
				ToggleState[ToggleName] = close; return close;
			}
		} else { return hover; }
	}

	if (ToggleState[ToggleName] == close) { return close; } else if (ToggleState[ToggleName] == open) { return open; } else { return disable; }
}

int ToggleSwitch::CreateToggle(int ToggleName, int x, int y, short SetInitialState, int (*EventFunc)()) {
	if (ToggleNum <= ToggleName) { if (ToggleNum == ToggleName) { ToggleState[ToggleNum] = SetInitialState; } ++ToggleNum; }

	int rec[] = { x, y ,x + 44 ,y + 20 };//ת��Ϊ����;

	short Event = ToggleDetec(ToggleName, rec, OFM_TS.PeekOFMessage());//��ⰴť״̬;

	if (ToggleState[ToggleName] != disable && ToggleState[ToggleName] != loading) {
		BeginBatchDraw();
		ToggleRender(ToggleName, rec, Event);
		EndBatchDraw();

		if (ToggleState[ToggleName] == open) { return EventFunc(); }
	} else {
		BeginBatchDraw();
		ToggleRender(ToggleName, rec, ToggleState[ToggleName]);
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
void ToggleSwitch::SetHoverStyle(short Togglerim, COLORREF MainColor) {
	ToggleRim[hover] = Togglerim;
	ToggleRimColor[hover] = MainColor;
	ToggleFillStyle[hover] = NULL;

	ToggleFillColor[hover] = NULL;
	SliderColor[hover] = MainColor;
}
void ToggleSwitch::SetLoadingStyle(short Togglerim, COLORREF rimcolor, COLORREF slidercolor) {
	ToggleRim[loading] = Togglerim;
	ToggleRimColor[loading] = rimcolor;
	ToggleFillStyle[loading] = BS_SOLID;

	ToggleFillColor[loading] = getbkcolor();;
	SliderColor[loading] = slidercolor;
}

void ToggleSwitch::ToggleDefaultStyle() {
	SetCloseStyle(1, RGB(51, 51, 51), RGB(51, 51, 51));
	SetOpenStyle(RGB(10, 89, 247), RGB(241, 243, 245));
	SetDisableStyle(1, RGB(153, 153, 153), RGB(153, 153, 153));
	SetHoverStyle(1, RGB(0, 0, 0));
	SetLoadingStyle(1, RGB(10, 89, 247), RGB(10, 89, 247));
}

short ToggleSwitch::GetToggleState(int ToggleName) { return ToggleState[ToggleName]; }

void ToggleSwitch::SetToggleState(int ToggleName, short SetState) { ToggleState[ToggleName] = SetState; }