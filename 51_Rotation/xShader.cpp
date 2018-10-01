#include "xShader.h"

bool xShader::Load(ID3D11Device * pd3dDevice, T_STR szFileName, C_STR vs, C_STR ps)
{
	m_pd3dDevice = pd3dDevice;
	HRESULT hr = S_OK;

	ID3DBlob* pErrorMsgs = NULL;
	//L"vertexshader.txt" => 셰이더파일이름, "VS" => VertexShader함수이름(), "vs_5_0"=> 컴파일러 
	DWORD dwFlags = D3DCOMPILE_DEBUG;
	if (FAILED(hr = D3DX11CompileFromFile(szFileName.c_str(), NULL, NULL,
		vs.c_str(), "vs_5_0", dwFlags, NULL, NULL, &m_pBlobVS, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
		return false;
	}

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreateVertexShader(m_pBlobVS->GetBufferPointer(), m_pBlobVS->GetBufferSize(), NULL, &m_pVS));

	//L"vertexshader.txt" => 셰이더파일이름, "PS" => PixelShader함수이름(), "ps_5_0"=> 컴파일러 
	ID3DBlob* pPSBuf = NULL;
	if (FAILED(hr = D3DX11CompileFromFile(szFileName.c_str(), NULL, NULL,
		ps.c_str(), "ps_5_0", dwFlags, NULL, NULL, &pPSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
		return false;
	}

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS));

	pPSBuf->Release();
	
	return true;
}

bool xShader::Init()			//초기화
{
	return true;
}
bool xShader::Frame()			//계산
{
	return true;
}
bool xShader::Render()			//드로우
{
	return true;
}
bool xShader::Release()			//소멸, 삭제
{
	SAFE_RELEASE(m_pTexSRV);
	//SAFE_RELEASE(m_pSamplerState);
	SAFE_RELEASE(m_pBlobVS);
	return true;
}

xShader::xShader()
{
}


xShader::~xShader()
{
}
