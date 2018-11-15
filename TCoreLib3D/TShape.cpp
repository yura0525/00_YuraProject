#include "TShape.h"

void TShape::SetColor(D3DXVECTOR4 vColor)
{
	m_cbData.vColor = vColor;
}

bool TShape::Create(ID3D11Device *pDevice, T_STR szShaderName, T_STR szTextureName)
{
	m_pd3dDevice = pDevice;
	CreateVertexData();
	CreateIndexData();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	
	
	LoadVertexShader(szShaderName);
	LoadPixelShader(szShaderName);
	LoadGeometryShader(szShaderName);

	CreateInputLayout();
	LoadTexture(szTextureName);
	return true;
}

HRESULT TShape::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	//버텍스버퍼, 인덱스버퍼. 벡터랑 배열만 가능하다.
	//시작주소로부터 연속된 메모리여야 한다.
	DX::CreateVertexBuffer(m_pd3dDevice,
		m_iNumVertex,
		m_iNumVertexSize,
		&m_VertexList.at(0),
		m_dxObj.m_pVertexBuffer.GetAddressOf());
	return hr;
}
HRESULT TShape::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	DX::CreateIndexBuffer(m_pd3dDevice,
		m_iNumIndex,
		m_iNumIndexSize,
		&m_IndexList.at(0),
		m_dxObj.m_pIndexBuffer.GetAddressOf());

	return hr;
}

HRESULT TShape::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	DX::CreateConstantBuffer(m_pd3dDevice,
		1,
		sizeof(T_CB_DATA),
		m_dxObj.m_pConstantBuffer.GetAddressOf(),
		&m_cbData, false);
	return hr;
}

HRESULT TShape::CreateInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0,0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0,12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0,40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	DX::CreateInputLayout(m_pd3dDevice,
		m_dxObj.m_pVSBlob->GetBufferSize(),
		m_dxObj.m_pVSBlob->GetBufferPointer(),
		layout, numElements,
		m_dxObj.m_pInputLayout.GetAddressOf());
	return hr;
}
HRESULT TShape::LoadVertexShader(T_STR szShaderName)
{
	HRESULT hr = S_OK;
	m_dxObj.m_pVertexShader.Attach(DX::LoadVertexShaderFile(m_pd3dDevice, szShaderName.c_str(),
		m_dxObj.m_pVSBlob.GetAddressOf()));
	return hr;
}
HRESULT TShape::LoadPixelShader(T_STR szShaderName)
{
	HRESULT hr = S_OK;
	m_dxObj.m_pPixelShader.Attach(DX::LoadPixelShaderFile(m_pd3dDevice, szShaderName.c_str()));
	return hr;
}
HRESULT TShape::LoadGeometryShader(T_STR szShaderName)
{
	HRESULT hr = S_OK;
	m_dxObj.m_pGeometryShader.Attach(DX::LoadGSShaderFile(m_pd3dDevice, szShaderName.c_str()));
	return hr;
}
HRESULT TShape::LoadTexture(T_STR szTextureName)
{
	HRESULT hr = S_OK;
	D3DX11_IMAGE_LOAD_INFO loadinfo;
	ZeroMemory(&loadinfo, sizeof(loadinfo));

	loadinfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadinfo.Format = DXGI_FORMAT_FROM_FILE;

	hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, szTextureName.c_str(), 
										&loadinfo, NULL, m_dxObj.m_pTextureRV.GetAddressOf(), NULL);

	return hr;
}

bool TShape::Init()
{
	return true;
}
bool TShape::Frame()
{
	return true;
}
bool TShape::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_dxObj.m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	m_dxObj.PreRender(pContext, m_iNumVertexSize);
	return true;
}

void TShape::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}

	m_vPosition.x = m_matWorld._41;
	m_vPosition.y = m_matWorld._42;
	m_vPosition.z = m_matWorld._43;

	m_vLook.x = m_matWorld._11;
	m_vLook.y = m_matWorld._12;
	m_vLook.z = m_matWorld._13;

	m_vSide.x = m_matWorld._21;
	m_vSide.y = m_matWorld._22;
	m_vSide.z = m_matWorld._23;

	m_vUp.x = m_matWorld._31;
	m_vUp.y = m_matWorld._32;
	m_vUp.z = m_matWorld._33;

	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);
}
bool TShape::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}
bool TShape::PostRender(ID3D11DeviceContext* pContext)
{
	m_dxObj.PostRender(pContext, m_iNumVertexSize, m_iNumIndex);
	return true;
}
bool TShape::Release()
{
	return true;
}


TShape::TShape()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}


TShape::~TShape()
{
}

//////////////////////////////////////////////////TBoxShape//////////////////////////////////////////////
HRESULT TBoxShape::CreateVertexData()
{
	HRESULT hr = S_OK;
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	// 앞면
	m_VertexList.resize(24);
	m_VertexList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	// 뒷면
	m_VertexList[4] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 오른쪽
	m_VertexList[8] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 왼쪽
	m_VertexList[12] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 윗면
	m_VertexList[16] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// 아랫면
	m_VertexList[20] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	m_iNumVertexSize = sizeof(PNCT_VERTEX);
	m_iNumVertex = m_VertexList.size();
	
	return hr;
}
HRESULT TBoxShape::CreateIndexData()
{
	HRESULT hr = S_OK;
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

	m_iNumIndex = m_IndexList.size();
	m_iNumIndexSize = sizeof(DWORD);
	return hr;
}

TBoxShape::TBoxShape()
{
	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}


TBoxShape::~TBoxShape()
{
}