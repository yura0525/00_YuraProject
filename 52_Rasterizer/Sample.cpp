#include "xCore.h"
#include "xTriangleObj.h"
#include "xPlaneObj.h"

class Sample : public xCore
{
public:
	xPlaneObj		m_Object;
	ID3D11RasterizerState*      m_pRSWireFrame;
	ID3D11RasterizerState*      m_pRSSolid;

public:
	

public:
	bool Init()
	{
		xCore::Init();
	
		SetRasterizerState();

		m_Object.Init();
		m_Object.Create(m_pd3dDevice, L"vertexshader.txt", L"../../data/hud.dds");
		

		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_Object.Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();
		m_Object.Render(m_pContext);

		//W키를 누르면 와이어 프레임이 보이게함.
		if (g_Input.bFront)
		{
			m_pContext->RSSetState(m_pRSWireFrame);
		}
		else
		{
			m_pContext->RSSetState(m_pRSSolid);
		}
		return true;
	}
	bool Release()
	{
		xCore::Release();
		m_Object.Release();
		if (m_pRSWireFrame != NULL)		m_pRSWireFrame->Release();
		if (m_pRSSolid != NULL)			m_pRSSolid->Release();

		return true;
	}
	HRESULT SetRasterizerState(D3D11_FILL_MODE fill = D3D11_FILL_SOLID)
	{
		HRESULT hr = S_OK;
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
		desc.DepthClipEnable = TRUE;

		hr = m_pd3dDevice->CreateRasterizerState(&desc, &m_pRSWireFrame);

		desc.FillMode = D3D11_FILL_SOLID;
		hr = m_pd3dDevice->CreateRasterizerState(&desc, &m_pRSSolid);
		return hr;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("AlphaBlend", 800, 600);