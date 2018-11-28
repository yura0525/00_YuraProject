#include "xCore.h"
#include "TShape.h"
#include "TCameraModelView.h"
#include "xMap.h"

class Sample : public xCore
{
	xMap	m_map;
public:
	bool Init()
	{
		m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 300.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		xMapDesc desc = {257, 257, 1.0f, 1.0f, L"../../data/map/Tile40.jpg", L"../../data/shader/map.hlsl"};

		m_map.Init();
		m_map.CreateHeightMap(m_pd3dDevice, m_pContext, L"../../data/map/heightMap513.bmp");
		desc.iNumRows = m_map.m_iNumRows;
		desc.iNumCols = m_map.m_iNumCols;
		desc.fScaleHeight = 0.5f;

		if (m_map.Load(m_pd3dDevice, desc) == false)
		{
			return false;
		}
		
		return true;
	}

	bool Render()
	{
		m_map.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_map.Render(m_pContext);
		return true;
	}

	bool Release()
	{
		m_map.Release();
		return true;
	}
public:
	Sample() {
	}
	~Sample() {}
};
GAMERUN("SampleModelView", 800, 600);
