#include "TDxState.h"

namespace DX
{
	ID3D11DepthStencilState*	TDxState::g_pDSVStateGreater = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSVStateEnableLessEqual = 0;

	ID3D11RasterizerState*		TDxState::g_pRSBackSolidState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSFrontCullState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSNoneCullState = 0;

	ID3D11RasterizerState*		TDxState::g_pRSBackWireFrameState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSFrontCullWireFrameState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSNoneCullWireFrameState = 0;

	ID3D11BlendState*			TDxState::g_pBSNoBlend = 0;
	ID3D11BlendState*			TDxState::g_pBSAlphaBlend = 0;

	HRESULT TDxState::CreateRS(ID3D11Device* pd3dDevice)
	{
		HRESULT hr;
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(rsDesc));
		rsDesc.DepthClipEnable = TRUE;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSBackSolidState);

		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_FRONT;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSFrontCullState);

		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_NONE;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSNoneCullState);

		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_BACK;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSBackWireFrameState);

		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_FRONT;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSFrontCullWireFrameState);

		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_NONE;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSNoneCullWireFrameState);

		return hr;
	}


	HRESULT TDxState::SetState(ID3D11Device* pd3dDevice)
	{
		HRESULT hr;
		////////////////////////////////////////blend///////////////////////////////////////
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		pd3dDevice->CreateBlendState(&bd, &TDxState::g_pBSAlphaBlend);


		D3D11_BLEND_DESC BlendState;
		ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
		BlendState.RenderTarget[0].BlendEnable = TRUE;
		BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

		BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		pd3dDevice->CreateBlendState(&BlendState, &TDxState::g_pBSNoBlend);
		

		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));

		dsd.DepthEnable = TRUE;								//Z버퍼를 활성화 여부.
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//출력되면 Z버퍼 기입.
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		//Z버퍼값이 작거나 같으면 뿌려줘라.

		hr = pd3dDevice->CreateDepthStencilState(&dsd, &TDxState::g_pDSVStateEnableLessEqual);

		dsd.DepthFunc = D3D11_COMPARISON_GREATER;
		hr = pd3dDevice->CreateDepthStencilState(&dsd, &TDxState::g_pDSVStateGreater);
		return hr;
	}
	bool TDxState::Release()
	{
		if (g_pDSVStateEnableLessEqual)
		{
			g_pDSVStateEnableLessEqual->Release();
		}
		if (g_pDSVStateGreater)
		{ 
			g_pDSVStateGreater->Release();
		}

		if (g_pRSBackSolidState)
		{
			g_pRSBackSolidState->Release();
		}

		if (g_pRSFrontCullState)
		{
			g_pRSFrontCullState->Release();
		}

		if (g_pRSNoneCullState)
		{
			g_pRSNoneCullState->Release();
		}

		if (g_pRSBackWireFrameState)
		{
			g_pRSBackWireFrameState->Release();
		}

		if (g_pRSNoneCullWireFrameState)
		{
			g_pRSNoneCullWireFrameState->Release();
		}
		if (g_pBSNoBlend)
		{
			g_pBSNoBlend->Release();
		}
		if (g_pBSAlphaBlend)
		{
			g_pBSAlphaBlend->Release();
		}
		return true;
	}
	TDxState::TDxState()
	{
		g_pDSVStateEnableLessEqual = NULL;
		g_pDSVStateGreater = NULL;

		g_pRSBackSolidState = NULL;
		g_pRSFrontCullState = NULL;
		g_pRSNoneCullState = NULL;

		g_pRSBackWireFrameState = NULL;
		g_pRSFrontCullWireFrameState = NULL;
		g_pRSNoneCullWireFrameState = NULL;

		g_pBSNoBlend = NULL;
		g_pBSAlphaBlend = NULL;
	}


	TDxState::~TDxState()
	{
	}

}
