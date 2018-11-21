#include "xCore.h"


struct P3VERTEX
{
	float x;
	float y;
	float z;
};

class Sample : public xCore
{
public:
	ID3D11Buffer *		m_pVertextBuffer;
	ID3D11VertexShader*	m_pVS;
	ID3D11PixelShader*	m_pPS;
	ID3D11InputLayout*	m_pVertexLayout;

	ID3D11RasterizerState*      m_pRSWireFrame;
	ID3D11RasterizerState*      m_pRSSolid;
public:
	HRESULT CreateVertextBuffer();
	HRESULT LoadShaderAndInputLayout();
	HRESULT CreatePixelShader();
public:
	bool Init()
	{
		SetRasterizerState();

		CreateVertextBuffer();
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
		return true;
	}
	bool Render()
	{
		//W키를 누르면 와이어 프레임이 보이게함.
		if (g_Input.bFront)
		{
			m_pContext->RSSetState(m_pRSWireFrame);
		}
		else
		{
			m_pContext->RSSetState(m_pRSSolid);
		}


		UINT offset = 0;
		UINT stride = sizeof(P3VERTEX);
		m_pContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);
		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pContext->IASetInputLayout(m_pVertexLayout);
		m_pContext->VSSetShader(m_pVS, NULL, 0);
		m_pContext->HSSetShader(NULL, NULL, 0);
		m_pContext->DSSetShader(NULL, NULL, 0);
		m_pContext->GSSetShader(NULL, NULL, 0);
		m_pContext->PSSetShader(m_pPS, NULL, 0);

		m_pContext->Draw(6, 0);
		return true;
	}
	bool Release()
	{
		if (m_pRSWireFrame != NULL)		m_pRSWireFrame->Release();
		if (m_pRSSolid != NULL)			m_pRSSolid->Release();

		m_pVS->Release();
		m_pPS->Release();
		m_pVertexLayout->Release();
		return true;
	}

	HRESULT SetRasterizerState(D3D11_FILL_MODE fill = D3D11_FILL_SOLID)
	{
		HRESULT hr = S_OK;
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
		desc.DepthClipEnable = TRUE;

		hr = m_pd3dDevice->CreateRasterizerState(&desc, &m_pRSWireFrame);

		desc.FillMode = D3D11_FILL_SOLID;
		hr = m_pd3dDevice->CreateRasterizerState(&desc, &m_pRSSolid);
		return hr;
	}
public:
	Sample() {}
	~Sample() {}
};

HRESULT Sample::CreateVertextBuffer()
{
	HRESULT hr;
	P3VERTEX vList[6];

	//정점의 저장순서 : 시계방향
	//반시계방향으로 그리면 안나온다.
	vList[0].x = -0.5f; vList[0].y = 0.5f; vList[0].z = 0.5f;
	vList[1].x = 0.5f; vList[1].y = 0.5f; vList[1].z = 0.5f;
	vList[2].x = -0.5f; vList[2].y = -0.5f; vList[2].z = 0.5f;
	vList[3].x = -0.5f; vList[3].y = -0.5f; vList[3].z = 0.5f;
	vList[4].x = 0.5f; vList[4].y = 0.5f; vList[4].z = 0.5f;
	vList[5].x = 0.5f; vList[5].y = -0.5f; vList[5].z = 0.5f;

	//GPU상에 메모리를 할당함.
	//hr = m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pVertextBuffer);
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = 6 * sizeof(P3VERTEX);		//36바이트
	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU에 메모리를 할당해라. 기본이 GPU메모리. GPU는 READ/WRITE 가능.CPU는 접근불가능하다.
												//D3D11_USAGE_STAGING만이 CPU가 접근가능하다. 단점은 느리다.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//정점버퍼
	
	//GPU상에 메모리를 셋팅하고 할당함.
	//hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pVertextBuffer);
	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(InitialData));
	InitialData.pSysMem = vList;

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pVertextBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT Sample::LoadShaderAndInputLayout()
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBuf = NULL;

	//L"vertexshader.txt" => 셰이더파일이름, "VS" => VertexShader함수이름(), "vs_5_0"=> 컴파일러 
	V_RETURN(D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL,
		"VS", "vs_5_0", 0, 0, NULL, &pVSBuf, NULL, NULL));

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS));

	//셰이더 함수의 선언. 전달인자 타입으로 POSITION을 쓰겠다.
	//float4 VS(in float3 pos : POSITION ) : SV_POSITION
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
	ID3DBlob* pPSBuf = NULL;
	//VertextShader 함수이름. vs_5_0 컴파일러.
	V_RETURN(D3DX11CompileFromFile(L"vertexshader.txt", NULL, NULL,
		"PS", "ps_5_0", 0, 0, NULL, &pPSBuf, NULL, NULL));

	//셰이더 컴파일된 결과(오브젝트파일, 목적파일)
	V_RETURN(m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS));
	pPSBuf->Release();

	return hr;
}
GAMERUN("VertexBuffer", 800, 600);