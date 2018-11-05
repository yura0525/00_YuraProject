#pragma once
#include "xStd.h"
namespace DX
{
	ID3D11Buffer * CreateVertexBuffer(ID3D11Device* pd3dDevice, 
		UINT iNumVertex,
		UINT iVertexSize,
		void* pData,
		ID3D11Buffer** ppBuffer);
	ID3D11Buffer * CreateIndexBuffer(ID3D11Device* pd3dDevice,
		UINT iNumCount,
		UINT iSize,
		void* pData,
		ID3D11Buffer** ppBuffer);
	ID3D11Buffer * CreateConstantBuffer(ID3D11Device* pd3dDevice,
		UINT iNumCount,
		UINT iSize,
		ID3D11Buffer** ppBuffer, 
		void* pData = NULL, 
		bool bDynamic = false);
	ID3D11InputLayout* CreateInputLayout(ID3D11Device* pd3dDevice,
		DWORD dwSize,
		LPCVOID lpData,
		D3D11_INPUT_ELEMENT_DESC* layout,
		UINT numElements,
		ID3D11InputLayout** ppInputLayout);

	ID3D11VertexShader*	LoadVertexShaderFile(ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut = NULL,
		const char* pFuncionName = "VS");

	ID3D11PixelShader*	LoadPixelShaderFile(ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut = NULL,
		const char* pFuncionName = "PS");

	HRESULT CompileShaderFromFile(const WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut);

	class TDxObj
	{
	public:
		ID3D11Buffer *				m_pVertexBuffer;
		ID3D11Buffer *				m_pIndexBuffer;
		ID3D11Buffer *				m_pConstantBuffer;
		ID3D11InputLayout*			m_pInputLayout;

		ID3D11VertexShader*			m_pVertexShader;
		ID3D11PixelShader*			m_pPixelShader;
		ID3DBlob*					m_pVSBlob;

	public:
		bool PreRender(ID3D11DeviceContext* pContext, UINT iVertexSize);
		bool Render(ID3D11DeviceContext* pContext,
			UINT iVertexSize, UINT iCount);
		bool PostRender(ID3D11DeviceContext* pContext,
			UINT iVertexSize, UINT iCount);
		bool Release();
	public:
		TDxObj();
		virtual ~TDxObj();
	};

}

