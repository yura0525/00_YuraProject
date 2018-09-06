#include "xCore.h"
#include "d3d11.h"

class Sample : public xCore
{
public:
	ID3D11Device *			m_pd3dDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;	//WINAPI의 더블버퍼링이랑 같음.백버퍼를 만듬.
	
	ID3D11RenderTargetView*	m_pRenderTargetView;
public:
	bool Init();
	bool Render();
	bool PreRender();
	bool PostRender();
	bool Release();
public:
	Sample() {}
	~Sample() {}
};


bool Sample::Init()
{
	//1)디바이스 생성.
	UINT Flags = 0;

	//다이렉트엑스에서 오류날때 친절하게 알려준다. 디버그시에만 알려주는걸로.
#ifdef _DEBUG
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL pFeatureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};

	//pFeatureLevels의 갯수이다.
	UINT FeatureLevels = 2;

	//하드웨어에서 지원하는게 그래픽카드가 지원하는것.
	D3D_DRIVER_TYPE DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	//GPU가 연산
		D3D_DRIVER_TYPE_WARP,		//CPU가 연산
		D3D_DRIVER_TYPE_REFERENCE,	//CPU가 연산
	};

	UINT numDriverType = sizeof(DriverType) / sizeof(D3D_DRIVER_TYPE);
	//백버퍼 만드는것.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));

	sd.BufferDesc.Width = g_rtClient.right;
	sd.BufferDesc.Height = g_rtClient.bottom;

	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;			//60메가 헤르츠 1/60
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//백버퍼의 용도.
	sd.BufferCount = 1;									//백버퍼의 갯수.
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	sd.SampleDesc.Count = 1;							//한번만 뿌린다. 여러번 뿌리면 안티앨리어싱.

	for(int iMode = 0; iMode < numDriverType; iMode++)
	{
		//D3D_FEATURE_LEVEL_11_0의 값이 들어온다.
		D3D_FEATURE_LEVEL retFL;

		if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(NULL, DriverType[iMode], NULL, Flags,
			pFeatureLevels, FeatureLevels, D3D11_SDK_VERSION,
			&sd, &m_pSwapChain, &m_pd3dDevice, &retFL, &m_pContext)))
		{
			if (retFL != D3D_FEATURE_LEVEL_11_0)
			{
				Release();
				continue;
			}
			break;
		}
	}

	// 2)랜더타겟 뷰 생성
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&pBackBuffer);

	//생성과 관련된 메소드는 m_pd3dDevice와 관련
	//생성된 이후의 제어는 m_pContext
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);

	//GetBuffer을 쓰고 원하는 작업을 한뒤에는 무조건 삭제해야한다.Release();
	pBackBuffer->Release();
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	
	// 3)뷰포트 생성
	// 화면 전체를 채워라.
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = g_rtClient.right;
	vp.Height = g_rtClient.bottom;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1, &vp);			//RasterizerStage에 셋팅.
	return true;
}

bool Sample::PreRender()
{
	//RGB 255가 0-1의값으로 컨버팅. RGBA
	float color[4] = { 1, 0, 0, 1 };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, color);
	return true;
}

bool Sample::PostRender()
{
	//백버퍼에있는걸 앞버퍼로 바꾼다.
	m_pSwapChain->Present(0, 0);
	return true;
}

bool Sample::Render()
{
	

	////지형, 캐릭터, 이펙트, UI


	return true;
}
bool Sample::Release()
{
	//장치의 모든 기본설정을 복원하고 장치를 만들때 상태로 되돌린다.
	if (m_pContext)				m_pContext->ClearState();

	//생성한 역순으로 Release한다.
	if (m_pRenderTargetView)	m_pRenderTargetView->Release();
	if (m_pSwapChain)			m_pSwapChain->Release();
	if (m_pd3dDevice)			m_pd3dDevice->Release();
	if (m_pContext)				m_pContext->Release();
	
	return true;
}
GAMERUN("CreateDevice", 800, 600);