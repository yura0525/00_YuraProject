#include "TShape.h"

HRESULT TLineShape::CreateVertexData()
{
	HRESULT hr = S_OK;
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	// 앞면
	m_VertexList.resize(2);
	m_VertexList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));

	m_iNumVertexSize = sizeof(PNCT_VERTEX);
	m_iNumVertex = m_VertexList.size();
	return hr;
}
HRESULT TLineShape::CreateIndexData()
{
	HRESULT hr = S_OK;
	m_IndexList.resize(2);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1;

	m_iNumIndex = m_IndexList.size();
	m_iNumIndexSize = sizeof(DWORD);
	return hr;
}

bool TLineShape::Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor)
{
	PNCT_VERTEX vertextList[2];
	vertextList[0] = m_VertexList[0];
	vertextList[1] = m_VertexList[1];
	vertextList[0].p = vStart;
	vertextList[0].c = vColor;
	vertextList[1].p = vEnd;
	vertextList[1].c = vColor;

	pContext->UpdateSubresource(m_dxObj.m_pVertexBuffer.Get(), 0, NULL, &vertextList, 0, 0);
	Render(pContext);

	//얘만 라인리스트이므로 그리고 나서 원래의 값으로 되돌려준다.
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

HRESULT TLineShape::LoadPixelShader(T_STR szShaderName)
{
	HRESULT hr = S_OK;
	m_dxObj.m_pPixelShader.Attach(DX::LoadPixelShaderFile(m_pd3dDevice, szShaderName.c_str(), NULL, "PSLine"));
	return hr;
}


TLineShape::TLineShape()
{
	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}


TLineShape::~TLineShape()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT TDirLineShape::CreateVertexData()
{
	HRESULT hr = S_OK;
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	// 앞면
	m_VertexList.resize(6);
	m_VertexList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[3] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[4] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));


	m_iNumVertexSize = sizeof(PNCT_VERTEX);
	m_iNumVertex = m_VertexList.size();
	return hr;
}
HRESULT TDirLineShape::CreateIndexData()
{
	HRESULT hr = S_OK;
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5;

	m_iNumIndex = m_IndexList.size();
	m_iNumIndexSize = sizeof(DWORD);
	return hr;
}

bool TDirLineShape::Render(ID3D11DeviceContext* pContext)
{
	TLineShape::Render(pContext);

	//얘만 라인리스트이므로 그리고 나서 원래의 값으로 되돌려준다.
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

TDirLineShape::TDirLineShape()
{
	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}


TDirLineShape::~TDirLineShape()
{
}