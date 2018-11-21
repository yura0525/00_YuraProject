#pragma once
#include "xWindow.h"
#include "TDirectWrite.h"
#include "TTimer.h"
#include "xInput.h"
#include "TCamera.h"

struct TOutput									//모니터의 정보와 갯수를 알수있다.
{
	DXGI_OUTPUT_DESC				desc;
	std::vector<DXGI_MODE_DESC>		m_pModeList;
	std::vector<IDXGIOutput*>		m_pOutputList;
};
struct TAdapter									//그래픽카드
{
	IDXGIAdapter*				m_pAdapter;
	DXGI_ADAPTER_DESC			desc;
	std::vector<TOutput>		m_OutputList;
};

class xCore : public xWindow
{
public:
	TTimer			m_Timer;
	TDirectWrite	m_Font;
	xInput			m_Input;
	TCamera			m_DefaultCamera;
	TCamera*		m_pMainCamera;

	ID3D11DepthStencilView*		m_pDSV;
	D3DXVECTOR4					m_YawPitchRoll;
public:
	bool GamePreInit();

	bool GameInit();
	bool GameRun();
	bool GameRelease();

	bool GameFrame();
	bool GameRender();
	bool GamePreRender();
	bool GamePostRender();
	void DeleteDeviceResources(UINT iWidth, UINT iHeight);
	HRESULT CreateDeviceResources(UINT iWidth, UINT iHeight);

	virtual void DeleteResources(UINT iWidth, UINT iHeight) {}
	virtual HRESULT CreateResources(UINT iWidth, UINT iHeight) { return S_OK; }

	HRESULT CreateDSV();
public:
	virtual bool Init()			{	return true;	}
	virtual bool Frame()		{	return true;	}
	virtual bool PreRender()	{	return true;	}
	virtual bool PostRender()	{	return true;	}
	virtual bool Render()		{	return true;	}
	virtual bool Release()		{	return true;	}

public:
	xCore();
	virtual ~xCore();
};

