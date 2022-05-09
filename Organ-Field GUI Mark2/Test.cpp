#include "Universal.h"
#include "Window.h"
#include "OFButton.h"
#include "ToggleSwitch.h"
#include "OFMessage.h"
#include "OFSlider.h"
#include "OFTools.h"
#include "OFBlur.h"
Window win;
OFButton OFB;
ToggleSwitch TS;
OFMessage OFM;
OFSlider OFS;
OFTaskManager OFTM;
OFBlur OFBr;

int ClickEvent() { OFTM.ProcessTraversal(); return 0; }
int ClickEvent_CLS() { system("cls"); OFBr.ShowOrig(0, 1); return 0; }

int Blur() {
	for (float i = 0; i < PI / 2; i += 0.005)OFBr.ShowBlur(0, sin(i));
	return 0;
}

float i = 0;
int ToggleEvent0() { OFS.SetSliderLocation(0, abs((sin(i += 0.004)) * 100)); cout << "\r" << abs((sin(i)) * 100); return 0; }
int ToggleEvent1() { cout << "ToggleOpen-1" << endl; return 0; }
int ToggleEvent2() { cout << "ToggleOpen-2" << endl; return 0; }

int main() {
	win.Initialize_Window(800, 800, EW_SHOWCONSOLE);
	win.SetWindowTilte("Organ-Field GUI");
	win.MoveWindow(10, 10);

	loadimage(NULL, _T("D:\\Organ-Field GUI.png"), 800, 800);

	OFBr.WriteBlurCache(0, 400, 0, 60, 60);
	OFBr.ColorFilter(0, -130, -130, -130);
	OFBr.CreateGaussBlur(0, 20, 20);
	//OFBr.CreateAverageBlur(0, 40);
	OFBr.ShowBlur(0, 1);

	/*int k = 0;
	for (int i = 0; i < 400; i += 20) {
		for (int j = 0; j < 400; j += 20) {
			OFBr.WriteBlurCache(k, j, i, 20, 20);
			OFBr.CreateGaussBlur(k, 20, 100000000);

			OFBr.ShowBlur(k, 0, 0, 1);
			++k;
		}
	}

	Sleep(350);

	OFBr.WriteBlurCache(++k, 0, 0, 400, 400);
	OFBr.ColorFilter(k, -200, -200, -200);
	OFBr.CreateGaussBlur(k, 20, 10);
	OFBr.ShowBlur(k, 0, 0, 1);*/

	OFB.ButtonDefaultStyle();
	TS.ToggleDefaultStyle();
	OFS.SliderDefaultStyle();

	OFM.PeekOFMessageUP();

	OFS.SetSliderLocation(0, 0);

	while (1) {
		OFB.CreateButton("Blur", 80, 10, 120, 60, normal, Blur);
		OFB.CreateButton("Disable", 80, 80, 120, 60, disable, NULL);
		OFB.CreateButton("Loading", 80, 150, 120, 60, loading, NULL);
		OFB.CreateButton("Clear", 80, 220, 120, 60, normal, ClickEvent_CLS);

		TS.CreateToggle(0, 10, 10, close, ToggleEvent0);
		TS.CreateToggle(1, 10, 40, close, ToggleEvent1);
		TS.CreateToggle(2, 10, 70, disable, Blur);
		TS.CreateToggle(3, 10, 100, close, ToggleEvent2);

		OFS.CreateSlider(0, 10, 360, 380, 30, normal);
	}
	system("pause");
};