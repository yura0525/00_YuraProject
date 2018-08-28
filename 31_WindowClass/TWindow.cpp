#include "TWindow.h"
TWindow * g_pWindow = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//DEBUG시에만 작동한다. 조건문이 false이면 예외발생.
	assert( g_pWindow != NULL );

	LRESULT ret;
	
	if (ret = g_pWindow->MsgProc(hWnd, msg, wParam, lParam))
	{
		return ret;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT TWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		//MessageBox(NULL, L"윈도우가 떴다.", L"윈도우 생성", MB_OK);
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
	return 0;
}

bool TWindow::SetWindow(HINSTANCE hInstance, const TCHAR *pTitleName,
						UINT iWidth, UINT iHeight)
{
	
	//윈도우 클래스 등록
	WNDCLASSEX wc;
	//ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MyWindow";
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	m_hInstance = hInstance;

	if (RegisterClassEx(&wc) == FALSE)
	{
		return false;
	}

	//윈도우 생성(등록된 클래스를 사용해서)
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		L"MyWindow",
		pTitleName,
		m_dwStyle,
		0, 0, iWidth, iHeight, NULL, NULL, hInstance, NULL);

	if (m_hWnd == NULL)
	{
		return false;
	}

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);

	CenterWindow();

	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}
void TWindow::Set(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
}

void TWindow::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeigth = GetSystemMetrics(SM_CYFULLSCREEN);

	int x = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
	int y = (iScreenHeigth - (m_rtWindow.bottom - m_rtWindow.top)) / 2;

	MoveWindow(m_hWnd, x, y, m_rtWindow.right, m_rtWindow.bottom, true);
}
bool TWindow::Run()
{
	if ( !GameInit() )
		return false;
	
	//GetMessage(&msg, NULL, 0, 0)
	//메세지큐에서 메세지가 1개이상 있을 경우 1개를 가져옴.
	//PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)
	//메세지큐에서 메세지 있으면 메세지를 반환, 없으면 false를 반환.
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

	return GameRelease();
}

bool TWindow::GameInit()
{
	return true;
}

bool TWindow::GameRun()
{
	return true;
}

bool TWindow::GameRelease()
{
	return true;
}

TWindow::TWindow()
{
	g_pWindow = this;
	m_dwStyle = WS_OVERLAPPEDWINDOW;
}


TWindow::~TWindow()
{
}

