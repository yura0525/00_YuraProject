#include "xCore.h"
#include "TObjectPlane.h"

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

		D3DXMatrixIdentity(&m_Obj.m_cbData.matWorld);
		D3DXMatrixLookAtLH(&m_Obj.m_cbData.matView, &D3DXVECTOR3(0,0,-5.0f), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
		D3DXMatrixPerspectiveFovLH(&m_Obj.m_cbData.matProj, D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);

		m_Obj.Init();
		m_Obj.Set(m_pd3dDevice);

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
		m_Obj.Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();

		m_pContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDSV);
		m_pContext->OMSetDepthStencilState(m_pDSVStateEnableLess, 0x00);
		m_pContext->OMSetDepthStencilState(m_pDSVStateEnable, 0x00);

		m_matWorld._43 = 1.0f;
		m_Obj.m_cbData.vColor = D3DXVECTOR4(1, 0, 0, 1);

		D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matView, &m_matView);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matProj, &m_matProj);

		//m_Obj.m_cbData.fActiveVP = 0;
		//m_pContext->RSSetViewports(1, &m_vp[0]);
		m_Obj.Render(m_pContext);

		//m_Obj.m_cbData.fActiveVP = 0;
		//m_pContext->RSSetViewports(1, &m_vp[1]);m_Obj.m_cbData.vColor = D3DXVECTOR4(0, 0, 1, 1);
		m_Obj.m_cbData.vColor = D3DXVECTOR4(0, 0, 1, 1);
		D3DXMatrixRotationX(&m_matWorld, m_Timer.m_fGameTime);

		D3DXMATRIX matPos;
		D3DXMatrixTranslation(&matPos, 1.0f, 0, 3.0f);

		m_matWorld = matPos * m_matWorld;
		//m_matWorld._43 = 3.0f;
		//m_matWorld._41 = 1.0f;
		D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);
		m_Obj.Render(m_pContext);

		//m_pContext->RSSetViewports(2, &m_vp[2]);
		//2
		//m_Obj.m_cbData.fActiveVP = 0;
		//m_pContext->GSSetConstantBuffers(0, 1, m_Obj.m_dxObj.m_pConstantBuffer.GetAddressOf());
		//m_Obj.Render(m_pContext);
		
		//3
		//m_Obj.m_cbData.fActiveVP = 1;
		//m_pContext->RSSetViewports(2, &m_vp[2]);
		//m_Obj.Render(m_pContext);
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