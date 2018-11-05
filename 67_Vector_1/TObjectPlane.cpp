#include "TObjectPlane.h"

bool TObjectPlane::Init()
{
	return true;
}
bool TObjectPlane::Set(ID3D11Device* pd3dDevice)
{
	float Radian = DtoR(180.0);
	float degree = RtoD(TBASIS_PI);

	TVector3 v(0.0f, 0.0f, 0.0f);
	TVector3 v2(0.0f, 1.0f, 0.0f);
	TVector3 v4(1.0f, 0.0f, 0.0f);

	float fDot = v2 | v4;
	TVector3 vCross = v2 ^ v4;

	TVector3 v0 = v4 * -1.0f;
	TVector3 v1 = (v0 + v2).Normalize();
	TVector3 v3 = (v2 - v0).Normalize();

	m_VertexList[0].p = v;//TVector3(-1.0f, 1.0f, 0.5f);
	m_VertexList[0].c = TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[1].p = v0;// TVector3(1.0f, 1.0f, 0.5f);
	m_VertexList[1].c = TVector4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[2].p = v1;//TVector3(1.0f, -1.0f, 0.5f);
	m_VertexList[2].c = TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[3].p = v2;//TVector3(-1.0f, -1.0f, 0.5f);
	m_VertexList[3].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[4].p = v3;//TVector3(-1.0f, -1.0f, 0.5f);
	m_VertexList[4].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[5].p = v4;//TVector3(-1.0f, -1.0f, 0.5f);
	m_VertexList[5].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);


	m_IndexList[0] = 0;	m_IndexList[1] = 1;	m_IndexList[2] = 2;
	m_IndexList[3] = 0;	m_IndexList[4] = 2;	m_IndexList[5] = 3;
	m_IndexList[6] = 0;	m_IndexList[7] = 3;	m_IndexList[8] = 3;
	m_IndexList[9] = 0;	m_IndexList[8] = 4;	m_IndexList[9] = 3;

	m_iNumVertex = 6;
	m_iNumIndex = 12;

	DX::CreateVertexBuffer(pd3dDevice, 
		m_iNumVertex,
		sizeof(PC_VERTEX),
		m_VertexList,
		&m_dxObj.m_pVertexBuffer);
	DX::CreateIndexBuffer(pd3dDevice,
		m_iNumIndex,
		sizeof(DWORD),
		m_IndexList,
		&m_dxObj.m_pIndexBuffer);

	DX::CreateConstantBuffer(pd3dDevice,
		1,
		sizeof(T_CB_DATA),
		&m_dxObj.m_pConstantBuffer,
		NULL, 
		true);

	m_dxObj.m_pVertexShader = DX::LoadVertexShaderFile(pd3dDevice, L"vs.vsh",
		&m_dxObj.m_pVSBlob);

	m_dxObj.m_pPixelShader = DX::LoadPixelShaderFile(pd3dDevice, L"ps.psh");

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	DX::CreateInputLayout(pd3dDevice,
		m_dxObj.m_pVSBlob->GetBufferSize(),
		m_dxObj.m_pVSBlob->GetBufferPointer(),
		layout, numElements,
		&m_dxObj.m_pInputLayout);
	return true;
}
bool TObjectPlane::Frame()
{
	return true;
}

bool TObjectPlane::Render(ID3D11DeviceContext* pContext)
{
	return m_dxObj.Render(pContext, sizeof(PC_VERTEX), m_iNumIndex);
}
bool TObjectPlane::Release()
{
	return true;
}

TObjectPlane::TObjectPlane()
{
	m_iNumIndex = 0;
	m_iNumVertex = 0;
}


TObjectPlane::~TObjectPlane()
{
}
