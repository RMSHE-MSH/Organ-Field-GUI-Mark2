#include "Universal.h"
#include "Window.h"
#include "OFButton.h"
#include "ToggleSwitch.h"
#include "OFMessage.h"
Window win;
OFButton OFB;
ToggleSwitch TS;
OFMessage OFM;

void ClickEvent() { cout << "ButtonClick-NULL" << endl; }
int ClickEvent2() { cout << "ButtonClick-Return" << endl; return 1; }
void ClickEvent_CLS() { system("cls"); }

void ToggleEvent0() { cout << "ToggleOpen-0" << endl; }
void ToggleEvent1() { cout << "ToggleOpen-1" << endl; }
void ToggleEvent2() { cout << "ToggleOpen-2" << endl; }

int main() {
	win.Initialize_Window(400, 380, EW_SHOWCONSOLE);
	win.SetWindowTilte("Organ-Field GUI");

	OFB.ButtonDefaultStyle();
	TS.ToggleDefaultStyle();

	OFM.PeekWindowMouse();
	while (1) {
		OFB.CreateButton("Test", 80, 10, 120, 60, normal, ClickEvent);
		OFB.CreateRintButton("Test2", 80, 80, 120, 60, normal, ClickEvent2);
		OFB.CreateButton("Disable", 80, 150, 120, 60, disable, NULL);
		OFB.CreateButton("Loading", 80, 220, 120, 60, loading, NULL);
		OFB.CreateButton("Clear", 80, 290, 120, 60, normal, ClickEvent_CLS);

		TS.CreateToggle(0, 10, 10, close, ToggleEvent0);
		TS.CreateToggle(1, 10, 40, close, ToggleEvent1);
		TS.CreateToggle(3, 10, 70, close, ToggleEvent2);
		TS.CreateToggle(3, 10, 100, close, ToggleEvent2);
	}
	system("pause");
};