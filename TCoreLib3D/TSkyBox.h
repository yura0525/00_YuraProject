#pragma once
#include "TShape.h"
#define MAX_SKYBOX_TEXTURE 6
class TSkyBox : public TBoxShape
{
public:
	BOOL m_bRenerType;
	ComPtr<ID3D11ShaderResourceView> m_pTexSRV[MAX_SKYBOX_TEXTURE];
public:
	//HRESULT LoadPixelShader(T_STR szShaderName);
	bool Create(ID3D11Device *pDevice, T_STR szShaderName, T_STR szTextureName);
	HRESULT LoadTexture(T_STR szTextureName);
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	TSkyBox();
	virtual ~TSkyBox();
};

