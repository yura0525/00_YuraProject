#include "TObjectPlane.h"

bool TObjectPlane::Init()
{
	m_cbData.fTime = 1.0f;
	return true;
}
bool TObjectPlane::Set(ID3D11Device* pd3dDevice)
{
	m_VertexList.resize(8);
	m_VertexList.push_back(PC_VERTEX({ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f) }));
	m_VertexList.push_back(PC_VERTEX({ D3DXVECTOR3(+1.0f, +1.0f, -1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f) }));
	m_VertexList.push_back(PC_VERTEX({ D3DXVECTOR3(+1.0f, -1.0f, -1.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f) }));
	m_VertexList.push_back(PC_VERTEX({ D3DXVECTOR3(-1.0f, -1.0f, +1.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f) }));
	m_VertexList.push_back(PC_VERTEX({ D3DXVECTOR3(-1.0f, +1.0f, +1.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f) }));
	m_VertexList.push_back(PC_VERTEX({ D3DXVECTOR3(+1.0f, +1.0f, +1.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f) }));
	m_VertexList.push_back(PC_VERTEX({ D3DXVECTOR3(+1.0f, -1.0f, +1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f) }));

	m_IndexList.resize(39);

	m_IndexList[0] = 0;		m_IndexList[1] = 1;		m_IndexList[2] = 2;
	m_IndexList[3] = 0;		m_IndexList[4] = 2;		m_IndexList[5] = 3;
	m_IndexList[6] = 4;		m_IndexList[7] = 5;		m_IndexList[8] = 1;
	m_IndexList[9] = 4;		m_IndexList[10] = 1;	m_IndexList[11] = 0;
	m_IndexList[12] = 4;	m_IndexList[13] = 6;	m_IndexList[14] = 5;
	m_IndexList[15] = 4;	m_IndexList[16] = 7;	m_IndexList[17] = 6;
	m_IndexList[18] = 3;	m_IndexList[19] = 2;	m_IndexList[20] = 6;
	m_IndexList[21] = 3;	m_IndexList[22] = 6;	m_IndexList[23] = 7;
	m_IndexList[24] = 1;	m_IndexList[25] = 5;	m_IndexList[26] = 6;
	m_IndexList[27] = 3;	m_IndexList[28] = 6;	m_IndexList[29] = 7;
	m_IndexList[30] = 1;	m_IndexList[31] = 6;	m_IndexList[32] = 2;
	m_IndexList[33] = 4;	m_IndexList[34] = 0;	m_IndexList[35] = 3;
	m_IndexList[36] = 4;	m_IndexList[37] = 3;	m_IndexList[38] = 7;

	m_iNumVertex = sizeof(m_VertexList) / sizeof(m_VertexList[0]);
	m_iNumIndex = sizeof(m_IndexList) / sizeof(m_IndexList[0]);

	DX::CreateVertexBuffer(pd3dDevice, 
		m_iNumVertex,
		sizeof(PC_VERTEX),
		&m_VertexList.at(0),
		m_dxObj.m_pVertexBuffer.GetAddressOf());
	DX::CreateIndexBuffer(pd3dDevice,
		m_iNumIndex,
		sizeof(DWORD),
		&m_IndexList.at(0),
		m_dxObj.m_pIndexBuffer.GetAddressOf());

	DX::CreateConstantBuffer(pd3dDevice,
		1,
		sizeof(T_CB_DATA),
		m_dxObj.m_pConstantBuffer.GetAddressOf(),
		NULL, 
		false);

	m_dxObj.m_pVertexShader.Attach(DX::LoadVertexShaderFile(pd3dDevice, L"vs.vsh",
									m_dxObj.m_pVSBlob.GetAddressOf()));

	m_dxObj.m_pPixelShader.Attach(DX::LoadPixelShaderFile(pd3dDevice, L"ps.psh"));
	//m_dxObj.m_pGeometryShader.Attach(DX::LoadGSShaderFile(pd3dDevice, L"gs.gsh"));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	DX::CreateInputLayout(pd3dDevice,
		m_dxObj.m_pVSBlob->GetBufferSize(),
		m_dxObj.m_pVSBlob->GetBufferPointer(),
		layout, numElements,
		m_dxObj.m_pInputLayout.GetAddressOf());
	return true;
}
bool TObjectPlane::Frame()
{
	m_cbData.fTime = (cosf(g_fGameTimer) * 0.5f) + 0.5f;
	return true;
}

bool TObjectPlane::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_dxObj.m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	return m_dxObj.Render(pContext, sizeof(PC_VERTEX), m_iNumIndex);
}
bool TObjectPlane::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	return PostRender(pContext);
}
bool TObjectPlane::PostRender(ID3D11DeviceContext* pContext)
{
	return m_dxObj.PostRender(pContext, sizeof(PC_VERTEX), m_iNumIndex);
}
bool TObjectPlane::Release()
{
	return m_dxObj.Release();
}

TObjectPlane::TObjectPlane()
{
	m_iNumIndex = 0;
	m_iNumVertex = 0;
}


TObjectPlane::~TObjectPlane()
{
}
