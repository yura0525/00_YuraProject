#include <windows.h>



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			MessageBox(NULL, L"�����찡 ����.", L"������ ����", MB_OK);
		}
		break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);		// WM_QUIT �޼����� �޽���ť�� Ǫ���ȴ�.
			//DestroyWindow(hWnd);	// WM_DESTROY �޼����� �޼���ť�� Ǫ���ȴ�.(�ٸ���ư���� �����Ҷ�)
		}
		return 0;
		//default:
		//	return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);return 0;
}

void GameInit()
{
	//����, ������Ʈ, ����Ʈ, UI �ʱ�ȭ, �ε�
	MessageBox(NULL, L"���� �ʱ�ȭ", L"���� ����", MB_OK);
}

void GameRun()
{
	//����, ������Ʈ, ����Ʈ, UI
	//MessageBox(NULL, L"���� ������", L"���� ����", MB_OK);
}

void GameRelease()
{
	//����, ������Ʈ, ����Ʈ, UI �Ҹ�
	MessageBox(NULL, L"���� ����", L"���� ����", MB_OK);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
{
	//������ Ŭ���� ���
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

	//������ ����(��ϵ� Ŭ������ ����ؼ�)
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
	//�޼���ť���� �޼����� 1���̻� ���� ��� 1���� ������.
	//�޼���ť�� �޼����� ������쿣 ��ȯ�� ���ϰ� �����. ����� �Լ�

	//PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)
	//�޼���ť���� �޼��� ������ �޼����� ��ȯ, ������ false�� ��ȯ.
	//�޼��� ť�� �޼����� ������ ������ ������ ��ȯ.
	//�Ϲ������� PeekMessage�� ����.
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