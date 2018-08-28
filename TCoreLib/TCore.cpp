#include "TCore.h"
#include "TSound.h"

HDC g_hOffScreenDC = NULL;
HDC g_hScreenDC = NULL;

bool TCore::GameInit()
{
	m_Timer.Init();
	I_Input.Init();
	TSound::GetInstance()->Init();

	m_hScreenDC = GetDC(m_hWnd);
	g_hScreenDC = m_hScreenDC;

	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);		//메인DC를 가지고 호환가능한 DC를 만든다. 항상 m_hScreenDC로 만들어야한다.
	g_hOffScreenDC = m_hOffScreenDC;

	m_hOffScreenBitmap = CreateCompatibleBitmap(m_hScreenDC, m_rtClient.right, m_rtClient.bottom);

	m_hOldOffBitmap = (HBITMAP)SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);

	COLORREF bkColor = RGB(0, 0, 0);
	m_hbrBack = CreateSolidBrush(bkColor);
	SelectObject(m_hOffScreenDC, m_hbrBack);

	HANDLE m_hPen = CreatePen(PS_SOLID, 0, RGB(255,0,0));
	SelectObject(m_hOffScreenDC, m_hPen);

	Init();

	return true;
}

bool TCore::GameFrame()
{
	m_Timer.Frame();
	I_Input.Frame();
	TSound::GetInstance()->Frame();

	Frame();

	return true;
}

bool TCore::GameRender()
{
	GamePreRender();

	Render();
	m_Timer.Render();
	I_Input.Render();
	TSound::GetInstance()->Render();

	GamePostRender();
	return true;
}

bool TCore::GamePreRender()
{
	SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);
	//PreRender(팔렛트 블릿)
	PatBlt(m_hOffScreenDC, 0, 0, m_rtClient.right, m_rtClient.bottom, PATCOPY);
	return true;
}
bool TCore::GamePostRender()
{
	//PostRender
	BitBlt(m_hScreenDC, 0, 0, m_rtClient.right, m_rtClient.bottom, m_hOffScreenDC, 0, 0, SRCCOPY);
	SelectObject(m_hOffScreenDC, m_hOldOffBitmap);
	return true;
}

bool TCore::GameRun()
{
	GameFrame();
	GameRender();

	return true;
}
bool TCore::GameRelease()
{
	Release();
	m_Timer.Release();
	I_Input.Release();
	TSound::GetInstance()->Release();

	DeleteObject(m_hbrBack);
	DeleteObject(m_hOffScreenBitmap);
	DeleteObject(m_hPen);

	ReleaseDC(m_hWnd, m_hScreenDC);
	ReleaseDC(m_hWnd, m_hOffScreenDC);
	return true;
}
TCore::TCore()
{
}


TCore::~TCore()
{
}
