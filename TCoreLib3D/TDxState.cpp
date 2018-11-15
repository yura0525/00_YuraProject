#include "TDxState.h"

namespace DX
{
	ID3D11DepthStencilState*	TDxState::g_pDSVStateGreater = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSVStateEnableLessEqual = 0;


	ID3D11RasterizerState*		TDxState::g_pRSSolid = 0;
	ID3D11RasterizerState*		TDxState::g_pRSWireFrameState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSBackCullSolidState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSFrontCullSolidState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSNoneCullSolidState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSBackWireFrameState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSFrontCullWireFrameState = 0;
	ID3D11RasterizerState*		TDxState::g_pRSNoneCullWireFrameState = 0;

	ID3D11BlendState*			TDxState::g_pBSNoBlend = 0;
	ID3D11BlendState*			TDxState::g_pBSAlphaBlend = 0;

	ID3D11SamplerState*			TDxState::g_pTexSS = 0;
	ID3D11SamplerState*			TDxState::g_pSSWrapLinear = 0;
	ID3D11SamplerState*			TDxState::g_pSSWrapPoint = 0;
	ID3D11SamplerState*			TDxState::g_pSSMirrorLinear = 0;
	ID3D11SamplerState*			TDxState::g_pSSMirrorPoint = 0;
	ID3D11SamplerState*			TDxState::g_pSSClampLinear = 0;
	ID3D11SamplerState*			TDxState::g_pSSClampPoint = 0;

	HRESULT TDxState::CreateRS(ID3D11Device* pd3dDevice)
	{
		HRESULT hr;
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(rsDesc));
		rsDesc.DepthClipEnable = TRUE;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;

		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSSolid);
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSBackCullSolidState);

		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_FRONT;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSFrontCullSolidState);

		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_NONE;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSNoneCullSolidState);

		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_BACK;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSWireFrameState);
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSBackWireFrameState);

		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_FRONT;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSFrontCullWireFrameState);

		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_NONE;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSNoneCullWireFrameState);

		return hr;
	}

	HRESULT TDxState::CreateSS(ID3D11Device* pd3dDevice)
	{
		HRESULT hr;
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		//텍스처 주소 지정 방식
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//필터링 방식
		hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pTexSS);
		hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pSSWrapLinear);

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pSSWrapPoint);

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pSSMirrorLinear);


		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pSSMirrorPoint);


		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pSSClampLinear);

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pSSClampPoint);
		return hr;
	}

	HRESULT TDxState::SetState(ID3D11Device* pd3dDevice)
	{
		HRESULT hr;
		////////////////////////////////////////Alpha Blend///////////////////////////////////////
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.AlphaToCoverageEnable = FALSE;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		pd3dDevice->CreateBlendState(&bd, &TDxState::g_pBSAlphaBlend);

		bd.RenderTarget[0].BlendEnable = FALSE;
		pd3dDevice->CreateBlendState(&bd, &TDxState::g_pBSNoBlend);
		
		////////////////////////////////////////DEPTH_STENCIL///////////////////////////////////////
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
		if (g_pDSVStateEnableLessEqual)		g_pDSVStateEnableLessEqual->Release();		g_pDSVStateEnableLessEqual = NULL;
		if (g_pDSVStateGreater)				g_pDSVStateGreater->Release();				g_pDSVStateGreater = NULL;

		if (g_pRSSolid)						g_pRSSolid->Release();						g_pRSSolid = NULL;
		if (g_pRSWireFrameState)			g_pRSWireFrameState->Release();				g_pRSWireFrameState = NULL;
		if (g_pRSBackCullSolidState)		g_pRSBackCullSolidState->Release();			g_pRSBackCullSolidState = NULL;
		if (g_pRSFrontCullSolidState)		g_pRSFrontCullSolidState->Release();		g_pRSFrontCullSolidState = NULL;
		if (g_pRSNoneCullSolidState)		g_pRSNoneCullSolidState->Release();			g_pRSNoneCullSolidState = NULL;
		if (g_pRSBackWireFrameState)		g_pRSBackWireFrameState->Release();			g_pRSBackWireFrameState = NULL;
		if (g_pRSFrontCullWireFrameState)	g_pRSFrontCullWireFrameState->Release();	g_pRSFrontCullWireFrameState = NULL;
		if (g_pRSNoneCullWireFrameState)	g_pRSNoneCullWireFrameState->Release();		g_pRSNoneCullWireFrameState = NULL;

		if (g_pBSNoBlend)					g_pBSNoBlend->Release();					g_pBSNoBlend = NULL;
		if (g_pBSAlphaBlend)				g_pBSAlphaBlend->Release();					g_pBSAlphaBlend = NULL;

		if (g_pTexSS)						g_pTexSS->Release();						g_pTexSS = NULL;
		if (g_pSSWrapLinear)				g_pSSWrapLinear->Release();					g_pSSWrapLinear = NULL;
		if (g_pSSWrapPoint)					g_pSSWrapPoint->Release();					g_pSSWrapPoint = NULL;
		if (g_pSSMirrorLinear)				g_pSSMirrorLinear->Release();				g_pSSMirrorLinear = NULL;
		if (g_pSSMirrorPoint)				g_pSSMirrorPoint->Release();				g_pSSMirrorPoint = NULL;
		if (g_pSSClampLinear)				g_pSSClampLinear->Release();				g_pSSClampLinear = NULL;
		if (g_pSSClampPoint)				g_pSSClampPoint->Release();					g_pSSClampPoint = NULL;
		return true;
	}
	TDxState::TDxState()
	{
		g_pDSVStateGreater = NULL;
		g_pDSVStateEnableLessEqual = NULL;

		g_pRSSolid = NULL;
		g_pRSWireFrameState = NULL;
		g_pRSBackCullSolidState = NULL;
		g_pRSFrontCullSolidState = NULL;
		g_pRSNoneCullSolidState = NULL;
		g_pRSBackWireFrameState = NULL;
		g_pRSFrontCullWireFrameState = NULL;
		g_pRSNoneCullWireFrameState = NULL;

		g_pBSNoBlend = NULL;
		g_pBSAlphaBlend = NULL;

		g_pTexSS = NULL;
		g_pSSWrapLinear = NULL;
		g_pSSWrapPoint = NULL;
		g_pSSMirrorLinear = NULL;
		g_pSSMirrorPoint = NULL;
		g_pSSClampLinear = NULL;
		g_pSSClampPoint = NULL;
	}


	TDxState::~TDxState()
	{
	}

}
