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

	hr = m_pd3dDevice->CreateDepthStencilView((ID3D11Resource*)pTex, &dsvd, &m_pDSV);

	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));

	dsd.DepthEnable = TRUE;								//Z버퍼를 활성화 여부.
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//출력되면 Z버퍼 기입.
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		//Z버퍼값이 작거나 같으면 뿌려줘라.

	hr = m_pd3dDevice->CreateDepthStencilState(&dsd, &m_pDSVStateEnableLess);

	dsd.DepthFunc = D3D11_COMPARISON_GREATER;
	hr = m_pd3dDevice->CreateDepthStencilState(&dsd, &m_pDSVStateEnable);
	return hr;
}

bool xCore::Init()
{
	xWindow::Init();
	m_Timer.Init();
	m_Font.Init();
	m_Input.Init();

	CreateDSV();

	DeleteDeviceResources(m_sd.BufferDesc.Width, m_sd.BufferDesc.Height);
	CreateDeviceResources(m_sd.BufferDesc.Width, m_sd.BufferDesc.Height);
	return true;
}
bool xCore::Frame()
{
	xWindow::Frame();
	m_Timer.Frame();
	m_Font.Frame();
	m_Input.Frame();
	return true;
}

bool xCore::PreRender()
{
	xWindow::PreRender();
	return true;
}
bool xCore::PostRender()
{
	return xWindow::PostRender();
}
bool xCore::Render()
{
	xWindow::Render();
	m_Timer.Render();
	m_Font.Render();
	m_Input.Render();
	return true;
}
bool xCore::Release()
{
	xWindow::Release();
	m_Timer.Release();
	m_Font.Release();
	m_Input.Release();

	SAFE_RELEASE(m_pDSV);
	SAFE_RELEASE(m_pDSVStateEnable);
	SAFE_RELEASE(m_pDSVStateEnableLess);
	return true;
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
	Init();

	return true;
}

bool xCore::GameFrame()
{
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
	return true;
}
bool xCore::GamePostRender()
{
	return true;
}

void xCore::DeleteDeviceResources(UINT iWidth, UINT iHeight)
{
	m_Font.DiscardDeviceResources();
}
HRESULT xCore::CreateDeviceResources(UINT iWidth, UINT iHeight)
{
	IDXGISurface1*		pBackBuffer;

	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pBackBuffer);
	hr = m_Font.CreateDeviceResources(pBackBuffer);
	
	if (pBackBuffer)	pBackBuffer->Release();
	
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
	return true;
}
xCore::xCore()
{
}


xCore::~xCore()
{
}
