#include "xCore.h"
#include "TShape.h"
#include "TCameraModelView.h"

class Sample : public xCore
{
	TBoxShape			m_ObjBox;
	TPlaneShape			m_ObjPlane;
	TLineShape			m_ObjLine;
	TSphereShape		m_ObjSphere;
	TCameraModelView*	m_pModelCamera;
	D3DXMATRIX			m_World[100];
public:
	LRESULT	MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return m_pModelCamera->MsgProc(hWnd, msg, wParam, lParam);
	}
	void    DeleteResources()
	{
	}
	HRESULT CreateResources(UINT width, UINT height)
	{
		return S_OK;
	}
	bool Init()
	{
		D3DXMatrixIdentity(&m_World[0]);
		for (int iObj = 1; iObj < 100; iObj++)
		{
			m_World[iObj] = m_World[0];
			m_World[iObj]._43 = iObj * 10;
		}

		m_pModelCamera = new TCameraModelView();
		m_pModelCamera->m_matProj = m_pMainCamera->m_matProj;

		m_pMainCamera = m_pModelCamera;
		m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 30.0f, -10.0f));

		m_ObjLine.Create(m_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");

		m_ObjBox.Create(m_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");

		m_ObjPlane.Create(m_pd3dDevice,
			L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");

		m_ObjSphere.Create(m_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");
		return true;
	}
	bool Frame()
	{
		//m_pMainCamera->ClassifyPoint(m_ObjBox.m_vPosition);
		//m_pMainCamera->ClassifySphere(m_ObjBox.m_vPosition, 150.0f);

		m_ObjLine.Frame();
		m_ObjPlane.Frame();
		m_ObjSphere.Frame();
		return m_ObjBox.Frame();
	}
	bool Render()
	{
		//D3DXMATRIX matWorld, matRot;
		//D3DXMatrixTranslation(&matWorld, 3.0f, 0, 0);
		//D3DXMatrixRotationY(&matRot, m_Timer.m_fGameTime);
		//matWorld = matWorld * matRot;
		//ApplyRS(m_pContext, TDxState::g_pRSNoneCullSolid);
		//// �� ���
		//m_ObjPlane.SetMatrix(&matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		//m_ObjPlane.Render(m_pContext);
		//ApplyRS(m_pContext, TDxState::g_pRSBackCullSolid);

		// �� ���
		//ApplyRS(m_pContext, TDxState::g_pRSWireFrame);
		/*D3DXMatrixTranslation(&matWorld, -3.0f, 0, 0);
		m_ObjSphere.SetMatrix(&matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_ObjSphere.Render(m_pContext);*/
		
		int iRenderCnt = 0;
		for (int iObj = 0; iObj < 100; iObj++)
		{
			D3DXVECTOR3 vPos;
			vPos.x = m_World[iObj]._41;
			vPos.y = m_World[iObj]._42;
			vPos.z = m_World[iObj]._43;
			if( m_pMainCamera->ClassifySphere(vPos, 1.44f) )
			{
				m_ObjBox.SetMatrix(&m_World[iObj], &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
				m_ObjBox.Render(m_pContext);
				iRenderCnt++;
			}
		}

		// �ڽ� ���
		/*m_ObjBox.SetMatrix(&m_pMainCamera->m_mModelRot, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_ObjBox.Render(m_pContext);*/

		
		// �ڽ��� ���� ���ἱ ���
		//m_ObjLine.Draw(m_pContext, m_ObjBox.m_vPosition, m_ObjPlane.m_vPosition, D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f));

		// �� ���⺤�� ���
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
		m_ObjLine.Release();
		m_ObjSphere.Release();
		m_ObjPlane.Release();
		return m_ObjBox.Release();
	}
public:
	Sample() {
	}
	~Sample() {}
};
GAMERUN("SampleModelView", 800, 600);
