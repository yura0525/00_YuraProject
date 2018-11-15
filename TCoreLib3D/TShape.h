#pragma once
#include "TObjectStd.h"


class TShape
{
public:
	ID3D11Device *				m_pd3dDevice;
	
	TDxObj				m_dxObj;

	vector<PNCT_VERTEX> m_VertexList;
	vector<DWORD>		m_IndexList;
	UINT				m_iNumVertexSize;
	T_CB_DATA			m_cbData;
	
	D3DXMATRIX			m_matWorld;
	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj;

	D3DXVECTOR3			m_vPosition;
	D3DXVECTOR3			m_vLook;
	D3DXVECTOR3			m_vSide;
	D3DXVECTOR3			m_vUp;

	UINT				m_iNumVertex;
	UINT				m_iNumIndex;
	
	UINT				m_iNumIndexSize;

	D3D11_PRIMITIVE_TOPOLOGY	m_Primitive;
public:
	void SetColor(D3DXVECTOR4 vColor);
public:
	bool Create(ID3D11Device *pDevice, T_STR szShaderName, T_STR szTextureName);

	virtual HRESULT CreateVertexData()
	{
		return S_OK;
	}
	virtual HRESULT CreateIndexData()
	{
		return S_OK;
	}
	virtual HRESULT CreateVertexBuffer();
	virtual HRESULT CreateIndexBuffer();
	virtual HRESULT CreateConstantBuffer();
	virtual HRESULT CreateInputLayout();
	virtual HRESULT LoadVertexShader(T_STR szShaderName);
	virtual HRESULT LoadPixelShader(T_STR szShaderName);
	virtual HRESULT LoadGeometryShader(T_STR szShaderName);
	virtual HRESULT LoadTexture(T_STR szTextureName);

	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender(ID3D11DeviceContext* pContext);
	virtual void SetMatrix(D3DXMATRIX* pWorld = NULL, D3DXMATRIX* pView = NULL, D3DXMATRIX* pProj = NULL);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Release();
	
public:
	TShape();
	virtual ~TShape();
};

class TBoxShape : public TShape
{
public:
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
public:
	TBoxShape();
	virtual ~TBoxShape();
};

class TPlaneShape : public TShape
{
public:
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
public:
	TPlaneShape();
	virtual ~TPlaneShape();
};


class TLineShape : public TShape
{
public:
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
	bool Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor = D3DXVECTOR4(1, 0, 0, 0));

	HRESULT LoadPixelShader(T_STR szShaderName);
public:
	TLineShape();
	virtual ~TLineShape();
};

class TDirLineShape : public TLineShape
{
public:
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
	bool Render(ID3D11DeviceContext* pContext);
public:
	TDirLineShape();
	virtual ~TDirLineShape();
};

