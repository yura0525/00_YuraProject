#pragma once
#include "xStd.h"
class TDevice
{
public:
	ID3D11Device *			m_pd3dDevice;	// ����̽� ��ü
	ID3D11DeviceContext*	m_pContext;		// ����̽� ���ؽ�Ʈ
	IDXGISwapChain*			m_pSwapChain;	//WINAPI�� ������۸��̶� ����.����۸� ����.// ����ü�� ��ü
	IDXGIFactory*			m_pDXGIFactory;	// DXGI ��ü

	ID3D11RenderTargetView*	m_pRenderTargetView;	// ���� ����Ÿ�� ��
	DXGI_SWAP_CHAIN_DESC	m_sd;
public:
	HRESULT CreateDevice();
	HRESULT CreateDX11GIFactory();
	HRESULT CreateSwapChain();
	HRESULT SetRenderTarget();
	void SetViewPort();

public:
	virtual void DeleteDeviceResources(UINT iWidth, UINT iHeight);
	virtual HRESULT CreateDeviceResources(UINT iWidth, UINT iHeight);

	HRESULT ResizeDevice(UINT iWidth, UINT iHeight);
public:
	TDevice();
	virtual ~TDevice();
};

