#pragma once
#include "TVector.h"
#include "TDxObject.h"
#include "TObjectStd.h"

struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
public:
	PC_VERTEX() : p(0, 0, 0), c(0, 0, 0, 0)
	{
	}
	PC_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR4 _c) : p(_p.x, _p.y, _p.z) , c(_c.x, _c.y, _c.z, _c.w)
	{
	}
};

//struct T_CB_DATA
//{
//	D3DXMATRIX matWorld;
//	D3DXMATRIX matView;
//	D3DXMATRIX matProj;
//
//	float fTime;
//	float fActiveVP;
//	float z, w;
//	D3DXVECTOR4 vColor;
//};

class TObjectPlane
{
public:
	vector<PC_VERTEX>	m_VertexList;
	vector<DWORD>		m_IndexList;
	DX::TDxObj		m_dxObj;
	T_CB_DATA		m_cbData;


	UINT						m_iNumIndex;
	UINT						m_iNumVertex;

public:
	bool Init();
	bool Set(ID3D11Device* pd3dDevice);

	bool Frame();
	bool PreRender(ID3D11DeviceContext* pContext);
	bool Render(ID3D11DeviceContext* pContext);
	bool PostRender(ID3D11DeviceContext* pContext);
	bool Release();

public:
	TObjectPlane();
	virtual ~TObjectPlane();
};

