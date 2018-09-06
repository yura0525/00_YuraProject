#include "xCore.h"
#include "d3d11.h"

class Sample : public xCore
{
public:
	ID3D11Device *			m_pd3dDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;	//WINAPI�� ������۸��̶� ����.����۸� ����.
	
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
	//1)����̽� ����.
	UINT Flags = 0;

	//���̷�Ʈ�������� �������� ģ���ϰ� �˷��ش�. ����׽ÿ��� �˷��ִ°ɷ�.
#ifdef _DEBUG
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL pFeatureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};

	//pFeatureLevels�� �����̴�.
	UINT FeatureLevels = 2;

	//�ϵ����� �����ϴ°� �׷���ī�尡 �����ϴ°�.
	D3D_DRIVER_TYPE DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	//GPU�� ����
		D3D_DRIVER_TYPE_WARP,		//CPU�� ����
		D3D_DRIVER_TYPE_REFERENCE,	//CPU�� ����
	};

	UINT numDriverType = sizeof(DriverType) / sizeof(D3D_DRIVER_TYPE);
	//����� ����°�.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));

	sd.BufferDesc.Width = g_rtClient.right;
	sd.BufferDesc.Height = g_rtClient.bottom;

	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;			//60�ް� �츣�� 1/60
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//������� �뵵.
	sd.BufferCount = 1;									//������� ����.
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	sd.SampleDesc.Count = 1;							//�ѹ��� �Ѹ���. ������ �Ѹ��� ��Ƽ�ٸ����.

	for(int iMode = 0; iMode < numDriverType; iMode++)
	{
		//D3D_FEATURE_LEVEL_11_0�� ���� ���´�.
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

	// 2)����Ÿ�� �� ����
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&pBackBuffer);

	//������ ���õ� �޼ҵ�� m_pd3dDevice�� ����
	//������ ������ ����� m_pContext
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);

	//GetBuffer�� ���� ���ϴ� �۾��� �ѵڿ��� ������ �����ؾ��Ѵ�.Release();
	pBackBuffer->Release();
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	
	// 3)����Ʈ ����
	// ȭ�� ��ü�� ä����.
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = g_rtClient.right;
	vp.Height = g_rtClient.bottom;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1, &vp);			//RasterizerStage�� ����.
	return true;
}

bool Sample::PreRender()
{
	//RGB 255�� 0-1�ǰ����� ������. RGBA
	float color[4] = { 1, 0, 0, 1 };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, color);
	return true;
}

bool Sample::PostRender()
{
	//����ۿ��ִ°� �չ��۷� �ٲ۴�.
	m_pSwapChain->Present(0, 0);
	return true;
}

bool Sample::Render()
{
	

	////����, ĳ����, ����Ʈ, UI


	return true;
}
bool Sample::Release()
{
	//��ġ�� ��� �⺻������ �����ϰ� ��ġ�� ���鶧 ���·� �ǵ�����.
	if (m_pContext)				m_pContext->ClearState();

	//������ �������� Release�Ѵ�.
	if (m_pRenderTargetView)	m_pRenderTargetView->Release();
	if (m_pSwapChain)			m_pSwapChain->Release();
	if (m_pd3dDevice)			m_pd3dDevice->Release();
	if (m_pContext)				m_pContext->Release();
	
	return true;
}
GAMERUN("CreateDevice", 800, 600);