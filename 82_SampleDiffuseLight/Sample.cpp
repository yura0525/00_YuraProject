#include "xCore.h"
#include "TShape.h"
#include "TCameraModelView.h"
#include "xMap.h"

struct LIGHT_CONSTANT_BUFFER
{
	D3DXMATRIX g_matInvWorld;
	D3DXVECTOR4 g_vLightDir;
	D3DXVECTOR4 g_vLightPos;
	D3DXVECTOR4 g_vEyeDir;
	D3DXVECTOR4 g_vEyePos;
};
class Sample : public xCore
{
	xMap		m_map;
	TLineShape	m_ObjLine;

	ComPtr<ID3D11Buffer>	m_pCBLight;
	LIGHT_CONSTANT_BUFFER	m_cbLightData;
	D3DXVECTOR3 m_vLightPos;
public:
	bool Init()
	{
		HRESULT hr = S_OK;
		m_vLightPos = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

		DX::CreateConstantBuffer(m_pd3dDevice,
			1,
			sizeof(LIGHT_CONSTANT_BUFFER),
			m_pCBLight.GetAddressOf(),
			&m_cbLightData, false);

		m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 30.0f, -30.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		xMapDesc desc = {5, 5, 1.0f, 1.0f, L"../../data/map/castle.jpg", L"../../data/shader/mapLight.hlsl"};

		m_map.Init();
		m_map.CreateHeightMap(m_pd3dDevice, m_pContext, L"../../data/map/HEIGHT_CASTLE.bmp");
		desc.iNumRows = m_map.m_iNumRows;
		desc.iNumCols = m_map.m_iNumCols;
		desc.fScaleHeight = 0.1f;

		if (m_map.Load(m_pd3dDevice, desc) == false)
		{
			return false;
		}
		
		m_ObjLine.Create(m_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/eye.bmp");

		return true;
	}

	bool Frame()
	{
		D3DXVECTOR3 vLightDir;
		D3DXMATRIX matRotY;
		//D3DXMatrixRotationY(&matRotY, 0.0f);
		D3DXMatrixRotationY(&matRotY, m_Timer.m_fGameTime * 0.5f);
		D3DXVec3TransformCoord(&vLightDir, &m_vLightPos, &matRotY);
		D3DXVec3Normalize(&vLightDir, &-vLightDir);
		m_cbLightData.g_vLightDir.x = vLightDir.x;
		m_cbLightData.g_vLightDir.y = vLightDir.y;
		m_cbLightData.g_vLightDir.z = vLightDir.z;
		m_cbLightData.g_vLightDir.w = 10.0f;		//intensity(세기, 강도)

		m_cbLightData.g_vLightPos.x = m_vLightPos.x;
		m_cbLightData.g_vLightPos.y = m_vLightPos.y;
		m_cbLightData.g_vLightPos.z = m_vLightPos.z;
		m_cbLightData.g_vLightPos.w = 200.0f;			//radius(반지름)

		m_cbLightData.g_vEyeDir.x = m_pMainCamera->m_vLook.x;
		m_cbLightData.g_vEyeDir.y = m_pMainCamera->m_vLook.y;
		m_cbLightData.g_vEyeDir.z = m_pMainCamera->m_vLook.z;
		m_cbLightData.g_vEyeDir.w = 10.0f;		//intensity(세기, 강도)

		m_cbLightData.g_vEyePos.x = m_pMainCamera->m_vPos.x;
		m_cbLightData.g_vEyePos.y = m_pMainCamera->m_vPos.y;
		m_cbLightData.g_vEyePos.z = m_pMainCamera->m_vPos.z;
		m_cbLightData.g_vEyePos.w = 200.0f;			//radius(반지름)


		return true;
	}
	bool Render()
	{
		m_map.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		D3DXMatrixInverse(&m_cbLightData.g_matInvWorld, NULL, &m_pMainCamera->m_matWorld);
		D3DXMatrixTranspose(&m_cbLightData.g_matInvWorld, &m_cbLightData.g_matInvWorld);
		D3DXMatrixTranspose(&m_cbLightData.g_matInvWorld, &m_cbLightData.g_matInvWorld);
		m_pContext->UpdateSubresource(m_pCBLight.Get(), 0, NULL, &m_cbLightData, 0, 0);

		m_pContext->VSSetConstantBuffers(1, 1, m_pCBLight.GetAddressOf());
		m_pContext->PSSetConstantBuffers(1, 1, m_pCBLight.GetAddressOf());

		m_map.Render(m_pContext);

		for (int iVertex = 0; iVertex < m_map.m_VertexList.size(); iVertex+= 100)
		{
			D3DXVECTOR3 vNormal = m_map.m_VertexList[iVertex].n;
			m_ObjLine.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
			m_ObjLine.Draw(m_pContext, m_map.m_VertexList[iVertex].p, m_map.m_VertexList[iVertex].p + vNormal,
							D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		
		return true;
	}

	bool Release()
	{
		m_map.Release();
		m_ObjLine.Release();
		return true;
	}
public:
	Sample() {
	}
	~Sample() {}
};
GAMERUN("SampleModelView", 800, 600);
