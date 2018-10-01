#pragma once
#include "xStd.h"

class xShader
{
public:
	ID3D11VertexShader*	m_pVS;
	ID3D11PixelShader*	m_pPS;
	ID3DBlob*			m_pBlobVS;
	T_STR				m_szName;

public:
	ID3D11Device *				m_pd3dDevice;				// 디바이스 객체
	ID3D11ShaderResourceView *	m_pTexSRV;
	//ID3D11SamplerState*			m_pSamplerState;

public:
	bool Load(ID3D11Device * pd3dDevice, T_STR szFileName, C_STR vs = "VS", C_STR ps = "PS");

public:
	bool Init();			//초기화
	bool Frame();			//계산
	bool Render();			//드로우
	bool Release();			//소멸, 삭제
public:
	xShader();
	virtual ~xShader();
};

