#pragma once
#include "TObjectStd.h"


class TShape
{
public:
	ID3D11Device *				m_pd3dDevice;
	
	TDxObj				m_dxObj;

	vector<PNCT_VERTEX> m_VertexList;
	vector<DWORD>		m_IndexList;
	T_CB_DATA			m_cbData;
	
	UINT				m_iNumVertex;
	UINT				m_iNumIndex;
	UINT				m_iNumVertexSize;
	UINT				m_iNumIndexSize;
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
