#pragma once
#include "xStd.h"

class TDevice
{
public:
	ID3D11Device *			m_pd3dDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;	//WINAPI�� ������۸��̶� ����.����۸� ����.
	IDXGIFactory*			m_pDXGIFactory;
	DXGI_SWAP_CHAIN_DESC	m_sd;

	ID3D11RenderTargetView*	m_pRenderTargetView;

public:
	HRESULT CreateDevice();
	HRESULT CreateDX11GIFactory();
	HRESULT CreateSwapChain();
	HRESULT SetRenderTarget();
	void SetViewPort();

public:
	bool Init();
	virtual bool Frame() { return true; }
	bool PreRender();
	bool PostRender();
	bool Render();
	bool Release();
public:
	TDevice();
	virtual ~TDevice();
};

