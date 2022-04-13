#include "Universal.h"
#include "Window.h"
#include "OFButton.h"
#include "ToggleSwitch.h"
#include "OFMessage.h"
#include "OFSlider.h"
#include "OFTools.h"
Window win;
OFButton OFB;
ToggleSwitch TS;
OFMessage OFM;
OFSlider OFS;
OFTaskManager OFTM;

int ClickEvent() { OFTM.ProcessTraversal(); return 0; }
int ClickEvent_CLS() { system("cls"); return 0; }

float i = 0;
int ToggleEvent0() { OFS.SetSliderLocation(0, abs((sin(i += 0.004)) * 100)); cout << "\r" << abs((sin(i)) * 100); return 0; }
int ToggleEvent1() { cout << "ToggleOpen-1" << endl; return 0; }
int ToggleEvent2() { cout << "ToggleOpen-2" << endl; return 0; }

int main() {
	win.Initialize_Window(400, 400, EW_SHOWCONSOLE);
	win.SetWindowTilte("Organ-Field GUI");

	OFB.ButtonDefaultStyle();
	TS.ToggleDefaultStyle();
	OFS.SliderDefaultStyle();

	OFM.PeekOFMessageUP();

	OFS.SetSliderLocation(0, 0);

	while (1) {
		OFB.CreateButton("Test", 80, 10, 120, 60, normal, ClickEvent);
		OFB.CreateButton("Disable", 80, 80, 120, 60, disable, NULL);
		OFB.CreateButton("Loading", 80, 150, 120, 60, loading, NULL);
		OFB.CreateButton("Clear", 80, 220, 120, 60, normal, ClickEvent_CLS);

		TS.CreateToggle(0, 10, 10, close, ToggleEvent0);
		TS.CreateToggle(1, 10, 40, close, ToggleEvent1);
		TS.CreateToggle(2, 10, 70, disable, ToggleEvent2);
		TS.CreateToggle(3, 10, 100, loading, ToggleEvent2);

		OFS.CreateSlider(0, 10, 360, 380, 30, normal);
	}
	system("pause");
};