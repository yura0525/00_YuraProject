#pragma once
#include <d3d11_1.h>
namespace DX
{
	class TDxState
	{
	public:
		static ID3D11DepthStencilState*		g_pDSVStateGreater;
		static ID3D11DepthStencilState*		g_pDSVStateEnableLessEqual;

		static ID3D11RasterizerState*		g_pRSBackSolidState;
		static ID3D11RasterizerState*		g_pRSFrontCullState;
		static ID3D11RasterizerState*		g_pRSNoneCullState;

		static ID3D11RasterizerState*		g_pRSBackWireFrameState;
		static ID3D11RasterizerState*		g_pRSFrontCullWireFrameState;
		static ID3D11RasterizerState*		g_pRSNoneCullWireFrameState;

		static ID3D11BlendState*			g_pBSNoBlend;
		static ID3D11BlendState*			g_pBSAlphaBlend;
	public:
		static HRESULT CreateRS(ID3D11Device* pd3dDevice);
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
}

