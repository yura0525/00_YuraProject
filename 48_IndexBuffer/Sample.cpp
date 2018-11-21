#include "xCore.h"
#include "D3Dcompiler.h"		//D3DCOMPILE_DEBUG
#include <math.h>

#define GPU
struct P3VERTEX
{
	float x;
	float y;
	float z;
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
	ID3D11InputLayout*	m_pVertexLayout;

	std::vector<P3VERTEX>	m_verList;
	std::vector<DWORD>		m_indexList;

public:
	HRESULT CreateVertextBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();

	HRESULT LoadShaderAndInputLayout();
	HRESULT CreatePixelShader();
public:
	bool Init()
	{
		m_constantData.r = cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.g = sinf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.a = 1;
		m_constantData.fTime[0] = g_fGameTimer;

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
		static float fAngle = 0.0f;
		fAngle += g_fSecPerFrame;
#ifdef GPU
		//gpu update
		m_constantData.r = cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.g = sinf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
		m_constantData.a = 1;
		m_constantData.fTime[0] = g_fGameTimer;
		m_constantData.fTime[1] = 0.5f;
		m_constantData.fTime[2] = 1.0f;
		m_constantData.fTime[3] = fAngle;
		m_pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_constantData, 0,0);
#elif defined CPU
		//cpu update
		//MAP			->자물쇠 열고
		//~~			-->상수버퍼갱신
		//UNMAP			->자물쇠 닫고
		D3D11_MAPPED_SUBRESOURCE address;
		HRESULT hr;
		if (SUCCEEDED(hr = m_pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
		{
			VS_CB* cb = (VS_CB*)address.pData;
			cb->r = cosf(g_fGameTimer) * 0.5f + 0.5f;
			cb->g = sinf(g_fGameTimer) * 0.5f + 0.5f;
			cb->b = 0.5f + cosf(g_fGameTimer) * 0.5f + 0.5f;
			cb->a = 1.0f;
			cb->fTime[0] = g_fGameTimer;
			cb->fTime[1] = 0.5f;
			cb->fTime[2] = 1.0f;
			cb->fTime[3] = fAngle;
			m_pContext->Unmap(m_pConstantBuffer, 0);
		}
#endif
		return true;
	}
	bool Render()
	{
		m_pContext->VSSetShader(m_pVS, NULL, 0);
		m_pContext->HSSetShader(NULL, NULL, 0);
		m_pContext->DSSetShader(NULL, NULL, 0);
		m_pContext->GSSetShader(NULL, NULL, 0);
		m_pContext->PSSetShader(m_pPS, NULL, 0);

		UINT offset = 0;
		UINT stride = sizeof(P3VERTEX);
		m_pContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);
		m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//상수버퍼 적용(픽셀셰이더)
		m_pContext->PSSetConstantBuffers(0,	//0 슬롯번호(레지스터 번호)
			1,								//1 상수버퍼 1개
			&m_pConstantBuffer);

		//상수버퍼 적용(버텍스셰이더)
		m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pContext->IASetInputLayout(m_pVertexLayout);
	
		//m_pContext->Draw(6, 0);
		m_pContext->DrawIndexed(m_indexList.size(), 0, 0);

		return true;
	}
	bool Release()
	{
		if( m_pVertextBuffer != NULL )	m_pVertextBuffer->Release();
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
	//정점의 저장순서 : 시계방향
	//반시계방향으로 그리면 안나온다.
	m_verList[0].x = -0.5f; m_verList[0].y = 0.5f; m_verList[0].z = 0.5f;
	m_verList[1].x = 0.5f; m_verList[1].y = 0.5f; m_verList[1].z = 0.5f;
	m_verList[2].x = -0.5f; m_verList[2].y = -0.5f; m_verList[2].z = 0.5f;
	/*vList[3].x = -0.5f; vList[3].y = -0.5f; vList[3].z = 0.5f;
	vList[4].x = 0.5f; vList[4].y = 0.5f; vList[4].z = 0.5f;*/
	m_verList[3].x = 0.5f; m_verList[3].y = -0.5f; m_verList[3].z = 0.5f;

	//GPU상에 메모리를 할당함.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_verList.size() * sizeof(P3VERTEX);		//36바이트
	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU에 메모리를 할당해라. 기본이 GPU메모리. GPU는 READ/WRITE 가능.CPU는 접근불가능하다.
												//D3D11_USAGE_STAGING만이 CPU가 접근가능하다. 단점은 느리다.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//정점버퍼

												//GPU상에 메모리를 셋팅하고 할당함.
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
	
	//GPU상에 메모리를 할당함.
	int iNumCount = sizeof(m_indexList) / sizeof(m_indexList[0]);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_indexList.size() * sizeof(DWORD);

	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU에 메모리를 할당해라. 기본이 GPU메모리. GPU는 READ/WRITE 가능.CPU는 접근불가능하다.
												//D3D11_USAGE_STAGING만이 CPU가 접근가능하다. 단점은 느리다.
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//인덱스버퍼

												//GPU상에 메모리를 셋팅하고 할당함.
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
	//GPU상에 메모리를 할당함.

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VS_CB);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//상수버퍼
												//GPU상에 메모리를 셋팅하고 할당함.

	//Usage
	//D3D11_USAGE_DEFAULT = 0,						 // gpu w/r가능. CPU 접근 불가능.
	//D3D11_USAGE_IMMUTABLE = 1,					 // gpu r 가능
	//D3D11_USAGE_DYNAMIC = 2,						 // 중간에 변경가능. CPUAccessFlags 값을 셋팅해야한다.
	//D3D11_USAGE_STAGING = 3						 // cpu w/r 가능
#ifdef CPU
	bd.Usage = D3D11_USAGE_DYNAMIC;				//D3D11_USAGE_DYNAMIC : GPU는 READ/WRITE 가능. cpu Read X/ Write 0.
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//D3D11_USAGE_DYNAMIC이면 CPUAccessFlags 값을 셋팅해야한다.
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
	//VertextShader 함수이름. vs_5_0 컴파일러.
	DWORD dwFlags = D3DCOMPILE_DEBUG;
	if (FAILED(hr = D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL,
		"VS", "vs_5_0", dwFlags, NULL, NULL, &pVSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
	}
	
	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int iNum = sizeof(layout) / sizeof(layout[0]);

	//layout.SemanticName = "POSITION";				//함수전달인자 타입.
	//layout.SemanticIndex = 0;
	//layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;	//POSITION은 float3개이다.
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

	//PixelShader 함수이름. ps_5_0 컴파일러.
	ID3DBlob* pPSBuf = NULL;
	ID3DBlob* pErrorMsgs = NULL;
	DWORD dwFlags = D3DCOMPILE_DEBUG;
	if (FAILED(hr = D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL,
		"PS", "ps_5_0", dwFlags, NULL, NULL, &pPSBuf, &pErrorMsgs, NULL)))
	{
		OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());
	}

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS));
	pPSBuf->Release();
	return hr;
}

GAMERUN("IndexBuffer", 800, 600);