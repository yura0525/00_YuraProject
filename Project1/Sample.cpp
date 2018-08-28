#include <windows.h>



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			MessageBox(NULL, L"윈도우가 떴다.", L"윈도우 생성", MB_OK);
		}
		break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);		// WM_QUIT 메세지가 메시지큐에 푸쉬된다.
			//DestroyWindow(hWnd);	// WM_DESTROY 메세지가 메세지큐에 푸쉬된다.(다른버튼으로 종료할때)
		}
		return 0;
		//default:
		//	return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);return 0;
}

void GameInit()
{
	//지형, 오브젝트, 이펙트, UI 초기화, 로딩
	MessageBox(NULL, L"게임 초기화", L"게임 실행", MB_OK);
}

void GameRun()
{
	//지형, 오브젝트, 이펙트, UI
	//MessageBox(NULL, L"게임 진행중", L"게임 실행", MB_OK);
}

void GameRelease()
{
	//지형, 오브젝트, 이펙트, UI 소멸
	MessageBox(NULL, L"게임 종료", L"게임 실행", MB_OK);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
{
	//윈도우 클래스 등록
	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= hInst;
	wc.lpszClassName	= L"MyWindow";
	wc.hIcon			= LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.hIconSm			= LoadIcon(NULL, IDI_HAND);
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;

	if (RegisterClassEx(&wc) == FALSE)
	{
		return 1;
	}

	//윈도우 생성(등록된 클래스를 사용해서)
	HWND hWnd = CreateWindowEx( WS_EX_APPWINDOW, 
		L"MyWindow", 
		L"KGCA SAMPLES", 
		WS_OVERLAPPEDWINDOW, 
		0, 0, 	800, 600, NULL, NULL, hInst, NULL);

	if (hWnd == NULL)
	{
		return 1;
	}

	ShowWindow(hWnd, SW_SHOW);

	GameInit();

	//GetMessage(&msg, NULL, 0, 0)
	//메세지큐에서 메세지가 1개이상 있을 경우 1개를 가져옴.
	//메세지큐에 메세지가 없을경우엔 반환을 안하고 대기함. 블록형 함수

	//PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)
	//메세지큐에서 메세지 있으면 메세지를 반환, 없으면 false를 반환.
	//메세지 큐에 메세지가 있으나 없으나 무조건 반환.
	//일반적으로 PeekMessage를 쓴다.
	MSG msg;		// WM_QUIT
	ZeroMemory(&msg, sizeof(MSG));

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			GameRun();
		}
	}

	GameRelease();
	return 0;
}