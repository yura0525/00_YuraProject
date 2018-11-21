#include "TDxState.h"
#include "xCore.h"

HRESULT xCore::CreateDSV()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* pTex = NULL;
	D3D11_TEXTURE2D_DESC td;

	//32비트 짜리 Mip맵이 없는 width, height 크기에 CPU 접근 못하는 Default의 텍스쳐 1장 생성. 
	td.Width = g_rtClient.right;
	td.Height = g_rtClient.bottom;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pTex);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	dsvd.Texture2D.MipSlice = 0;

	hr = m_pd3dDevice->CreateDepthStencilView(pTex, &dsvd, &m_pDSV);

	if (pTex) pTex->Release();
	return hr;
}

bool xCore::GamePreInit()
{
	HRESULT hr;

	TDevice::CreateDX11GIFactory();
	std::vector<TAdapter> m_pTAdapterList;		//그래픽 카드 정보와 갯수를 알수있다.

	for (int iAdapter = 0; ; iAdapter++)			//그래픽카드 찾는다.
	{
		IDXGIAdapter* pAdapter = NULL;
		TAdapter tAdapter;

		hr = m_pDXGIFactory->EnumAdapters(iAdapter, &pAdapter);

		if (FAILED(hr)) break;

		pAdapter->GetDesc(&tAdapter.desc);
		tAdapter.m_pAdapter = pAdapter;
		m_pTAdapterList.push_back(tAdapter);
	}

	for (int iAdapter = 0; iAdapter < m_pTAdapterList.size(); iAdapter++)
	{
		DXGI_MODE_DESC desc[512];

		for (int iOutput = 0; ; iOutput++)	//그래픽카드 별로 연결된 모니터를 찾는다.
		{
			TOutput Output;
			IDXGIOutput* pOutput = NULL;

			hr = m_pTAdapterList.at(iAdapter).m_pAdapter->EnumOutputs(iOutput, &pOutput);
			if (pOutput == NULL || FAILED(hr)) break;

			if (pOutput != NULL)
			{
				pOutput->GetDesc(&Output.desc);
				UINT NumModes = 512;

				hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING, &NumModes, desc);
			}

			Output.m_pOutputList.push_back(pOutput);

			m_pTAdapterList.at(iAdapter).m_OutputList.push_back(Output);

			for (int iMode = 0; iMode < 512; iMode++)
			{
				if (desc[iMode].Width >= 800 && desc[iMode].Width <= 1920 &&
					desc[iMode].Height >= 600 && desc[iMode].Height <= 1080)
				{
					m_pTAdapterList.at(iAdapter).m_OutputList[iOutput].m_pModeList.push_back(desc[iMode]);
				}
			}
		}
	}
	return true;
}

bool xCore::GameInit()
{
	GamePreInit();

	HRESULT hr;
	if (FAILED(hr = CreateDX11GIFactory()))
	{
		return false;
	}
	if (FAILED(hr = CreateDevice()))
	{
		return false;
	}
	if (FAILED(hr = CreateSwapChain()))
	{
		return false;
	}
	if (FAILED(hr = SetRenderTarget()))
	{
		return false;
	}
	SetViewPort();
	CreateDSV();

	TDxState::SetState(m_pd3dDevice);

	//Alt + Enter 키를 막는다.
	if (FAILED(hr = m_pDXGIFactory->MakeWindowAssociation(NULL, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	{
		return hr;
	}	

	m_Timer.Init();
	m_Font.Init();
	m_Input.Init();

	IDXGISurface* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pBackBuffer);
	m_Font.Set(m_hWnd, m_sd.BufferDesc.Width, m_sd.BufferDesc.Height, pBackBuffer);
	
	pBackBuffer->Release();

	/*
	DeleteDeviceResources(m_sd.BufferDesc.Width, m_sd.BufferDesc.Height);
	CreateDeviceResources(m_sd.BufferDesc.Width, m_sd.BufferDesc.Height);*/

	m_DefaultCamera.SetViewMatrix();
	m_DefaultCamera.SetProjMatrix(D3DX_PI * 0.5f, (float)m_rtClient.right / m_rtClient.bottom);
	m_pMainCamera = &m_DefaultCamera;

	Init();

	return true;
}

bool xCore::GameFrame()
{	
	m_Timer.Frame();
	m_Font.Frame();
	m_Input.Frame();

	Frame();
	return true;
}

bool xCore::GameRender()
{
	GamePreRender();
	{
		PreRender();
		Render();
		PostRender();
	}
	GamePostRender();
	return true;
}

bool xCore::GamePreRender()
{
	//RGB 255가 0-1의값으로 컨버팅. RGBA
	float color[4] = { 0.44f, 0.61f, 0.83f, 1 };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDSV);


	ApplyDDS(m_pContext, TDxState::g_pDSVStateEnableLessEqual);
	ApplyBS(m_pContext, TDxState::g_pBSAlphaBlend);
	ApplyRS(m_pContext, TDxState::g_pRSBackCullSolid);
	ApplySS(m_pContext, TDxState::g_pSSWrapLinear);

	m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}
bool xCore::GamePostRender()
{
	m_Timer.Render();
	m_Font.Render();
	m_Input.Render();

	//백버퍼에있는걸 앞버퍼로 바꾼다.
	m_pSwapChain->Present(0, 0);
	return true;
}

void xCore::DeleteDeviceResources(UINT iWidth, UINT iHeight)
{
	m_Font.DiscardDeviceResources();
	DeleteResources(iWidth, iHeight);
}

HRESULT xCore::CreateDeviceResources(UINT iWidth, UINT iHeight)
{
	IDXGISurface1*		pBackBuffer = NULL;

	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pBackBuffer);
	hr = m_Font.CreateDeviceResources(pBackBuffer);
	
	if (pBackBuffer)	pBackBuffer->Release();

	m_pMainCamera->UpdateProjMatrix(iWidth, iHeight);
	CreateResources(iWidth, iHeight);
	return hr;
}
bool xCore::GameRun()
{
	GameFrame();
	GameRender();

	return true;
}
bool xCore::GameRelease()
{
	Release();

	m_Timer.Release();
	m_Font.Release();
	m_Input.Release();
	TDxState::Release();

	m_pSwapChain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(m_pDSV);

	//생성한 역순으로 Release한다.
	if (m_pRenderTargetView)	m_pRenderTargetView->Release();
	if (m_pSwapChain)			m_pSwapChain->Release();
	if (m_pd3dDevice)			m_pd3dDevice->Release();
	if (m_pContext)				m_pContext->Release();
	if (m_pDXGIFactory)			m_pDXGIFactory->Release();

	m_pRenderTargetView = NULL;
	m_pSwapChain = NULL;
	m_pd3dDevice = NULL;
	m_pContext = NULL;
	m_pDXGIFactory = NULL;

	return true;
}
xCore::xCore()
{
	m_pMainCamera = NULL;
}


xCore::~xCore()
{
}
