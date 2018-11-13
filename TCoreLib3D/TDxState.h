#pragma once
#include <d3d11_1.h>
namespace DX
{
	static ID3D11DepthStencilState *	g_pDSVStateGreater = 0;
	static ID3D11DepthStencilState*		g_pDSVStateEnableLessEqual = 0;

	static ID3D11RasterizerState*		m_pRSSolidState = 0;
	static ID3D11RasterizerState*		m_pRSFrontCullState = 0;
	static ID3D11RasterizerState*		m_pRSNoneCullState = 0;

	static ID3D11RasterizerState*		m_pRSWireFrameState = 0;
	static ID3D11RasterizerState*		m_pRSFrontCullWireFrameState = 0;
	static ID3D11RasterizerState*		m_pRSNoneCullWireFrameState = 0;


	class TDxState
	{
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
}

