#pragma once
#include "TVector.h"
#include "TDxObject.h"

struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
};

struct T_CB_DATA
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	float fTime;
	float fActiveVP;
	float z, w;
	D3DXVECTOR4 vColor;
};

class TObjectPlane
{
public:
	PC_VERTEX		m_VertexList[9];
	DWORD			m_IndexList[24];
	DX::TDxObj		m_dxObj;
	T_CB_DATA		m_cbData;


	UINT						m_iNumIndex;
	UINT						m_iNumVertex;

public:
	bool Init();
	bool Set(ID3D11Device* pd3dDevice);

	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();

public:
	TObjectPlane();
	virtual ~TObjectPlane();
};

