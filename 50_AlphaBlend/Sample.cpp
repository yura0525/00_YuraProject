#include "xCore.h"
#include "D3Dcompiler.h"		//D3DCOMPILE_DEBUG
#include <math.h>

#define CPU
struct P3VERTEX //pnct
{
	float x;
	float y;
	float z;
	float r, g, b, a;
	float u;
	float v;
};

struct VS_CB
{
	float r, g, b, a;
	float fTime[4];
};

class Sample : public xCore
{
public:
	VS_CB				m_constantData;

	ID3D11Buffer*		m_pVertextBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	ID3D11Buffer*		m_pConstantBuffer;

	ID3D11VertexShader*	m_pVS;
	ID3D11PixelShader*	m_pPS;
	ID3D11PixelShader*	m_pPS2;
	ID3D11PixelShader*	m_pPS3;

	ID3D11InputLayout*	m_pVertexLayout;

	std::vector<P3VERTEX>		m_verList;
	std::vector<DWORD>			m_indexList;
	
	ID3D11ShaderResourceView*	m_pTexSRV;
	ID3D11ShaderResourceView*	m_pTexSRVAlpha;
	ID3D11ShaderResourceView*	m_pTexSRVNoAlpha;
	
	ID3D11SamplerState*			m_pSamplerState;

	ID3D11BlendState*			m_pAlphaBlend;
	ID3D11BlendState*			m_pAlphaBlend2;
	ID3D11BlendState*			m_pAlphaBlend3;

public:
	HRESULT CreateVertextBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();

	HRESULT LoadShaderAndInputLayout();
	HRESULT CreatePixelShader();

