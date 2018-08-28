#include "TWindow.h"
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
{
	TWindow win;
	win.Init(hInst);

	return 0;
}