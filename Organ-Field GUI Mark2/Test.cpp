#include "Window.h"
#include "OFButton.h"
Window win;
OFButton OFB;

ExMessage m;
int rec[4] = { 10, 10, 120, 60 };
int rec1[4] = { 10, 70, 120, 120 };

void Initialize_settings() {
	OFB.SetNormalStyle(1, RGB(180, 180, 180), BS_SOLID, NULL, RGB(225, 225, 225));
	OFB.SetHoverStyle(1, RGB(180, 180, 180), BS_SOLID, NULL, RGB(229, 243, 255));
	OFB.SetClickStyle(1, RGB(10, 89, 247), BS_SOLID, NULL, RGB(204, 232, 255));

	OFB.SetButtonTextStyle(30, FW_MEDIUM, RGB(30, 30, 30), "Î¢ÈíÑÅºÚ");

	OFB.ButtonRender(rec, OFB.ButtonDetec(rec, m), NULL);
	OFB.DrawButtonText("TEST", RGB(0, 0, 0), rec);

	OFB.ButtonRender(rec1, OFB.ButtonDetec(rec1, m), NULL);
	OFB.DrawButtonText("TEST1", RGB(0, 0, 0), rec1);
}

int main() {
	win.Initialize_Window(1200, 700, EW_SHOWCONSOLE);
	win.SetWindowTilte("Organ-Field GUI");

	Initialize_settings();

	while (1) {
		m = win.GetWindowMouse();
		OFB.ButtonRender(rec, OFB.ButtonDetec(rec, m), NULL);
		OFB.DrawButtonText("TEST", RGB(0, 0, 0), rec);

		OFB.ButtonRender(rec1, OFB.ButtonDetec(rec1, m), NULL);
		OFB.DrawButtonText("TEST1", RGB(0, 0, 0), rec1);
	}
	system("pause");
};