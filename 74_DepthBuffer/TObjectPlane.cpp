#include "TObjectPlane.h"

bool TObjectPlane::Init()
{
	m_cbData.fTime = 1.0f;
	return true;
}
bool TObjectPlane::Set(ID3D11Device* pd3dDevice)
{
	m_VertexList[0].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VertexList[0].c = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[1].p = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VertexList[1].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[3].p = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_VertexList[3].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	D3DXVECTOR3 vDir0 = m_VertexList[1].p - m_VertexList[0].p;
	D3DXVECTOR3 vDir1 = m_VertexList[3].p - m_VertexList[0].p;

	//m_VertexList[2].p = (vDir0 + vDir1).Normal();
	D3DXVec3Normalize(&m_VertexList[2].p, &(vDir0 + vDir1));
	m_VertexList[2].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	
	//m_VertexList[5].p = (-vDir0).Normal();
	D3DXVec3Normalize(&m_VertexList[5].p, &(-vDir0 * 1.0f));

	m_VertexList[5].c = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);
	
	D3DXVec3Normalize(&m_VertexList[6].p, &(m_VertexList[2].p * -1.0f));
	m_VertexList[6].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);

	//m_VertexList[7].p = -vDir1.Normal();
	D3DXVec3Normalize(&m_VertexList[7].p, &(-vDir1 * 1.0f));
	m_VertexList[7].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vDir0 = m_VertexList[3].p - m_VertexList[0].p;
	vDir1 = m_VertexList[5].p - m_VertexList[0].p;

	//m_VertexList[4].p = (vDir0 + vDir1).Normal();
	D3DXVec3Normalize(&m_VertexList[4].p, &(vDir0 + vDir1));

	m_VertexList[4].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	D3DXVec3Normalize(&m_VertexList[8].p, &(m_VertexList[4].p * -1.0f));
	m_VertexList[8].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	//내적
	//직각이므로 0이다.
	//float fDot = D3DXVec3Dot(&m_VertexList[1].p, &m_VertexList[3].p);

	//외적
	//D3DXVECTOR3 vRet;
	//D3DXVec3Cross(&vRet, &m_VertexList[1].p, &m_VertexList[3].p);

	//float Radian = DtoR(180.0);
	//float degree = RtoD(TBASIS_PI);

	//D3DXVECTOR3 v(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 v2(0.0f, 1.0f, 0.0f);
	//D3DXVECTOR3 v4(1.0f, 0.0f, 0.0f);

	//float fDot = v2 | v4;
	//D3DXVECTOR3 vCross = v2 ^ v4;

	//D3DXVECTOR3 v0 = v4 * -1.0f;
	//D3DXVECTOR3 v1 = (v0 + v2).Normalize();
	//D3DXVECTOR3 v3 = (v2 + v4).Normalize();

	//v0.Normal();
	//v1.Normal();
	//v3.Normal();

	//m_VertexList[0].p = v;//D3DXVECTOR3(-1.0f, 1.0f, 0.5f);
	//m_VertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	//m_VertexList[1].p = v0;// D3DXVECTOR3(1.0f, 1.0f, 0.5f);
	//m_VertexList[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	//m_VertexList[2].p = v1;//D3DXVECTOR3(1.0f, -1.0f, 0.5f);
	//m_VertexList[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	//m_VertexList[3].p = v2;//D3DXVECTOR3(-1.0f, -1.0f, 0.5f);
	//m_VertexList[3].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_VertexList[4].p = v3;//D3DXVECTOR3(-1.0f, -1.0f, 0.5f);
	//m_VertexList[4].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_VertexList[5].p = v4;//D3DXVECTOR3(-1.0f, -1.0f, 0.5f);
	//m_VertexList[5].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);


	m_IndexList[0] = 0;		m_IndexList[1] = 1;		m_IndexList[2] = 2;
	m_IndexList[3] = 0;		m_IndexList[4] = 2;		m_IndexList[5] = 3;
	m_IndexList[6] = 0;		m_IndexList[7] = 3;		m_IndexList[8] = 4;
	m_IndexList[9] = 0;		m_IndexList[10] = 4;	m_IndexList[11] = 5;
	m_IndexList[12] = 0;	m_IndexList[13] = 5;	m_IndexList[14] = 6;
	m_IndexList[15] = 0;	m_IndexList[16] = 6;	m_IndexList[17] = 7;
	m_IndexList[18] = 0;	m_IndexList[19] = 7;	m_IndexList[20] = 8;
	m_IndexList[21] = 0;	m_IndexList[22] = 8;	m_IndexList[23] = 1;

	m_iNumVertex = sizeof(m_VertexList) / sizeof(m_VertexList[0]);
	m_iNumIndex = sizeof(m_IndexList) / sizeof(m_IndexList[0]);

	DX::CreateVertexBuffer(pd3dDevice, 
		m_iNumVertex,
		sizeof(PC_VERTEX),
		m_VertexList,
		m_dxObj.m_pVertexBuffer.GetAddressOf());
	DX::CreateIndexBuffer(pd3dDevice,
		m_iNumIndex,
		sizeof(DWORD),
		m_IndexList,
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

bool TObjectPlane::Render(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_dxObj.m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
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
