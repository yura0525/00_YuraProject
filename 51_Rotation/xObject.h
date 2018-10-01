#pragma once
#include "xStd.h"
#include "xTexManager.h"
#include "xShaderManager.h"

struct PCT_VERTEX //pnct
{
	float x;
	float y;
	float z;
	float r, g, b, a;
	float u;
	float v;
};
//콘스턴트 버퍼, 상수버퍼. 셰이더내에서 글로벌 변수와 비슷하게 쓰인다.
//float4개 단위로 보내야한다. 안쓰더라도 float 4개단위로 보내야한다.
//fTime[0]이거 하나만 쓰더라도 float fTime[4];이렇게 선언해야한다.
struct VS_CB
{
	float r, g, b, a;
	float fTime[4];
};

class xObject
{

public:
	VS_CB				m_constantData;

	ID3D11Buffer*		m_pVertextBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	ID3D11Buffer*		m_pConstantBuffer;

	xShader*			m_pShader;
	ID3D11InputLayout*	m_pVertexLayout;

	std::vector<PCT_VERTEX>		m_verList;
	std::vector<DWORD>			m_indexList;

	//ID3D11ShaderResourceView*	m_pTexSRV;

	//texture info
	UINT						m_iTexIndex;
	xTexture*					m_pTexture;
	ID3DBlob*					m_pBlobVS;
	//ID3D11SamplerState*			m_pSamplerState;
	ID3D11BlendState*			m_pAlphaBlend;
public:
	virtual HRESULT CreateVertextBuffer(ID3D11Device *	pd3dDevice);
	virtual HRESULT CreateIndexBuffer(ID3D11Device *	pd3dDevice);
	virtual HRESULT CreateConstantBuffer(ID3D11Device *	pd3dDevice);

	virtual HRESULT CreateShader(ID3D11Device *	pd3dDevice, T_STR szShaderName);
	virtual HRESULT CreateTexture(ID3D11Device * pd3dDevice, T_STR szShaderName);
	virtual HRESULT CreateInputLayout(ID3D11Device *	pd3dDevice);
	virtual HRESULT SetBlendState(ID3D11Device * pd3dDevice);
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender(ID3D11DeviceContext* pContext);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Release();
	virtual bool Create(ID3D11Device *	pd3dDevice, T_STR szShaderName, T_STR szTexName);
public:
	xObject();
	virtual ~xObject();
};

