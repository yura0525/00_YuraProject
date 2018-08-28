#pragma once
#include "TStd.h"

class TWindow
{
public:
	HWND			m_hWnd;
	HINSTANCE		m_hInstance;
	RECT			m_rtWindow;
	RECT			m_rtClient;

private:
	DWORD			m_dwStyle;

public:
	void Set(DWORD dwStyle);
	void CenterWindow();

	bool SetWindow(HINSTANCE hInstance, const TCHAR *pTitleName = L"KGCA Sample",
		UINT iWidth = 800, UINT iHeight = 600);

	bool Run();

public:
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual bool GameInit();
	virtual bool GameRun();
	virtual bool GameRelease();

public:
	TWindow();
	virtual ~TWindow();
};

