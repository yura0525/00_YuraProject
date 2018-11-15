#include "xCore.h"
#include "TObjectPlane.h"

class Sample : public xCore
{
public:
	TObjectPlane	m_ObjBox;
	D3D11_VIEWPORT	m_vp[4];
	
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_matView;
	D3DXMATRIX					m_matProj;
	ID3D11DepthStencilView*		m_pDSV;
	ID3D11DepthStencilState*	m_pDSVStateEnable;
	ID3D11DepthStencilState*	m_pDSVStateDisable;

public:
	HRESULT CreateDSV()
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

		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));

		dsd.DepthEnable = TRUE;								//Z버퍼를 활성화 여부.
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//출력되면 Z버퍼 기입.
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		//Z버퍼값이 작거나 같으면 뿌려줘라.

		hr = m_pd3dDevice->CreateDepthStencilState(&dsd, &m_pDSVStateEnable);

		dsd.DepthFunc = D3D11_COMPARISON_GREATER;
		hr = m_pd3dDevice->CreateDepthStencilState(&dsd, &m_pDSVStateDisable);
		return hr;
	}

	bool Init()
	{
		xCore::Init();

		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixLookAtLH(&m_matView, &D3DXVECTOR3(0,0,-5.0f), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);

		CreateDSV();

		m_ObjBox.Init();
		m_ObjBox.Set(m_pd3dDevice);

		m_vp[0].TopLeftX = 0;
		m_vp[0].TopLeftY = 0;
		m_vp[0].Width = g_rtClient.right / 2;
		m_vp[0].Height = g_rtClient.bottom / 2;
		m_vp[0].MinDepth = 0.0f;
		m_vp[0].MaxDepth = 1.0f;

		m_vp[1].TopLeftX = g_rtClient.right / 2;
		m_vp[1].TopLeftY = 0;
		m_vp[1].Width = g_rtClient.right / 2;
		m_vp[1].Height = g_rtClient.bottom / 2;
		m_vp[1].MinDepth = 0.0f;
		m_vp[1].MaxDepth = 1.0f;


		m_vp[2].TopLeftX = 0;
		m_vp[2].TopLeftY = g_rtClient.bottom / 2;
		m_vp[2].Width = g_rtClient.right / 2;
		m_vp[2].Height = g_rtClient.bottom / 2;
		m_vp[2].MinDepth = 0.0f;
		m_vp[2].MaxDepth = 1.0f;


		m_vp[3].TopLeftX = g_rtClient.right / 2;
		m_vp[3].TopLeftY = g_rtClient.bottom / 2;
		m_vp[3].Width = g_rtClient.right / 2;
		m_vp[3].Height = g_rtClient.bottom / 2;
		m_vp[3].MinDepth = 0.0f;
		m_vp[3].MaxDepth = 1.0f;
		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_ObjBox.Frame();
		return true;
	}
	bool Render()
	{
		m_ObjBox.m_cbData.vColor = D3DXVECTOR4(1, 0, 0, 1);
		m_matWorld._43 = 1.0f;
		D3DXMatrixLookAtLH(&m_matView,
			&D3DXVECTOR3(+0.0f, +0.0f, -80.0f * (cosf(g_fGameTimer) * 0.5f + 0.5f)),
			&D3DXVECTOR3(+0.0f, +0.0f, +0.0f),
			&D3DXVECTOR3(+0.0f, +1.0f, +0.0f));

		D3DXMATRIX ll;
		D3DXMatrixRotationZ(&ll, g_fGameTimer);
		m_matView = ll * m_matView;

		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matWorld, &m_matWorld);
		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matView, &m_matView);
		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matProj, &m_matProj);

		m_pContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDSV);
		m_pContext->OMSetDepthStencilState(m_pDSVStateEnable, 0x00);

		//m_Obj.m_cbData.fActiveVP = 0;
		//m_pContext->RSSetViewports(1, &m_vp[0]);
		m_ObjBox.Render(m_pContext);

		m_pContext->OMSetDepthStencilState(m_pDSVStateDisable, 0);
		//m_Obj.m_cbData.fActiveVP = 0;
		//m_pContext->RSSetViewports(1, &m_vp[1]);m_Obj.m_cbData.vColor = D3DXVECTOR4(0, 0, 1, 1);
		m_matWorld._43 = 3.0f;
		m_ObjBox.m_cbData.vColor = D3DXVECTOR4(0, 0, 1, 1);
		//D3DXMatrixRotationX(&m_matWorld, m_Timer.m_fGameTime);

		/*D3DXMATRIX matPos;
		D3DXMatrixTranslation(&matPos, 1.0f, 0, 3.0f);

		m_matWorld = matPos * m_matWorld;*/
		//m_matWorld._43 = 3.0f;
		//m_matWorld._41 = 1.0f;
		D3DXMatrixTranspose(&m_ObjBox.m_cbData.matWorld, &m_matWorld);
		m_ObjBox.Render(m_pContext);

		//m_pContext->RSSetViewports(2, &m_vp[2]);
		//2
		//m_Obj.m_cbData.fActiveVP = 0;
		//m_pContext->GSSetConstantBuffers(0, 1, m_Obj.m_dxObj.m_pConstantBuffer.GetAddressOf());
		//m_Obj.Render(m_pContext);
		
		//3
		//m_Obj.m_cbData.fActiveVP = 1;
		//m_pContext->RSSetViewports(2, &m_vp[2]);
		//m_Obj.Render(m_pContext);
		return xCore::Render();
	}
	bool Release()
	{
		xCore::Release();
		m_ObjBox.Release();

		SAFE_RELEASE(m_pDSV);
		SAFE_RELEASE(m_pDSVStateEnable);
		SAFE_RELEASE(m_pDSVStateDisable);
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Vector_1", 800, 600);