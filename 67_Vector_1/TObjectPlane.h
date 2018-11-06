#pragma once
#include "TVector.h"
#include "TDxObject.h"

struct PC_VERTEX
{
	TVector3 p;
	TVector4 c;

};

struct T_CB_DATA
{
	float fTime;
	float y, z, w;
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

