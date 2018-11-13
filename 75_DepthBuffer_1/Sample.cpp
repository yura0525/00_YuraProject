#include "xCore.h"
#include "TObjectPlane.h"
#include "TDxState.h"

class Sample : public xCore
{
public:
	TObjectPlane	m_Obj;
	D3D11_VIEWPORT	m_vp[4];
	
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_matView;
	D3DXMATRIX					m_matProj;

public:

	bool Init()
	{
		xCore::Init();

		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixLookAtLH(&m_matView, &D3DXVECTOR3(0,5,-5.0f), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);

		m_Obj.Init();
		m_Obj.Set(m_pd3dDevice);

		/*m_vp[0].TopLeftX = 0;
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
		m_vp[3].MaxDepth = 1.0f;*/
		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_Obj.Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();

		D3DXMATRIX rot;
		D3DXMatrixRotationY(&rot, (float)D3DX_PI * 0.25f);
		D3DXMatrixTranslation(&m_matWorld, 0, 0, 0);
		m_matWorld = rot * m_matWorld;

		D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);
		m_pContext->RSSetState(DX::m_pRSSolidState);
		m_pContext->OMSetDepthStencilState(DX::g_pDSVStateEnableLessEqual, 0);
		m_Obj.Render(m_pContext);


		//2)
		D3DXMATRIX Scale;
		D3DXMatrixScaling(&Scale, 2, 2, 2);
		D3DXMatrixTranslation(&m_matWorld, 0, 0, 1.0f);
		m_matWorld = Scale * m_matWorld;

		D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matView, &m_matView);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matProj, &m_matProj);

		m_pContext->RSSetState(DX::m_pRSSolidState);
		m_pContext->OMSetDepthStencilState(DX::g_pDSVStateEnableLessEqual, 0);

		m_Obj.Render(m_pContext);
		return true;
	}
	bool Release()
	{
		xCore::Release();
		m_Obj.Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Vector_1", 800, 600);