#include "Universal.h"
#include "Window.h"
#include "OFButton.h"
#include "ToggleSwitch.h"
#include "OFMessage.h"
#include "OFSlider.h"
#include "OFTools.h"
#include "OFBlur.h"
#include "OFEXPGraph.h"
Window win;
OFButton OFB;
ToggleSwitch TS;
OFMessage OFM;
OFSlider OFS;
OFTaskManager OFTM;
OFBlur OFBr;
OFEXPGraph OFEG;

int ClickEvent() { OFTM.ProcessTraversal(); return 0; }
int ClickEvent_CLS() { system("cls"); OFBr.ShowOrig(0, 1); return 0; }

int Blur() {
	OFTM.ProcessTraversal();
	OFTM.ProcessKill("Taskmgr.exe");
	OFBr.ShowBlur(0, 1);

	cout << OFTM.TMT.ProcessKill_Result << "\n" << OFTM.TMT.Processlist << endl;
	return 0;
}

float i = 0;
int ToggleEvent0() { OFS.SetSliderLocation(0, abs((sin(i += 0.004)) * 100)); cout << "\r" << abs((sin(i)) * 100); return 0; }
int ToggleEvent1() { cout << "ToggleOpen-1" << endl; return 0; }
int ToggleEvent2() { cout << "ToggleOpen-2" << endl; return 0; }