	HRESULT LoadTextureFile(ID3D11ShaderResourceView**	ppTexSRV, const TCHAR* szFileName);
	HRESULT SetBlendState();

public:
	bool Init()
	{
		xCore::Init();

		if (FAILED(SetBlendState()))
		{
			return false;
		}

		m_constantData.r = cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.g = sinf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.a = 1;
		m_constantData.fTime[0] = g_fGameTimer;

		LoadTextureFile(&m_pTexSRV, L"../../data/Particle4.dds");
		LoadTextureFile(&m_pTexSRVAlpha, L"../../data/hud.dds");
		LoadTextureFile(&m_pTexSRVNoAlpha, L"../../data/Dirt_Diff.dds");

		CreateVertextBuffer();
		CreateIndexBuffer();
		CreateConstantBuffer();

		if (FAILED(LoadShaderAndInputLayout()))
		{
			return false;
		}

		if (FAILED(CreatePixelShader()))
		{
			return false;
		}

		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		//static float fAngle = 0.0f;
		//fAngle += g_fSecPerFrame;
#ifdef GPU
		//gpu update
		m_constantData.r = cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.g = sinf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.a = 1;
		m_constantData.fTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.fTime[1] = 1.0f;
		m_constantData.fTime[2] = 1.0f;
		//m_constantData.fTime[3] = fAngle;
		m_pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_constantData, 0, 0);
#elif defined CPU
		//cpu update
		//MAP			->�ڹ��� ����
		//~~			-->������۰���
		//UNMAP			->�ڹ��� �ݰ�
		D3D11_MAPPED_SUBRESOURCE address;
		HRESULT hr;
		if (SUCCEEDED(hr = m_pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
		{
			VS_CB* cb = (VS_CB*)address.pData;
			cb->r = cosf(g_fGameTimer) * 0.5f + 0.5f;
			cb->g = sinf(g_fGameTimer) * 0.5f + 0.5f;
			cb->b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
			cb->a = 1.0f;
			cb->fTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
			cb->fTime[1] = 2.0f;
			cb->fTime[2] = 2.0f;
			//cb->fTime[3] = fAngle;
			m_pContext->Unmap(m_pConstantBuffer, 0);
		}
#endif
		return true;
	}
	bool Render()
	{
		xCore::Render();

		m_pContext->VSSetShader(m_pVS, NULL, 0);
		m_pContext->HSSetShader(NULL, NULL, 0);
		m_pContext->DSSetShader(NULL, NULL, 0);
		m_pContext->GSSetShader(NULL, NULL, 0);
		m_pContext->PSSetSamplers(0, 1, &m_pSamplerState);
		
		m_pContext->OMSetBlendState(m_pAlphaBlend, 0, -1);
		m_pContext->PSSetShaderResources(0, 1, &m_pTexSRV);
		m_pContext->PSSetShader(m_pPS, NULL, 0);


		UINT offset = 0;
		UINT stride = sizeof(P3VERTEX);
		m_pContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);
		m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//������� ����(�ȼ����̴�)
		m_pContext->PSSetConstantBuffers(0,	//0 ���Թ�ȣ(�������� ��ȣ)
			1,								//1 ������� 1��
			&m_pConstantBuffer);

		//������� ����(���ؽ����̴�)
		m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pContext->IASetInputLayout(m_pVertexLayout);

		//m_pContext->Draw(6, 0);
		//pass Rendering
		//m_pContext->DrawIndexed(m_indexList.size(), 0, 0);

		m_pContext->PSSetShader(m_pPS2, NULL, 0);
		m_pContext->OMSetBlendState(m_pAlphaBlend2, 0, -1);
		m_pContext->PSSetShaderResources(0, 1, &m_pTexSRVNoAlpha);
		m_pContext->DrawIndexed(m_indexList.size(), 0, 0);

		m_pContext->PSSetShader(m_pPS3, NULL, 0);
		m_pContext->OMSetBlendState(m_pAlphaBlend3, 0, -1);
		m_pContext->PSSetShaderResources(0, 1, &m_pTexSRVAlpha);
		m_pContext->DrawIndexed(m_indexList.size(), 0, 0);

		return true;
	}
	bool Release()
	{
		xCore::Release();

		if (m_pVertextBuffer != NULL)	m_pVertextBuffer->Release();
		if (m_pIndexBuffer != NULL)		m_pIndexBuffer->Release();
		if (m_pConstantBuffer != NULL)	m_pConstantBuffer->Release();
		if (m_pVS != NULL)				m_pVS->Release();
		if (m_pPS != NULL)				m_pPS->Release();
		if (m_pVertexLayout != NULL)	m_pVertexLayout->Release();

		m_pVertextBuffer = NULL;
		m_pIndexBuffer = NULL;
		m_pConstantBuffer = NULL;
		m_pVS = NULL;
		m_pPS = NULL;
		m_pVertexLayout = NULL;
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

HRESULT Sample::CreateVertextBuffer()
{
	HRESULT hr;
	P3VERTEX vList[6];

	m_verList.resize(4);
	//������ ������� : �ð����
	//�ݽð�������� �׸��� �ȳ��´�.
	m_verList[0].x = -0.5f; m_verList[0].y = 0.5f; m_verList[0].z = 0.5f;
	m_verList[0].u = 0.0f; m_verList[0].v = 0.0f;
	m_verList[0].r = 1.0f; m_verList[0].g = 0.0f; m_verList[0].b = 0.0f;

	m_verList[1].x = 0.5f; m_verList[1].y = 0.5f; m_verList[1].z = 0.5f;
	m_verList[1].u = 1.0f; m_verList[1].v = 0.0f;
	m_verList[1].r = 0.0f; m_verList[1].g = 1.0f; m_verList[1].b = 0.0f;

	m_verList[2].x = -0.5f; m_verList[2].y = -0.5f; m_verList[2].z = 0.5f;
	m_verList[2].u = 0.0f; m_verList[2].v = 1.0f;
	m_verList[2].r = 0.0f; m_verList[2].g = 0.0f; m_verList[2].b = 1.0f;

	/*vList[3].x = -0.5f; vList[3].y = -0.5f; vList[3].z = 0.5f;
	vList[4].x = 0.5f; vList[4].y = 0.5f; vList[4].z = 0.5f;*/
	m_verList[3].x = 0.5f; m_verList[3].y = -0.5f; m_verList[3].z = 0.5f;
	m_verList[3].u = 1.0f; m_verList[3].v = 1.0f;
	m_verList[3].r = 1.0f; m_verList[3].g = 1.0f; m_verList[3].b = 1.0f;

	//GPU�� �޸𸮸� �Ҵ���.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_verList.size() * sizeof(P3VERTEX);		//36����Ʈ
	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU�� �޸𸮸� �Ҵ��ض�. �⺻�� GPU�޸�. GPU�� READ/WRITE ����.CPU�� ���ٺҰ����ϴ�.
												//D3D11_USAGE_STAGING���� CPU�� ���ٰ����ϴ�. ������ ������.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//��������

												//GPU�� �޸𸮸� �����ϰ� �Ҵ���.
												//hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pVertextBuffer);
	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(InitialData));
	InitialData.pSysMem = &(m_verList.at(0));

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pVertextBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT Sample::CreateIndexBuffer()
{
	HRESULT hr;
	m_indexList.push_back(0);
	m_indexList.push_back(1);
	m_indexList.push_back(2);
	m_indexList.push_back(2);
	m_indexList.push_back(1);
	m_indexList.push_back(3);

	/*DWORD indices[] =
	{
	0,1,2,
	2,1,3,
	};*/

	//GPU�� �޸𸮸� �Ҵ���.
	int iNumCount = sizeof(m_indexList) / sizeof(m_indexList[0]);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_indexList.size() * sizeof(DWORD);

	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU�� �޸𸮸� �Ҵ��ض�. �⺻�� GPU�޸�. GPU�� READ/WRITE ����.CPU�� ���ٺҰ����ϴ�.
												//D3D11_USAGE_STAGING���� CPU�� ���ٰ����ϴ�. ������ ������.
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//�ε�������

												//GPU�� �޸𸮸� �����ϰ� �Ҵ���.
												//hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pIndexBuffer)))
	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(InitialData));
	InitialData.pSysMem = &(m_indexList.at(0));

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pIndexBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT Sample::CreateConstantBuffer()
{
	HRESULT hr;
	//GPU�� �޸𸮸� �Ҵ���.

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VS_CB);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�������
												//GPU�� �޸𸮸� �����ϰ� �Ҵ���.

												//Usage
												//D3D11_USAGE_DEFAULT = 0,						 // gpu w/r����. CPU ���� �Ұ���.
												//D3D11_USAGE_IMMUTABLE = 1,					 // gpu r ����
												//D3D11_USAGE_DYNAMIC = 2,						 // �߰��� ���氡��. CPUAccessFlags ���� �����ؾ��Ѵ�.
												//D3D11_USAGE_STAGING = 3						 // cpu w/r ����
#ifdef CPU
	bd.Usage = D3D11_USAGE_DYNAMIC;				//D3D11_USAGE_DYNAMIC : GPU�� READ/WRITE ����. cpu Read X/ Write 0.
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//D3D11_USAGE_DYNAMIC�̸� CPUAccessFlags ���� �����ؾ��Ѵ�.
#elif defined GPU
	bd.Usage = D3D11_USAGE_DEFAULT;
#endif

	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(InitialData));
	InitialData.pSysMem = &(m_constantData);

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pConstantBuffer)))
	{
		return hr;
	}
	return hr;
}
HRESULT Sample::LoadShaderAndInputLayout()
{
	HRESULT hr = S_OK;

	ID3DBlob* pVSBuf = NULL;
	ID3DBlob* pErrorMsgs = NULL;
	//VertextShader �Լ��̸�. vs_5_0 �����Ϸ�.
	DWORD dwFlags = D3DCOMPILE_DEBUG;
	if (FAILED(hr = D3DX11CompileFromFile(L"vertextshader.txt", NULL, NULL,
		"VS", "vs_5_0", dwFlags, NULL, NULL, &pVSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
	}

	//���̴� �����ϵ� ���(������Ʈ����, ��������)
	V_RETURN(m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//5��° ���������� 0���� DXGI_FORMAT_R32G32B32_FLOAT��ŭ ������.
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//5��° ���������� 12����Ʈ���� DXGI_FORMAT_R32G32_FLOAT��ŭ ������.
	{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 28,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int iNum = sizeof(layout) / sizeof(layout[0]);

	//layout.SemanticName = "POSITION";				//�Լ��������� Ÿ��.
	//layout.SemanticIndex = 0;
	//layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;	//POSITION�� float3���̴�.
	//layout.InputSlot = 0;
	//layout.AlignedByteOffset = 0;
	//layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//layout.InstanceDataStepRate = 0;
	m_pd3dDevice->CreateInputLayout(layout, iNum, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout);
	pVSBuf->Release();
	return hr;
}

HRESULT Sample::CreatePixelShader()
{
	HRESULT hr = S_OK;

	//PixelShader �Լ��̸�. ps_5_0 �����Ϸ�.
	ID3DBlob* pPSBuf = NULL;
	ID3DBlob* pErrorMsgs = NULL;
	DWORD dwFlags = D3DCOMPILE_DEBUG;
	if (FAILED(hr = D3DX11CompileFromFile(L"vertextshader.txt", NULL, NULL,
		"PS", "ps_5_0", dwFlags, NULL, NULL, &pPSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
	}

	//���̴� �����ϵ� ���(������Ʈ����, ��������)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS));
	pPSBuf->Release();

	//2)
	//ID3DBlob* pPSBuf = NULL;
	//ID3DBlob* pErrorMsgs = NULL;
	//DWORD dwFlags = D3DCOMPILE_DEBUG;
	if (FAILED(hr = D3DX11CompileFromFile(L"vertextshader.txt", NULL, NULL,
		"PS2", "ps_5_0", dwFlags, NULL, NULL, &pPSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
	}

	//���̴� �����ϵ� ���(������Ʈ����, ��������)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS2));
	pPSBuf->Release();


	//3)
	//ID3DBlob* pPSBuf = NULL;
	//ID3DBlob* pErrorMsgs = NULL;
	//DWORD dwFlags = D3DCOMPILE_DEBUG;
	if (FAILED(hr = D3DX11CompileFromFile(L"vertextshader.txt", NULL, NULL,
		"PS3", "ps_5_0", dwFlags, NULL, NULL, &pPSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
	}

	//���̴� �����ϵ� ���(������Ʈ����, ��������)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS3));
	pPSBuf->Release();
	return hr;
}

HRESULT Sample::LoadTextureFile(ID3D11ShaderResourceView**	ppTexSRV, const TCHAR* szFileName)
{
	HRESULT hr = S_OK;

	//D3DX11_IMAGE_LOAD_INFO      pLoadInfo;
	//D3DX11_IMAGE_INFO			pSrcInfo;

	//ZeroMemory(&pLoadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	//pLoadInfo.Format = DXGI_FORMAT_FROM_FILE;
	//pLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//pLoadInfo.Width = D3DX11_DEFAULT;
	//pLoadInfo.Height = D3DX11_DEFAULT;
	//pLoadInfo.pSrcInfo = &pSrcInfo;
	//pLoadInfo.MipLevels = 1;

	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, szFileName, NULL, NULL, ppTexSRV, NULL)))
	{
		return hr;
	}

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	m_pd3dDevice->CreateSamplerState(&sd, &m_pSamplerState);
	return hr;
}

HRESULT Sample::SetBlendState()
{
	HRESULT hr = S_OK;
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.RenderTarget[0].BlendEnable = true;

	//FinalColor = destColor * DescBlend + srcColor * SrcBlend;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	//FinalAlpha = destAlpha * DescBlendAlpha + srcAlpha * SrcBlendAlpha;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_pd3dDevice->CreateBlendState(&bd, (ID3D11BlendState **)&m_pAlphaBlend);

	//2)
	bd.RenderTarget[0].BlendEnable = false;

	//FinalColor = destColor * DescBlend + srcColor * SrcBlend;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	//FinalAlpha = destAlpha * DescBlendAlpha + srcAlpha * SrcBlendAlpha;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pd3dDevice->CreateBlendState(&bd, (ID3D11BlendState **)&m_pAlphaBlend2);

	//3)
	bd.RenderTarget[0].BlendEnable = true;
	//FinalAlpha = destAlpha * DescBlendAlpha + srcAlpha * SrcBlendAlpha;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pd3dDevice->CreateBlendState(&bd, (ID3D11BlendState **)&m_pAlphaBlend3);
	return hr;
}

GAMERUN("AlphaBlend", 800, 600);