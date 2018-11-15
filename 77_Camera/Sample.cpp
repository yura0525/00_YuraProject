#include "TDxState.h"
#include "xCore.h"
#include "TShape.h"

using namespace DX;

class Sample : public xCore
{
public:
	TBoxShape		m_ObjBox;
	TPlaneShape     m_ObjPlane;
	TLineShape		m_ObjLine;

	ID3D11DepthStencilState*	m_DSS[2];
	ID3D11DepthStencilView*		m_pDepthStencilView[2];
public:

	void DeleteResources(UINT iWidth, UINT iHeight)
	{

	}
	HRESULT CreateResources(UINT iWidth, UINT iHeight)
	{
		return S_OK;
	}
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
		xCore::Init();
		
		HRESULT hr = S_OK;
		
		m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 3.0f, -5.0f));

		m_ObjBox.Init();
		m_ObjBox.Create(m_pd3dDevice, L"../../data/shader.hlsl", L"../../data/eye.bmp");

		m_ObjPlane.Init();
		m_ObjPlane.Create(m_pd3dDevice, L"../../data/shader.hlsl", L"../../data/eye.bmp");

		m_ObjLine.Create(m_pd3dDevice, L"../../data/shader.hlsl", L"../../data/eye.bmp");
		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_ObjPlane.Frame();
		m_ObjBox.Frame();
		m_ObjLine.Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();

		m_ObjBox.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		D3DXMATRIX matTrans, matRotation;
		/*D3DXMatrixTranslation(&matTrans, 3.0f, 0.0f, 0.0f);
		D3DXMatrixRotationY(&matRotation, m_Timer.m_fGameTime);
		m_matWorld = matTrans * matRotation;

		D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matView, &m_matView);
		D3DXMatrixTranspose(&m_Obj.m_cbData.matProj, &m_matProj);*/

		m_ObjBox.Render(m_pContext);

		
		D3DXMATRIX matWolrd;
		D3DXMatrixScaling(&matWolrd, 10.0f, 1, 1);
		D3DXMatrixTranslation(&matWolrd, -3.0f, 0.0f, 0.0f);
		//ApplyRS(m_pContext, TDxState::g_pRSNoneCullState);
		m_ObjPlane.SetMatrix(&matWolrd, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_ObjPlane.Render(m_pContext);

		//D3DXVECTOR3 vDir = m_ObjPlane.m_vLook;
		//vDir *= 10.0f;
		m_ObjLine.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_ObjLine.Draw(m_pContext, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f,0.0f,0.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f,0.0f));

		m_ObjLine.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_ObjLine.Draw(m_pContext, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f));

		m_ObjLine.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_ObjLine.Draw(m_pContext, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.0f));

		//2)
		/*D3DXMATRIX Scale;
		D3DXMatrixScaling(&Scale, 2, 2, 2);
		D3DXMatrixTranslation(&m_matWorld, 0, 0, 1.0f);
		m_matWorld = Scale * m_matWorld;
		m_Obj.SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		m_Obj.Render(m_pContext);*/
		return true;
	}
	bool Release()
	{
		xCore::Release();
		m_ObjBox.Release();
		m_ObjPlane.Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Vector_1", 800, 600);