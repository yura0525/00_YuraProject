#include "TDxState.h"

namespace DX
{
	ID3D11BlendState*			TDxState::g_pBSNoBlend = 0;
	ID3D11BlendState*			TDxState::g_pBSAlphaBlend = 0;

	ID3D11DepthStencilState*	TDxState::g_pDSVStateGreater = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSVStateEnableLessEqual = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSVStateDepthWriteDisable = 0;

	ID3D11RasterizerState*		TDxState::g_pRSSolid = 0;
	ID3D11RasterizerState*		TDxState::g_pRSWireFrame = 0;
	ID3D11RasterizerState*		TDxState::g_pRSBackCullSolid = 0;
	ID3D11RasterizerState*		TDxState::g_pRSFrontCullSolid = 0;
	ID3D11RasterizerState*		TDxState::g_pRSNoneCullSolid = 0;
	ID3D11RasterizerState*		TDxState::g_pRSBackCullWireFrame = 0;
	ID3D11RasterizerState*		TDxState::g_pRSFrontCullWireFrame = 0;
	ID3D11RasterizerState*		TDxState::g_pRSNoneCullWireFrame = 0;


	ID3D11SamplerState*			TDxState::g_pTexSS = 0;
	ID3D11SamplerState*			TDxState::g_pSSWrapLinear = 0;
	ID3D11SamplerState*			TDxState::g_pSSWrapPoint = 0;
	ID3D11SamplerState*			TDxState::g_pSSMirrorLinear = 0;
	ID3D11SamplerState*			TDxState::g_pSSMirrorPoint = 0;
	ID3D11SamplerState*			TDxState::g_pSSClampLinear = 0;
	ID3D11SamplerState*			TDxState::g_pSSClampPoint = 0;

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
		pd3dDevice->CreateBlendState(&bd, &g_pBSAlphaBlend);

		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		pd3dDevice->CreateBlendState(&bd, &g_pBSNoBlend);
		
		////////////////////////////////////////DEPTH_STENCIL///////////////////////////////////////
		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));

		dsd.DepthEnable = TRUE;								//Z버퍼를 활성화 여부.
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//출력되면 Z버퍼 기입.
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		//Z버퍼값이 작거나 같으면 뿌려줘라.
		hr = pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSVStateEnableLessEqual);

		dsd.DepthEnable = TRUE;
		dsd.DepthFunc = D3D11_COMPARISON_GREATER;
		hr = pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSVStateGreater);

		dsd.DepthEnable = TRUE;								//Z버퍼를 활성화 여부.
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;	//Z버퍼랑 비교도안하고 Z버퍼에 기입도 하지 않는다.
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		//Z버퍼값이 작거나 같으면 뿌려줘라.
		hr = pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSVStateDepthWriteDisable);
		
		//////////////////////////////////Rasterizer////////////////////////////////////////////////////
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(rsDesc));
		rsDesc.DepthClipEnable = TRUE;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;

		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &g_pRSSolid);
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &g_pRSBackCullSolid);

		rsDesc.CullMode = D3D11_CULL_FRONT;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &g_pRSFrontCullSolid);

		rsDesc.CullMode = D3D11_CULL_NONE;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &g_pRSNoneCullSolid);

		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_BACK;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &g_pRSWireFrame);
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &g_pRSBackCullWireFrame);

		rsDesc.CullMode = D3D11_CULL_FRONT;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &g_pRSFrontCullWireFrame);

		rsDesc.CullMode = D3D11_CULL_NONE;
		hr = pd3dDevice->CreateRasterizerState(&rsDesc, &g_pRSNoneCullWireFrame);

		//////////////////////////////////D3D11_SAMPLER_DESC////////////////////////////////////////////////////
		D3D11_SAMPLER_DESC samplerDesc;
		//텍스처 주소 지정 방식
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//필터링 방식
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

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
	bool TDxState::Release()
	{
		if (g_pDSVStateEnableLessEqual)		g_pDSVStateEnableLessEqual->Release();		g_pDSVStateEnableLessEqual = NULL;
		if (g_pDSVStateGreater)				g_pDSVStateGreater->Release();				g_pDSVStateGreater = NULL;
		if (g_pDSVStateDepthWriteDisable)	g_pDSVStateDepthWriteDisable->Release();	g_pDSVStateDepthWriteDisable = NULL;


		if (g_pRSSolid)						g_pRSSolid->Release();						g_pRSSolid = NULL;
		if (g_pRSWireFrame)					g_pRSWireFrame->Release();					g_pRSWireFrame = NULL;
		if (g_pRSBackCullSolid)				g_pRSBackCullSolid->Release();				g_pRSBackCullSolid = NULL;
		if (g_pRSFrontCullSolid)			g_pRSFrontCullSolid->Release();				g_pRSFrontCullSolid = NULL;
		if (g_pRSNoneCullSolid)				g_pRSNoneCullSolid->Release();				g_pRSNoneCullSolid = NULL;
		if (g_pRSBackCullWireFrame)			g_pRSBackCullWireFrame->Release();			g_pRSBackCullWireFrame = NULL;
		if (g_pRSFrontCullWireFrame)		g_pRSFrontCullWireFrame->Release();			g_pRSFrontCullWireFrame = NULL;
		if (g_pRSNoneCullWireFrame)			g_pRSNoneCullWireFrame->Release();			g_pRSNoneCullWireFrame = NULL;

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
	}


	TDxState::~TDxState()
	{
	}

}
