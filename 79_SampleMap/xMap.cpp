#include "xMap.h"


HRESULT xMap::CreateVertexData()
{
	m_iNumVertex = m_iNumVerties;
	m_VertexList.resize(m_iNumVertex);

	float fHalfCols = (m_iNumCols - 1) / 2.0f;
	float fHalfRows = (m_iNumRows - 1) / 2.0f;
	float fOffsetU = 1.0f / (m_iNumCols - 1);
	float fOffsetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int iIndex = (iRow * m_iNumCols) + iCol;
			m_VertexList[iIndex].p.x = (iCol - fHalfCols) * m_fCellDistance;
			m_VertexList[iIndex].p.y = GetHeightOfVertex(iIndex);
			m_VertexList[iIndex].p.z = -((iRow - fHalfRows) * m_fCellDistance);
			m_VertexList[iIndex].n = GetNormalOfVertex(iIndex);
			m_VertexList[iIndex].c = GetColorOfVertex(iIndex);
			m_VertexList[iIndex].t = GetTextureOfVertex(fOffsetU * iCol, fOffsetV * iRow);
		}
	}
	return S_OK;
}
HRESULT xMap::CreateIndexData()
{
	m_iNumIndex = m_iNumFace * 3;
	m_IndexList.resize(m_iNumIndex);

	int iCurIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			//0	1   4   
			//2	3	5
			int iNextRow = iRow + 1;
			m_IndexList[iCurIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 1] = m_IndexList[iCurIndex + 0] + 1;
			m_IndexList[iCurIndex + 2] = iNextRow * m_iNumCols + iCol;	//0번의 아랫줄이다.
			m_IndexList[iCurIndex + 3] = m_IndexList[iCurIndex + 2];
			m_IndexList[iCurIndex + 4] = m_IndexList[iCurIndex + 1];
			m_IndexList[iCurIndex + 5] = iNextRow * m_iNumCols + iCol+1;

			iCurIndex += 6;
		}
	}

	return S_OK;
}


float xMap::GetHeightOfVertex(int iIndex)
{
	return 0.0f;
}

D3DXVECTOR2 xMap::GetTextureOfVertex(float fU, float fV)
{
	return D3DXVECTOR2(fU, fV);
}

D3DXVECTOR3 xMap::GetNormalOfVertex(int iIndex)
{
	return D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

D3DXVECTOR4 xMap::GetColorOfVertex(int iIndex)
{
	return D3DXVECTOR4(1.0f, 1.0f, 1.0f,1.0f);
}

bool xMap::CreateMap(xMapDesc desc)
{
	//2의 엔승 + 1개가 가장 좋다. 1 + 1, 2 + 1, 4 + 1, 8 + 1, 16 + 1
	if (desc.iNumCols > 1025)
		desc.iNumCols = 1025;

	if (desc.iNumRows > 1025)
		desc.iNumRows = 1025;

	m_iNumRows = desc.iNumRows;
	m_iNumCols = desc.iNumCols;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumVerties = m_iNumRows * m_iNumCols;
	m_iNumFace = m_iNumCellRows * m_iNumCellCols * 2;
	m_fCellDistance = desc.fDistance;

	return true;
}
bool xMap::Load(ID3D11Device *pDevice, xMapDesc desc)
{
	m_pd3dDevice = pDevice;

	if ( !CreateMap(desc) )
	{
		return false;
	}
	
	TBoxShape::Create(m_pd3dDevice, desc.szShaderFile, desc.szTextureFile);
	return true;
}

bool xMap::Init()
{
	return true;
}
bool xMap::Frame()
{
	return true;
}
bool xMap::Render(ID3D11DeviceContext* pContext)
{
	TBoxShape::Render(pContext);
	return true;
}
bool xMap::Release()
{
	return true;
}

xMap::xMap()
{
}


xMap::~xMap()
{
}
