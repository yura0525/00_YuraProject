#include "xObject.h"

#define CPU
bool xObject::Init()
{
	m_constantData.r = (rand() % 255) / 255.0f;
	m_constantData.g = (rand() % 255) / 255.0f;
	m_constantData.b = (rand() % 255) / 255.0f;
	m_constantData.a = 1;
	//m_constantData.fTime[0] = g_fGameTimer;

	return true;
}

bool xObject::Frame()
{
	return true;
}
bool xObject::PreRender(ID3D11DeviceContext* pContext)
{
	return true;
}
bool xObject::Render(ID3D11DeviceContext*	pContext)
{
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
	//MAP			->자물쇠 열고
	//~~			-->상수버퍼갱신
	//UNMAP			->자물쇠 닫고
	D3D11_MAPPED_SUBRESOURCE address;
	HRESULT hr;
	if (SUCCEEDED(hr = pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
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
		pContext->Unmap(m_pConstantBuffer, 0);
	}
#endif

	pContext->VSSetShader(m_pShader->m_pVS, NULL, 0);
	pContext->HSSetShader(NULL, NULL, 0);
	pContext->DSSetShader(NULL, NULL, 0);
	pContext->GSSetShader(NULL, NULL, 0);
	pContext->PSSetSamplers(0, 1, &m_pSamplerState);

	pContext->OMSetBlendState(m_pAlphaBlend, 0, -1);
	pContext->PSSetShaderResources(0, 1, &(m_pTexture->m_pTexSRV));
	pContext->PSSetShader(m_pShader->m_pPS, NULL, 0);


	UINT offset = 0;
	UINT stride = sizeof(PCT_VERTEX);
	pContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//상수버퍼 적용(픽셀셰이더)
	pContext->PSSetConstantBuffers(0,	//0 슬롯번호(레지스터 번호)
		1,								//1 상수버퍼 1개
		&m_pConstantBuffer);

	//상수버퍼 적용(버텍스셰이더)
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IASetInputLayout(m_pVertexLayout);

	//m_pContext->Draw(6, 0);
	//multi pass Rendering
	pContext->DrawIndexed(m_indexList.size(), 0, 0);

	return true;
}
bool xObject::PostRender(ID3D11DeviceContext* pContext)
{
	return true;
}
bool xObject::Release()
{
	if (m_pVertextBuffer != NULL)	m_pVertextBuffer->Release();
	if (m_pIndexBuffer != NULL)		m_pIndexBuffer->Release();
	if (m_pConstantBuffer != NULL)	m_pConstantBuffer->Release();
	if (m_pVertexLayout != NULL)	m_pVertexLayout->Release();

	m_pVertextBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pConstantBuffer = NULL;
	m_pVertexLayout = NULL;

	return true;
}

bool xObject::Create(ID3D11Device *	pd3dDevice, T_STR szShaderName, T_STR szTexName)
{
	CreateShader(pd3dDevice, szShaderName);
	CreateInputLayout(pd3dDevice);
	CreateTexture(pd3dDevice, szTexName);
	CreateConstantBuffer(pd3dDevice);
	CreateVertextBuffer(pd3dDevice);
	CreateIndexBuffer(pd3dDevice);

	SetBlendState(pd3dDevice);
	return true;
}
HRESULT xObject::CreateVertextBuffer(ID3D11Device *	pd3dDevice)
{
	HRESULT hr;
	PCT_VERTEX vList[6];

	m_verList.resize(4);
	//정점의 저장순서 : 시계방향
	//반시계방향으로 그리면 안나온다.
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

	//GPU상에 메모리를 할당함.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_verList.size() * sizeof(PCT_VERTEX);		//36바이트
	bd.Usage = D3D11_USAGE_DEFAULT;				//GPU에 메모리를 할당해라. 기본이 GPU메모리. GPU는 READ/WRITE 가능.CPU는 접근불가능하다.
												//D3D11_USAGE_STAGING만이 CPU가 접근가능하다. 단점은 느리다.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//정점버퍼

												//GPU상에 메모리를 셋팅하고 할당함.
												//hr = m_pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pVertextBuffer);
	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(InitialData));
	InitialData.pSysMem = &(m_verList.at(0));

	if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pVertextBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT xObject::CreateIndexBuffer(ID3D11Device *	pd3dDevice)
{
	HRESULT hr;
	//m_indexList.push_back(0);
	//m_indexList.push_back(1);
	//m_indexList.push_back(2);
	//m_indexList.push_back(2);
	//m_indexList.push_back(1);
	//m_indexList.push_back(3);

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

	if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pIndexBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT xObject::CreateConstantBuffer(ID3D11Device * pd3dDevice)
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

	if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitialData, &m_pConstantBuffer)))
	{
		return hr;
	}
	return hr;
}

HRESULT xObject::CreateShader(ID3D11Device * pd3dDevice, T_STR szShaderName)
{
	int iIndex	= I_ShaderMgr.Load(pd3dDevice, szShaderName);
	m_pShader = I_ShaderMgr.GetPtr(iIndex);
	
	return S_OK;
}
HRESULT xObject::CreateTexture(ID3D11Device * pd3dDevice, T_STR szShaderName)
{
	m_iTexIndex = I_TextureMgr.Load(pd3dDevice, szShaderName);
	m_pTexture = I_TextureMgr.GetPtr(m_iTexIndex);
	return S_OK;
}
HRESULT xObject::CreateInputLayout(ID3D11Device *	pd3dDevice)
{
	HRESULT hr = S_OK;

	//InputLayout.
	//셰이더 함수의 선언. 전달인자 타입으로 POSITION을 쓰겠다.
	//float4 VS(in float3 pos : POSITION ) : SV_POSITION
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//5번째 전달인자인 0에서 DXGI_FORMAT_R32G32B32_FLOAT만큼 꺼낸다.
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//5번째 전달인자인 12바이트에서 DXGI_FORMAT_R32G32_FLOAT만큼 꺼낸다.
	{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//5번째 전달인자인 28바이트에서 DXGI_FORMAT_R32G32_FLOAT만큼 꺼낸다.
	{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 28,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int iNum = sizeof(layout) / sizeof(layout[0]);

	//layout.SemanticName = "POSITION";				//함수전달인자 타입.
	//layout.SemanticIndex = 0;
	//layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;	//POSITION은 float3개이다.
	//layout.InputSlot = 0;
	//layout.AlignedByteOffset = 0;
	//layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//layout.InstanceDataStepRate = 0;
	pd3dDevice->CreateInputLayout(layout, iNum, m_pShader->m_pBlobVS->GetBufferPointer(), m_pShader->m_pBlobVS->GetBufferSize(), &m_pVertexLayout);
	//m_pBlobVS->Release();
	return hr;
}

HRESULT xObject::SetBlendState(ID3D11Device * pd3dDevice)
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

	pd3dDevice->CreateBlendState(&bd, (ID3D11BlendState **)&m_pAlphaBlend);
	return hr;
}

xObject::xObject()
{
}


xObject::~xObject()
{
}
