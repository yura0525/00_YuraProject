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
//�ܽ���Ʈ ����, �������. ���̴������� �۷ι� ������ ����ϰ� ���δ�.
//float4�� ������ �������Ѵ�. �Ⱦ����� float 4�������� �������Ѵ�.
//fTime[0]�̰� �ϳ��� ������ float fTime[4];�̷��� �����ؾ��Ѵ�.
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

