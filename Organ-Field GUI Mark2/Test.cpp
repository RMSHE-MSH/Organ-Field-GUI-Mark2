#include "Window.h"
#include "OFButton.h"
Window win;
OFButton OFB;

void ClickEvent() { cout << "ButtonClick-1" << endl; }
void ClickEvent2() { cout << "ButtonClick-2" << endl; }
void ClickEvent3() { cout << "ButtonClick-3" << endl; }
void ClickEvent_CLS() { system("cls"); }

int main() {
	win.Initialize_Window(1200, 700, EW_SHOWCONSOLE);
	win.SetWindowTilte("Organ-Field GUI");

	OFB.ButtonDefaultStyle();
	while (1) {
		OFB.CreateButton("Test", 10, 10, 120, 60, normal, ClickEvent);
		OFB.CreateButton("Test2", 10, 80, 120, 60, normal, ClickEvent2);
		OFB.CreateButton("Test3", 10, 150, 120, 60, normal, ClickEvent3);
		OFB.CreateButton("Disable", 10, 220, 120, 60, disable, NULL);
		OFB.CreateButton("Loading", 10, 290, 120, 60, loading, NULL);
		OFB.CreateButton("Clear", 10, 360, 120, 60, normal, ClickEvent_CLS);
	}
	system("pause");
};