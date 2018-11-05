#include "TDxObject.h"
namespace DX
{
	ID3D11Buffer * CreateVertexBuffer(ID3D11Device* pd3dDevice, UINT iNumVertex, UINT iVertexSize, void* pData, ID3D11Buffer** ppBuffer)
	{
		HRESULT hr;
		ID3D11Buffer* pBuffer = NULL;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;

		ZeroMemory(&bd, sizeof(bd));
		ZeroMemory(&sd, sizeof(sd));

		//����Ʈ�̸� UpdateSubResource()
		//D3D11_USAGE_DEFAULT , D3D11_USAGE_DYNAMIC�������� �˾ƾ��Ѵ�.
		//D3D11_USAGE_DEFAULT <------------>D3D11_USAGE_DYNAMIC
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.ByteWidth = iNumVertex * iVertexSize;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		sd.pSysMem = pData;

		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &sd, &pBuffer)))
		{
			return NULL;
		}
		*ppBuffer = pBuffer;
		return pBuffer;
	}

ID3D11Buffer * CreateIndexBuffer(ID3D11Device* pd3dDevice,
	UINT iNumCount,
	UINT iSize,
	void* pData,
	ID3D11Buffer** ppBuffer)
{
	HRESULT hr;
	ID3D11Buffer* pBuffer = NULL;
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA sd;

	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&sd, sizeof(sd));

	//����Ʈ�̸� UpdateSubResource()
	//D3D11_USAGE_DEFAULT , D3D11_USAGE_DYNAMIC�������� �˾ƾ��Ѵ�.
	//D3D11_USAGE_DEFAULT <------------>D3D11_USAGE_DYNAMIC
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = iNumCount * iSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	sd.pSysMem = pData;

	if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &sd, &pBuffer)))
	{
		return NULL;
	}
	*ppBuffer = pBuffer;
	return pBuffer;
}

ID3D11Buffer * CreateConstantBuffer(ID3D11Device* pd3dDevice,
	UINT iNumCount,
	UINT iSize,
	ID3D11Buffer** ppBuffer,
	void* pData,
	bool bDynamic)
{
	HRESULT hr;
	ID3D11Buffer* pBuffer = NULL;
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA sd;

	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&sd, sizeof(sd));

	//����Ʈ�̸� UpdateSubResource()<---------------> map(),unMap()
	//D3D11_USAGE_DEFAULT , D3D11_USAGE_DYNAMIC�������� �˾ƾ��Ѵ�.
	//D3D11_USAGE_DEFAULT <------------>D3D11_USAGE_DYNAMIC
	if (bDynamic)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}

	bd.ByteWidth = iNumCount * iSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (pData != NULL)
	{
		sd.pSysMem = pData;
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &sd, &pBuffer)))
		{
			return NULL;
		}
	}
	else
	{
		sd.pSysMem = pData;
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
		{
			return NULL;
		}
	}

	*ppBuffer = pBuffer;
	return pBuffer;
}

ID3D11InputLayout* CreateInputLayout(ID3D11Device* pd3dDevice,
	DWORD dwSize,
	LPCVOID lpData,
	D3D11_INPUT_ELEMENT_DESC* layout,
	UINT numElements,
	ID3D11InputLayout** ppInputLayout)
{
	ID3D11InputLayout* pInputLayout;

	HRESULT hr;
	if (FAILED(hr = pd3dDevice->CreateInputLayout(layout,
		numElements,
		lpData,
		dwSize,
		&pInputLayout)))
		{
			return NULL;
		}

		*ppInputLayout = pInputLayout;
		return pInputLayout;
	}
	
	ID3D11VertexShader*	LoadVertexShaderFile(
		ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut,
		const char* pFuncionName)
	{
		HRESULT hr;
		ID3DBlob* pBlob;
		ID3D11VertexShader*	pVertexShader;

		if (FAILED(hr = CompileShaderFromFile(
			(TCHAR*)pShaderFile, 
			pFuncionName,
			"vs_5_0",
			&pBlob)))
		{
			return NULL;
		}
		if (FAILED(hr = pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			NULL,
			&pVertexShader)))
		{
			if( pBlob )
				pBlob->Release();
			return NULL;
		}

		if (ppBlobOut == NULL)
		{
			pBlob->Release();
		}
		else
		{
			*ppBlobOut = pBlob;
		}
		
		return pVertexShader;
	}

	HRESULT CompileShaderFromFile(const WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )	
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			if (pErrorBlob) pErrorBlob->Release();
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	ID3D11PixelShader*	LoadPixelShaderFile(ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut,
		const char* pFuncionName)
	{
		HRESULT hr;
		ID3DBlob* pBlob;
		ID3D11PixelShader*	pPixelShader;

		if (FAILED(hr = CompileShaderFromFile(
			(TCHAR*)pShaderFile,
			pFuncionName,
			"ps_5_0",
			&pBlob)))
		{
			return NULL;
		}
		if (FAILED(hr = pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			NULL,
			&pPixelShader)))
		{
			if (pBlob)
				pBlob->Release();
			return NULL;
		}

		if (ppBlobOut == NULL)
		{
			pBlob->Release();
		}
		else
		{
			*ppBlobOut = pBlob;
		}

		return pPixelShader;
	}

	bool TDxObj::PreRender(ID3D11DeviceContext* pContext, UINT iVertexSize)
	{
		pContext->IASetInputLayout(m_pInputLayout);
		UINT stride = iVertexSize;
		UINT offset = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pContext->VSSetShader(m_pVertexShader, NULL, 0);
		pContext->PSSetShader(m_pPixelShader, NULL, 0);
		return true;
	}
	bool TDxObj::Render(ID3D11DeviceContext* pContext,
		UINT iVertexSize, UINT iCount)
	{
		PreRender(pContext, iVertexSize);
		PostRender(pContext, iVertexSize, iCount);
		return true;
	}
	bool TDxObj::PostRender(ID3D11DeviceContext* pContext,
		UINT iVertexSize, UINT iCount)
	{
		if(m_pIndexBuffer != NULL)
			pContext->DrawIndexed(iCount, 0, 0);
		else
			pContext->Draw(iCount, 0);

		return true;
	}
	bool TDxObj::Release()
	{
		SAFE_RELEASE(m_pVertexBuffer);
		SAFE_RELEASE(m_pIndexBuffer);
		SAFE_RELEASE(m_pConstantBuffer);
		SAFE_RELEASE(m_pInputLayout);
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPixelShader);

		return true;
	}
	TDxObj::TDxObj()
	{
		SAFE_ZERO(m_pVertexBuffer);
		SAFE_ZERO(m_pIndexBuffer);
		SAFE_ZERO(m_pConstantBuffer);
		SAFE_ZERO(m_pInputLayout);
		SAFE_ZERO(m_pVertexShader);
		SAFE_ZERO(m_pPixelShader);
	}


	TDxObj::~TDxObj()
	{
	}

}