#include "xCore.h"
#include "TShape.h"
#include "TCameraModelView.h"
#include "xMap.h"

class Sample : public xCore
{
	xMap		m_map;
	TLineShape	m_ObjLine;
public:
	bool Init()
	{
		m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 50.0f, -50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		xMapDesc desc = {5, 5, 1.0f, 1.0f, L"../../data/map/Tile40.jpg", L"../../data/shader/map.hlsl"};

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

	bool Render()
	{
		m_map.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_map.Render(m_pContext);

		for (int iVertex = 0; iVertex < m_map.m_VertexList.size(); iVertex+= 1000)
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
