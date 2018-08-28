#include "TWindow.h"
#include <tchar.h>

#pragma comment (lib, "winmm.lib")

int g_iFPS = 0;
int g_iTmpCount = 0;
float g_fSecPerFrame = 0.0f;
float g_fGameTime = 0.0f;
TCHAR g_szBuffer[256];
DWORD m_dwKeyState[256];

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
bool TWindow::Init(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"MyWindow";

	if (RegisterClassEx(&wc) == false)
	{
		return false;
	}

	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"MyWIndow",L"aaaaa",
		WS_OVERLAPPEDWINDOW, 0,0, 1024, 768, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
		return false;

	ShowWindow(hWnd, SW_SHOW);

	HBITMAP hBitmap = (HBITMAP)LoadImage(hInstance, L"test.bmp", IMAGE_BITMAP, 0, 0, 
											LR_DEFAULTSIZE | LR_LOADFROMFILE);
	HDC hdc = GetDC(hWnd);
	HDC hMemDC = CreateCompatibleDC(hdc);

	//셋팅
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	DWORD dwTick = timeGetTime();

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			DWORD dwCurrentTick = timeGetTime();
			DWORD dwElapsedTick  = dwCurrentTick  - dwTick;

			g_fSecPerFrame = dwElapsedTick / 1000.0f;

			g_fGameTime += g_fSecPerFrame;

			static float fTime = 0.0f;

			fTime += g_fSecPerFrame;
			if (fTime >= 1.0f)
			{
				g_iFPS = g_iTmpCount;
				fTime -= 1.0f;

				g_iTmpCount = 0;
			}

			dwTick = dwCurrentTick;

			_stprintf_s(g_szBuffer, L"%d : %10.4f : %10.4f", g_iFPS, g_fSecPerFrame, g_fGameTime);

			//출력 dc
			BitBlt(hdc, 0, 0, 1024, 768, hMemDC, 0, 0, SRCCOPY);

			//wcslen 유니코드일때 문자열의 길이를 구하는 함수.
			//#define _tcslen         wcslen
			TextOut(hdc, 100, 100, g_szBuffer, _tcslen(g_szBuffer));
			g_iTmpCount++;

			for (int iKey = 0; iKey < 255; iKey++)
			{
				SHORT sKey = GetAsyncKeyState(iKey);
				m_dwKeyState[iKey] = (sKey & 0x8000) ? 1 : 0;
			}

			if (m_dwKeyState[VK_LBUTTON])
			{
				MessageBox(NULL, L"VK_LBUTTON", L"MOUSE", MB_OK);
			}
			if (m_dwKeyState['W'])
			{
				MessageBox(NULL, L"W", L"KEYBOARD", MB_OK);
			}
			if (m_dwKeyState[VK_LCONTROL] &&
				m_dwKeyState[VK_RBUTTON])
			{
				MessageBox(NULL, L"COMBO", L"KEY + MOUSE", MB_OK);
			}
		}
	}

	//복원
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hMemDC);
	ReleaseDC(hWnd, hdc);

	return true;
}

TWindow::TWindow()
{
}


TWindow::~TWindow()
{
}