int main() {
	win.Initialize_Window(1000, 1000, EW_SHOWCONSOLE);
	win.SetWindowTilte("Organ-Field GUI");
	win.MoveWindow(10, 10);

	/*loadimage(NULL, _T("D:\\Organ-Field GUI2.png"), 800, 800);

	OFBr.WriteBlurCache(0, 0, 0, 800, 800);
	OFBr.ColorFilter(0, 230, 230, 230);
	OFBr.CreateGaussBlur(0, 10, 1);
	OFBr.ShowBlur(0, 1);*/

	/*int k = 0;
	for (int i = 0; i < 800; i += 40) {
		for (int j = 0; j < 800; j += 40) {
			OFBr.WriteBlurCache(k, j, i, 40, 40);
			OFBr.CreateGaussBlur(k, 200, 10);

			OFBr.ShowBlur(k, 1);
			++k;
		}
	}

	Sleep(350);

	OFBr.WriteBlurCache(++k, 0, 0, 800, 800);
	OFBr.ColorFilter(k, -200, -200, -200);
	OFBr.CreateGaussBlur(k, 20, 10);
	OFBr.ShowBlur(k, 1);*/

	/*OFB.ButtonDefaultStyle();
	TS.ToggleDefaultStyle();
	OFS.SliderDefaultStyle();

	OFM.PeekOFMessageUP();

	OFS.SetSliderLocation(0, 0);*/

	/*for (float j = 0; j < 100000; j += 0.01) {
		BeginBatchDraw();

		cleardevice();
		OFEG.CreateFunctionGraphBOX(0, 310 + 300 * sin(j), 310 + 300 * cos(j), 100 + abs(400 * sin(j)), 100 + abs(400 * cos(j)));
		for (float i = -2 * sin(j) - 1; i < 3; i += 0.01) { OFEG.ImportNewFunction(0, 0, 4 * sin(i), pow(i, 3) / 2 * sin(j)); }
		OFEG.AutoVisualDraw(0, 0);

		EndBatchDraw();
	}*/

	int A = 6000;

	Sleep(A);
	OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
	for (float i = -3.14; i < 3.14; i += 0.00004) { OFEG.ImportNewFunction(0, 0, 2 * sin(i), pow(i, 2)); }
	OFEG.AutoVisualDraw(0, 0);

	Sleep(A); cleardevice();
	OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
	for (float i = -2; i < 2; i += 0.00004) { OFEG.ImportNewFunction(0, 0, 3 * sin(i), pow(i, 3)); }
	OFEG.AutoVisualDraw(0, 0);

	Sleep(A); cleardevice();
	OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
	for (float i = -10; i < 10; i += 0.00004) { OFEG.ImportNewFunction(0, 0, (1 + 1 * i) * cos(i), (1 + 1 * i) * sin(i)); }
	OFEG.AutoVisualDraw(0, 0);

	Sleep(A); cleardevice();
	OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
	for (float i = -10; i < 10; i += 0.00004) { OFEG.ImportNewFunction(0, 0, pow(tan(sin(i)), 3), pow(tan(cos(i)), 3)); }
	OFEG.AutoVisualDraw(0, 0);

	Sleep(A); cleardevice();
	OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
	for (float i = -10; i < 10; i += 0.00004) { OFEG.ImportNewFunction(0, 0, tan(sin(i)), tan(cos(i))); }
	OFEG.AutoVisualDraw(0, 0);

	Sleep(A); cleardevice();
	OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
	for (float i = -10; i < 10; i += 0.00004) { OFEG.ImportNewFunction(0, 0, 2 * (2 * cos(i) - cos(2 * i)), 2 * (2 * sin(i) - sin(2 * i))); }
	OFEG.AutoVisualDraw(0, 0);

	Sleep(A); cleardevice();
	OFEG.CreateFunctionGraphBOX(0, 10, 10, 300, 300);
	for (float i = -3.14; i < 3.14; i += 0.008) { OFEG.ImportNewFunction(0, 0, 2 * sin(i), pow(i, 2)); }
	OFEG.AutoVisualDraw(0, 0);

	OFEG.CreateFunctionGraphBOX(1, 320, 10, 300, 300);
	for (float i = -2; i < 2; i += 0.005) { OFEG.ImportNewFunction(1, 0, 3 * sin(i), pow(i, 3)); }
	OFEG.AutoVisualDraw(1, 0);

	OFEG.CreateFunctionGraphBOX(2, 630, 10, 300, 300);
	for (float i = -10; i < 10; i += 0.005) { OFEG.ImportNewFunction(2, 0, 0.5 * (1 + 1 * i) * cos(i), 0.5 * (1 + 1 * i) * sin(i)); }
	OFEG.AutoVisualDraw(2, 0);

	OFEG.CreateFunctionGraphBOX(3, 10, 320, 300, 300);
	for (float i = -10; i < 10; i += 0.008) { OFEG.ImportNewFunction(3, 0, pow(tan(sin(i)), 3), pow(tan(cos(i)), 3)); }
	OFEG.AutoVisualDraw(3, 0);

	OFEG.CreateFunctionGraphBOX(4, 320, 320, 300, 300);
	for (float i = -10; i < 10; i += 0.008) { OFEG.ImportNewFunction(4, 0, tan(sin(i)), tan(cos(i))); }
	OFEG.AutoVisualDraw(4, 0);

	OFEG.CreateFunctionGraphBOX(0, 630, 320, 300, 300);
	for (float i = -10; i < 10; i += 0.008) { OFEG.ImportNewFunction(0, 0, 2 * (2 * cos(i) - cos(2 * i)), 2 * (2 * sin(i) - sin(2 * i))); }
	OFEG.AutoVisualDraw(0, 0);

	/*while (1) {
		OFB.CreateButton("Blur", 80, 10, 120, 60, normal, Blur);
		OFB.CreateButton("Disable", 80, 80, 120, 60, disable, NULL);
		OFB.CreateButton("Loading", 80, 150, 120, 60, loading, NULL);
		OFB.CreateButton("Clear", 80, 220, 120, 60, normal, ClickEvent_CLS);

		TS.CreateToggle(0, 10, 10, close, ToggleEvent0);
		TS.CreateToggle(1, 10, 40, close, ToggleEvent1);
		TS.CreateToggle(2, 10, 70, disable, Blur);
		TS.CreateToggle(3, 10, 100, close, ToggleEvent2);

		OFS.CreateSlider(0, 10, 360, 380, 30, normal);
	}*/
	system("pause");
};