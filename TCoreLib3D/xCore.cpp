#include "xCore.h"

bool xCore::Init()
{
	xWindow::Init();
	m_Timer.Init();
	m_Font.Init();

	DeleteDeviceResources(m_sd.BufferDesc.Width, m_sd.BufferDesc.Height);
	CreateDeviceResources(m_sd.BufferDesc.Width, m_sd.BufferDesc.Height);
	return 0;
}
bool xCore::Frame()
{
	xWindow::Frame();
	m_Timer.Frame();
	m_Font.Frame();
	return 0;
}

bool xCore::PreRender()
{
	return xWindow::PreRender();
}
bool xCore::PostRender()
{
	return xWindow::PostRender();
}
bool xCore::Render()
{
	xWindow::Render();
	m_Timer.Render();
	m_Font.Render();
	return 0;
}
bool xCore::Release()
{
	xWindow::Release();
	m_Timer.Release();
	m_Font.Release();
	return 0;
}

bool xCore::GameInit()
{
	GamePreInit();
	Init();

	return true;
}

bool xCore::GameFrame()
{
	Frame();

	return true;
}

bool xCore::GameRender()
{
	GamePreRender();
	{
		PreRender();
		Render();
		PostRender();
	}
	GamePostRender();
	return true;
}

bool xCore::GamePreRender()
{
	return true;
}
bool xCore::GamePostRender()
{
	return true;
}

void xCore::DeleteDeviceResources(UINT iWidth, UINT iHeight)
{
	m_Font.DiscardDeviceResources();
}
HRESULT xCore::CreateDeviceResources(UINT iWidth, UINT iHeight)
{
	IDXGISurface1*		m_pBackBuffer;

	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&m_pBackBuffer);
	hr = m_Font.CreateDeviceResources(m_pBackBuffer);
	
	if (m_pBackBuffer)	m_pBackBuffer->Release();
	
	return S_OK;
}
bool xCore::GameRun()
{
	GameFrame();
	GameRender();

	return true;
}
bool xCore::GameRelease()
{
	Release();
	return true;
}
xCore::xCore()
{
}


xCore::~xCore()
{
}
