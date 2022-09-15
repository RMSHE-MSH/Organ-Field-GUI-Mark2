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
	win.Initialize_Window(930, 930, EW_SHOWCONSOLE);
	win.SetWindowTilte("Organ-Field GUI");
	win.MoveWindow(5, 5);

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

	//for (float i = -10; i < 10; i += 0.01) { OFEG.ImportNewFunction(0, 0, (1 + 1 * i) * cos(i), (1 + 1 * i) * sin(i)); }
	//OFEG.ImportNewFunction(0, (1 + 1 * i) * sin(j / sin(i + 1)), (1 + 1 * i) * cos(j / tan(i + 1)));
	//OFEG.ImportNewFunction(0, (sin(i) * i) * cos(i), (i)*sin(i));

	/*float k = 0.5;
	for (float j = 0; j < 200; j += 0.001) {
		OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
		for (float i = -PI; i < PI; i += 0.001) {
			OFEG.ImportNewFunction(0, sin(i), cos(i));
			OFEG.Style.FunctionPointCustomRadius.push_back(k += 0.005);
			OFEG.Style.FunctionPointCustomColor.push_back(HSVtoRGB(180 * (sin(i * 1.2) + 1), 0.5, 0.8));
		}
		OFEG.SSAARender(0, 4, 0);
		OFEG.AutoVisualDraw(0, 0);
	}
	OFEG.IMGtoVIDEO(0);*/

	//OFEG.Style.LineStyle.StaticLineColor = RGB(209, 52, 56);
	//OFEG.Style.LineStyle.FunctionLineWidth = 1;
	//OFEG.Style.PointStyle.StaticFunctionPointRadius = 2;
	//OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
	//for (float i = 1; i <= 10; i += 0.01) {
	//	OFEG.ImportNewFunction(0, i, i);
	//	OFEG.Style.PointStyle.DynamicFunctionPointColor.push_back(RGB(255, 107, 129)); OFEG.Style.LineStyle.DynamicFunctionLineColor.push_back(RGB(255, 107, 129));
	//	//OFEG.ImportNewFunction(0, i, sin(i));
	//	//OFEG.Style.PointStyle.DynamicFunctionPointColor.push_back(RGB(112, 161, 255)); OFEG.Style.LineStyle.DynamicFunctionLineColor.push_back(RGB(112, 161, 255));
	//	//OFEG.ImportNewFunction(0, i, pow(i, 2)); OFEG.Style.PointStyle.DynamicFunctionPointColor.push_back(RGB(123, 237, 159)); OFEG.Style.LineStyle.DynamicFunctionLineColor.push_back(RGB(123, 237, 159));
	//	//OFEG.ImportNewFunction(0, i, pow(2, i)); OFEG.Style.FunctionPointCustomColor.push_back(RGB(123, 237, 159));
	//	//HSVtoRGB(180 * (sin((i)*PI / 2) + 1), 0.5, 0.8);
	//}
	//OFEG.SSAARender(0, 4, 0);
	//OFEG.AutoVisualDraw(0, 0);

	//for (float j = 0; j < 30; j += 0.01) {
	//	OFEG.CreateFunctionGraphBOX(0, 10, 10, 963, 940);
	//	for (float i = -PI; i < PI; i += 0.001) {
	//		OFEG.ImportValue(0, (cos(j) + sin(i * j / (sin(i) + 2)) + 1) * sin(i + j), (sin(j) + cos(i * j / (cos(i) + 2)) + 1) * cos(i + j));
	//		OFEG.Style.PointStyle.DynamicFunctionPointRadius.push_back((0.09 * (sin(i * 10) + 2)));
	//		OFEG.Style.PointStyle.DynamicFunctionPointRadius.push_back(HSVtoRGB(180 * (sin((i) / (2 / (j + 1)) * PI) + 1), 0.5, 0.8));
	//	}
	//	//OFEG.SSAARender(0, 2, 60);
	//	OFEG.AutoVisualDraw(0, 1);
	//}
	//OFEG.IMGtoVIDEO(0);

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

	//OFEG.Style.ThemeStyle.DefaultTheme = lightTheme;
	OFEG.Style.PointStyle.StaticFunctionPointRadius = 2;
	OFEG.CreateFunctionGraphBOX(0, 10, 10, 900, 900);
	for (float i = -10; i < 4; i += 0.001) {
		OFEG.ImportValue(0, 3 * i, pow(0.5 * i, 3) + pow(i, 2));
	}
	OFEG.SSAARender(0, 4, 0);
	OFEG.AutoVisualDraw(0, 0);

	system("pause");
};