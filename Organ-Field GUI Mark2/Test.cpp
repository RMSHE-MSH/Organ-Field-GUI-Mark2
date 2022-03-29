#include "Window.h"
#include "OFButton.h"
Window win;
OFButton OFB;

void ClickEvent() {
	cout << "ButtonClick" << endl;
}
void ClickEvent2() {
	cout << "ButtonClick2" << endl;
}
void ClickEvent3() {
	cout << "ButtonClick3" << endl;
}

int main() {
	win.Initialize_Window(1200, 700, EW_SHOWCONSOLE);
	win.SetWindowTilte("Organ-Field GUI");

	OFB.ButtonDefaultStyle();
	while (1) {
		OFB.CreateButton("Test", 10, 10, 60, 40, normal, ClickEvent);
		OFB.CreateButton("Test2", 80, 10, 60, 40, normal, ClickEvent2);
		OFB.CreateButton("Test3", 150, 10, 60, 40, normal, ClickEvent3);
	}
	system("pause");
};