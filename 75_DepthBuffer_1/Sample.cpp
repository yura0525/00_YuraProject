#include "xCore.h"
#include "TObjectPlane.h"
#include "TDxState.h"
using namespace DX;

class Sample : public xCore
{
public:
	TObjectPlane	m_ObjBox;
	
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_matView;
	D3DXMATRIX					m_matProj;

	ID3D11DepthStencilState*	m_DSS[2];
	ID3D11DepthStencilView*		m_pDepthStencilView[2];
public:

	ID3D11DepthStencilView * CreateDepthStencilView(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight)
	{
		HRESULT hr;
		ID3D11DepthStencilView* pDSV = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
		D3D11_TEXTURE2D_DESC DescDepth;
		DescDepth.Width = dwWidth;
		DescDepth.Height = dwHeight;
		DescDepth.MipLevels = 1;
		DescDepth.ArraySize = 1;
		DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DescDepth.SampleDesc.Count = 1;
		DescDepth.SampleDesc.Quality = 0;
		DescDepth.Usage = D3D11_USAGE_DEFAULT;
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DescDepth.CPUAccessFlags = 0;
		DescDepth.MiscFlags = 0;
		if (FAILED(hr = pDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
		{
			return nullptr;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DescDepth.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		if (FAILED(hr = pDevice->CreateDepthStencilView(
			pDSTexture.Get(), &dsvd, &pDSV)))
		{
			return nullptr;
		}
		return pDSV;
	}

	bool Init()
	{
		HRESULT hr = S_OK;

		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsd.StencilEnable = TRUE;
		dsd.StencilReadMask = 0xff;
		dsd.StencilWriteMask = 0xff;

		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		// 디폴트 값
		dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		if (FAILED(hr = m_pd3dDevice->CreateDepthStencilState(&dsd, &m_DSS[0])))
		{
			return false;
		}

		// 스텐실 사용
		memset(&dsd, 0, sizeof(dsd));
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_ALWAYS;
		dsd.StencilEnable = TRUE;
		dsd.StencilReadMask = 0xff;
		dsd.StencilWriteMask = 0xff;

		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		if (FAILED(hr = m_pd3dDevice->CreateDepthStencilState(&dsd, &m_DSS[1])))
		{
			return false;
		}

		m_pDepthStencilView[0] = CreateDepthStencilView(m_pd3dDevice, g_rtClient.right, g_rtClient.bottom);
		m_pDepthStencilView[1] = CreateDepthStencilView(m_pd3dDevice, g_rtClient.right, g_rtClient.bottom);


		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixLookAtLH(&m_matView, &D3DXVECTOR3(0, 3, -10.0f), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);

		m_ObjBox.Init();
		m_ObjBox.Set(m_pd3dDevice);
		return true;
	}
	bool Frame()
	{
		m_ObjBox.Frame();
		return true;
	}
	bool Render()
	{
		m_pContext->ClearDepthStencilView(m_pDepthStencilView[0], D3D11_CLEAR_DEPTH, 1.0, 0);
		m_pContext->ClearDepthStencilView(m_pDepthStencilView[1], D3D10_CLEAR_STENCIL, 1.0, 0);

		/*ApplyDDS(m_pContext, g_pDSVStateEnableLessEqual);
		ApplyRS(m_pContext, m_pRSSolidState);*/
		//m_pContext->OMSetDepthStencilState(m_DSS[0], 0x00);
		ApplyDDS(m_pContext, m_DSS[0]);
		ApplyBS(m_pContext, TDxState::g_pBSNoBlend);
		ApplyRS(m_pContext, TDxState::g_pRSBackCullSolid);

		D3DXMATRIX matTrans, matRotation;
		D3DXMatrixTranslation(&matTrans, 3.0f, 0.0f, 0.0f);
		D3DXMatrixRotationY(&matRotation, m_Timer.m_fGameTime);
		m_matWorld = matTrans * matRotation;

		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matWorld, &m_matWorld);
		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matView, &m_matView);
		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matProj, &m_matProj);
		
		//m_pContext->RSSetState(DX::m_pRSSolidState);
		//m_pContext->OMSetDepthStencilState(DX::g_pDSVStateEnableLessEqual, 0);
		m_ObjBox.Render(m_pContext);


		//2)
		//m_pContext->OMSetDepthStencilState(m_DSS[1], 0x00);
		/*ApplyDDS(m_pContext, m_DSS[1]);
		ApplyBS(m_pContext, TDxState::g_pBSAlphaBlend);
		ApplyRS(m_pContext, TDxState::g_pRSBackCullSolidState);
		D3DXMATRIX Scale;
		D3DXMatrixScaling(&Scale, 2, 2, 2);
		D3DXMatrixTranslation(&m_matWorld, 0, 0, 1.0f);
		m_matWorld = Scale * m_matWorld;

		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matWorld, &m_matWorld);
		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matView, &m_matView);
		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matProj, &m_matProj);

		
		//m_pContext->RSSetState(DX::m_pRSSolidState);

		//DX::ApplyDDS(m_pContext, TDxState::g_pDSVStateEnableLessEqual);
		//m_pContext->OMSetDepthStencilState(DX::g_pDSVStateEnableLessEqual, 0);

		m_ObjBox.Render(m_pContext);*/
		return true;
	}
	bool Release()
	{
		m_ObjBox.Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("DepthBuffer_1(x)", 800, 600);