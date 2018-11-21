#include "TDxState.h"
#include "xCore.h"
#include "TShape.h"

using namespace DX;

class Sample : public xCore
{
public:
	TBoxShape			m_ObjBox;
	TPlaneShape		m_ObjPlane;
	TLineShape		m_ObjLine;
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
		m_pMainCamera->SetViewMatrix(D3DXVECTOR3(3.0f, 3.0f, -5.0f));

		m_ObjBox.Create(m_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");
		m_ObjPlane.Create(m_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");
		m_ObjLine.Create(m_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");
		return true;
	}
	bool Frame()
	{
		m_ObjBox.Frame();
		//m_ObjPlane.Frame();
		m_ObjLine.Frame();
		return true;
	}
	bool Render()
	{
		D3DXMATRIX matWorld, matRot;
		D3DXMatrixTranslation(&matWorld, 3.0f, 0.0f, 0.0f);
		D3DXMatrixRotationY(&matRot, m_Timer.m_fGameTime);
		matWorld = matWorld * matRot;
		ApplyRS(m_pContext, TDxState::g_pRSNoneCullSolid);

		// 면 출력
		m_ObjPlane.SetMatrix(&matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_ObjPlane.Render(m_pContext);

		// 박스 출력
		ApplyRS(m_pContext, TDxState::g_pRSBackCullSolid);
		m_ObjBox.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_ObjBox.Render(m_pContext);

		// 박스와 면의 연결선 출력
		//m_ObjLine.Draw(m_pContext, m_ObjBox.m_vPosition, m_ObjPlane.m_vPosition, D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f));

		//// 면 방향벡터 출력
		m_ObjLine.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		D3DXVECTOR3 vDir = m_ObjPlane.m_vLook * 10.0f;
		m_ObjLine.Draw(m_pContext, m_ObjPlane.m_vPosition, m_ObjPlane.m_vPosition + vDir, D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
		vDir = m_ObjPlane.m_vUp * 10.0f;
		m_ObjLine.Draw(m_pContext, m_ObjPlane.m_vPosition, m_ObjPlane.m_vPosition + vDir, D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
		vDir = m_ObjPlane.m_vSide * 10.0f;
		m_ObjLine.Draw(m_pContext, m_ObjPlane.m_vPosition, m_ObjPlane.m_vPosition + vDir, D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
		return true;
	}
	bool Release()
	{
		m_ObjBox.Release();
		//m_ObjLine.Release();
		//m_ObjPlane.Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Camera(x)", 800, 600);