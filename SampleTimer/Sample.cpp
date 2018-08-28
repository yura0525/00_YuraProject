#include "TCore.h"

class Sample : public TCore
{
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("SampleTimer", 800, 600)

//int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
//{
//	Sample win;
//	win.SetWindow(hInst, L#"WindowClass", 800, 600);
//	win.Run();
//}