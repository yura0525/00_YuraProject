#pragma once
#include <d3d11.h>
namespace DX
{
	class TDxState
	{
	public:
		static ID3D11DepthStencilState*		g_pDSVStateGreater;
		static ID3D11DepthStencilState*		g_pDSVStateEnableLessEqual;

		static ID3D11RasterizerState*		g_pRSSolid;
		static ID3D11RasterizerState*		g_pRSWireFrameState;
		static ID3D11RasterizerState*		g_pRSBackCullSolidState;
		static ID3D11RasterizerState*		g_pRSFrontCullSolidState;
		static ID3D11RasterizerState*		g_pRSNoneCullSolidState;
		static ID3D11RasterizerState*		g_pRSBackWireFrameState;
		static ID3D11RasterizerState*		g_pRSFrontCullWireFrameState;
		static ID3D11RasterizerState*		g_pRSNoneCullWireFrameState;

		static ID3D11BlendState*			g_pBSNoBlend;
		static ID3D11BlendState*			g_pBSAlphaBlend;

		static ID3D11SamplerState*			g_pTexSS;
		static ID3D11SamplerState*			g_pSSWrapLinear;
		static ID3D11SamplerState*			g_pSSWrapPoint;
		static ID3D11SamplerState*			g_pSSMirrorLinear;
		static ID3D11SamplerState*			g_pSSMirrorPoint;
		static ID3D11SamplerState*			g_pSSClampLinear;
		static ID3D11SamplerState*			g_pSSClampPoint;

	public:
		static HRESULT CreateRS(ID3D11Device* pd3dDevice);
		static HRESULT CreateSS(ID3D11Device* pd3dDevice);
		static HRESULT SetState(ID3D11Device* pd3dDevice);
		static bool Release();
	public:
		TDxState();
		virtual ~TDxState();
	};
	static void ApplyDDS(ID3D11DeviceContext* pContext, ID3D11DepthStencilState *pState, UINT iRef = 0x01)
	{
		pContext->OMSetDepthStencilState(pState, iRef);
	}

	static void ApplyRS(ID3D11DeviceContext* pContext, ID3D11RasterizerState *pState)
	{
		pContext->RSSetState(pState);
	}

	static void ApplyBS(ID3D11DeviceContext* pContext, ID3D11BlendState* pState, 
						const float fBlendFactor[] = 0,
						UINT iMask = 0xffffffff)
	{
		pContext->OMSetBlendState(pState, fBlendFactor, iMask);
	}
	static void ApplySS(ID3D11DeviceContext* pContext, ID3D11SamplerState* pSamplerState, UINT iSlot = 0, UINT iArray = 1)
	{
		pContext->PSSetSamplers(iSlot, iArray, &pSamplerState);
	}
}
using namespace DX;