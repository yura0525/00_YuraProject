#include "TDxState.h"

namespace DX
{
	HRESULT TDxState::CreateRS(ID3D11Device* pd3dDevice)
	{
		HRESULT hr;
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));

		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		rd.DepthClipEnable = TRUE;
		
		hr = pd3dDevice->CreateRasterizerState(&rd, &m_pRSSolidState);

		rd.CullMode = D3D11_CULL_FRONT;
		hr = pd3dDevice->CreateRasterizerState(&rd, &m_pRSFrontCullState);

		rd.CullMode = D3D11_CULL_NONE;
		hr = pd3dDevice->CreateRasterizerState(&rd, &m_pRSNoneCullState);

		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.CullMode = D3D11_CULL_BACK;
		hr = pd3dDevice->CreateRasterizerState(&rd, &m_pRSWireFrameState);

		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.CullMode = D3D11_CULL_FRONT;
		hr = pd3dDevice->CreateRasterizerState(&rd, &m_pRSFrontCullWireFrameState);

		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.CullMode = D3D11_CULL_NONE;
		hr = pd3dDevice->CreateRasterizerState(&rd, &m_pRSNoneCullWireFrameState);

		return hr;
	}


	HRESULT TDxState::SetState(ID3D11Device* pd3dDevice)
	{
		HRESULT hr;
		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));

		dsd.DepthEnable = TRUE;								//Z버퍼를 활성화 여부.
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//출력되면 Z버퍼 기입.
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		//Z버퍼값이 작거나 같으면 뿌려줘라.

		hr = pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSVStateEnableLessEqual);

		dsd.DepthFunc = D3D11_COMPARISON_GREATER;
		hr = pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSVStateGreater);
		return hr;
	}
	bool TDxState::Release()
	{
		if (g_pDSVStateEnableLessEqual)
		{
			g_pDSVStateEnableLessEqual->Release();
			g_pDSVStateEnableLessEqual = NULL;
		}
		if (g_pDSVStateGreater)
		{ 
			g_pDSVStateGreater->Release();
			g_pDSVStateGreater = NULL;
		}

		if (m_pRSSolidState)
		{
			m_pRSSolidState->Release();
			m_pRSSolidState = NULL;
		}

		if (m_pRSFrontCullState)
		{
			m_pRSFrontCullState->Release();
			m_pRSFrontCullState = NULL;
		}

		if (m_pRSNoneCullState)
		{
			m_pRSNoneCullState->Release();
			m_pRSNoneCullState = NULL;
		}

		if (m_pRSWireFrameState)
		{
			m_pRSWireFrameState->Release();
			m_pRSWireFrameState = NULL;
		}

		if (m_pRSNoneCullWireFrameState)
		{
			m_pRSNoneCullWireFrameState->Release();
			m_pRSNoneCullWireFrameState = NULL;
		}
		return true;
	}
	TDxState::TDxState()
	{
		g_pDSVStateEnableLessEqual = NULL;
		g_pDSVStateGreater = NULL;

		m_pRSSolidState = NULL;
		m_pRSFrontCullState = NULL;
		m_pRSNoneCullState = NULL;

		m_pRSWireFrameState = NULL;
		m_pRSFrontCullWireFrameState = NULL;
		m_pRSNoneCullWireFrameState = NULL;
	}


	TDxState::~TDxState()
	{
	}

}
