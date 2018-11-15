#include "xCore.h"
#include "TObjectPlane.h"

class Sample : public xCore
{
public:
	TObjectPlane	m_ObjBox;
	D3D11_VIEWPORT	m_vp[4];

public:
	bool Init()
	{
		xCore::Init();
		m_ObjBox.Init();
		m_ObjBox.Set(m_pd3dDevice);

		m_vp[0].TopLeftX = 0;
		m_vp[0].TopLeftY = 0;
		m_vp[0].Width = g_rtClient.right / 2;
		m_vp[0].Height = g_rtClient.bottom / 2;
		m_vp[0].MinDepth = 0.0f;
		m_vp[0].MaxDepth = 1.0f;

		m_vp[1].TopLeftX = g_rtClient.right / 2;
		m_vp[1].TopLeftY = 0;
		m_vp[1].Width = g_rtClient.right / 2;
		m_vp[1].Height = g_rtClient.bottom / 2;
		m_vp[1].MinDepth = 0.0f;
		m_vp[1].MaxDepth = 1.0f;


		m_vp[2].TopLeftX = 0;
		m_vp[2].TopLeftY = g_rtClient.bottom / 2;
		m_vp[2].Width = g_rtClient.right / 2;
		m_vp[2].Height = g_rtClient.bottom / 2;
		m_vp[2].MinDepth = 0.0f;
		m_vp[2].MaxDepth = 1.0f;


		m_vp[3].TopLeftX = g_rtClient.right / 2;
		m_vp[3].TopLeftY = g_rtClient.bottom / 2;
		m_vp[3].Width = g_rtClient.right / 2;
		m_vp[3].Height = g_rtClient.bottom / 2;
		m_vp[3].MinDepth = 0.0f;
		m_vp[3].MaxDepth = 1.0f;
		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_ObjBox.Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();

		//1)
		m_ObjBox.m_cbData.fActiveVP = 0;
		m_pContext->RSSetViewports(1, &m_vp[0]);
		m_ObjBox.Render(m_pContext);

		//2)
		m_ObjBox.m_cbData.fActiveVP = 0;
		m_pContext->RSSetViewports(1, &m_vp[1]);
		m_ObjBox.Render(m_pContext);

		//3)
		m_pContext->RSSetViewports(2, &m_vp[2]);
		m_ObjBox.m_cbData.fActiveVP = 0;
		m_pContext->GSSetConstantBuffers(0, 1, m_ObjBox.m_dxObj.m_pConstantBuffer.GetAddressOf());
		m_ObjBox.Render(m_pContext);
		
		//4)
		m_ObjBox.m_cbData.fActiveVP = 1;
		m_ObjBox.Render(m_pContext);
		return true;
	}
	bool Release()
	{
		xCore::Release();
		m_ObjBox.Release();

		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Vector_1", 800, 600);