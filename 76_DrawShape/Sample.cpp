#include "TDxState.h"
#include "xCore.h"
#include "TShape.h"

using namespace DX;

class Sample : public xCore
{
public:
	TBoxShape		m_Obj;
	
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_matView;
	D3DXMATRIX					m_matProj;
public:

	void DeleteResources(UINT iWidth, UINT iHeight)
	{

	}
	HRESULT CreateResources(UINT iWidth, UINT iHeight)
	{
		return S_OK;
	}

	bool Init()
	{
		HRESULT hr = S_OK;
		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixLookAtLH(&m_matView, &D3DXVECTOR3(0, 3, -10.0f), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);

		m_Obj.Create(m_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");
		return true;
	}
	bool Frame()
	{
		m_Obj.Frame();
		return true;
	}
	bool Render()
	{
		D3DXMATRIX matTrans, matRotation;
		D3DXMatrixTranslation(&matTrans, 3.0f, 0.0f, 0.0f);
		D3DXMatrixRotationY(&matRotation, m_Timer.m_fGameTime);
		m_matWorld = matTrans * matRotation;

		D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matView, &m_matView);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matProj, &m_matProj);

		m_Obj.SetMatrix(&m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_Obj.Render(m_pContext);


		//2)
		D3DXMATRIX Scale;
		D3DXMatrixScaling(&Scale, 2, 2, 2);
		D3DXMatrixTranslation(&m_matWorld, 0, 0, 1.0f);
		m_matWorld = Scale * m_matWorld;

		D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matView, &m_matView);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matProj, &m_matProj);

		m_Obj.SetMatrix(&m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_Obj.Render(m_pContext);
		return true;
	}
	bool Release()
	{
		m_Obj.Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("DrawShape(x)", 800, 600);