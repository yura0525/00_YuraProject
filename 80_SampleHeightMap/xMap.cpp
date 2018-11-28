#include "xMap.h"


HRESULT xMap::CreateVertexData()
{
	m_iNumVertex = m_iNumVerties;
	m_VertexList.resize(m_iNumVertex);

	float fHalfCols = (m_iNumCols - 1) / 2.0f;
	float fHalfRows = (m_iNumRows - 1) / 2.0f;
	float fOffsetU = 1.0f / (m_iNumCols - 1);
	float fOffsetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int iIndex = (iRow * m_iNumCols) + iCol;
			m_VertexList[iIndex].p.x = (iCol - fHalfCols) * m_fCellDistance;
			m_VertexList[iIndex].p.y = GetHeightOfVertex(iIndex);
			m_VertexList[iIndex].p.z = -((iRow - fHalfRows) * m_fCellDistance);
			m_VertexList[iIndex].n = GetNormalOfVertex(iIndex);
			m_VertexList[iIndex].c = GetColorOfVertex(iIndex);
			m_VertexList[iIndex].t = GetTextureOfVertex(fOffsetU * iCol, fOffsetV * iRow);
		}
	}
	return S_OK;
}
HRESULT xMap::CreateIndexData()
{
	m_iNumIndex = m_iNumFace * 3;
	m_IndexList.resize(m_iNumIndex);

	int iCurIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			//0	1   4   
			//2	3	5
			int iNextRow = iRow + 1;
			m_IndexList[iCurIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 1] = m_IndexList[iCurIndex + 0] + 1;
			m_IndexList[iCurIndex + 2] = iNextRow * m_iNumCols + iCol;	//0번의 아랫줄이다.
			m_IndexList[iCurIndex + 3] = m_IndexList[iCurIndex + 2];
			m_IndexList[iCurIndex + 4] = m_IndexList[iCurIndex + 1];
			m_IndexList[iCurIndex + 5] = iNextRow * m_iNumCols + iCol+1;

			iCurIndex += 6;
		}
	}

	return S_OK;
}


float xMap::GetHeightOfVertex(int iIndex)
{
	return m_fHeightList[iIndex] * m_fScaleHeight;
}

D3DXVECTOR2 xMap::GetTextureOfVertex(float fU, float fV)
{
	return D3DXVECTOR2(fU * 50, fV * 50);
}

D3DXVECTOR3 xMap::GetNormalOfVertex(int iIndex)
{
	return D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

D3DXVECTOR4 xMap::GetColorOfVertex(int iIndex)
{
	return D3DXVECTOR4(	randstep(0.0f, 1.0f),
						randstep(0.0f, 1.0f),
						randstep(0.0f, 1.0f), 1.0f);
}

bool xMap::CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, T_STR szName)
{
	HRESULT hr = S_OK;
	ID3D11Resource* pLoadTexture = NULL;
	
	D3DX11_IMAGE_INFO ImageInfo;

	D3DX11_IMAGE_LOAD_INFO info;
	ZeroMemory(&info, sizeof(info));
	//CPU가 읽고 쓸수 있게 하려고.DYNAMIC은 읽기는 안되고 쓰기만 됨.
	info.Usage = D3D11_USAGE_STAGING;									
	info.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	info.Format = DXGI_FORMAT_FROM_FILE;
	info.MipLevels = 1;
	info.pSrcInfo = &ImageInfo;		//ImageInfo원본에 대한 정보, info이렇게 컨버팅해달라는 정보.

	if (FAILED(hr = D3DX11CreateTextureFromFile(pDevice, szName.c_str(), &info, NULL, &pLoadTexture, NULL)))
	{
		return false;
	}

	//제일 최상의 부모가 IUnknown인 모델. ComponentModel 구조
	//(QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTex2D))로 형변환을 해야한다.)
	//ID3D11Resource로 생성하고 ID3D11Texture2D로 형변환을 해야한다.
	//__uuidof(ID3D11Texture2D)는 유일한 키이다.
	//생성과 변환을 동시에 한다. 형변환뒤 pLoadTexture는 삭제해도 된다.
	//이렇게 하는 이유는 DX가 버전이 올라가서 SDK가 달라져도 호환이 가능하게 하려고.
	ID3D11Texture2D* pTex2D = NULL;
	if (FAILED(pLoadTexture->QueryInterface(
		__uuidof(ID3D11Texture2D), (LPVOID*)&pTex2D)))
	{
		return false;
	}

	SAFE_RELEASE(pLoadTexture);

	D3D11_TEXTURE2D_DESC desc;
	pTex2D->GetDesc(&desc);

	m_fHeightList.resize(desc.Height*desc.Width);

	//Map() <-> Unmap 은 다른사람이 작업못하게 하는 CriticalSection과 비슷하다.
	D3D11_MAPPED_SUBRESOURCE map;
	UINT index = D3D11CalcSubresource(0, 0, 1);
	if (SUCCEEDED(pContext->Map(pTex2D, index, D3D11_MAP_READ, 0, &map)))
	{
		UCHAR* pTexels = (UCHAR*)map.pData;
		for (UINT row = 0; row < desc.Height; row++)
		{
			UINT rowStart = row * map.RowPitch;
			for (UINT col = 0; col < desc.Width; col++)
			{
				UINT colStart = col * 4;
				UINT uRed = pTexels[rowStart + colStart + 0];
				m_fHeightList[row * desc.Width + col] = uRed;	/// DWORD이므로 pitch/4	
			}
		}
		pContext->Unmap(pTex2D, D3D11CalcSubresource(0, 0, 1));
	}

	m_iNumRows = desc.Height;
	m_iNumCols = desc.Width;
	return true;
}
bool xMap::CreateMap(xMapDesc desc)
{
	//2의 엔승 + 1개가 가장 좋다. 1 + 1, 2 + 1, 4 + 1, 8 + 1, 16 + 1
	if (desc.iNumCols > 1025)
		desc.iNumCols = 1025;

	if (desc.iNumRows > 1025)
		desc.iNumRows = 1025;

	m_iNumRows = desc.iNumRows;
	m_iNumCols = desc.iNumCols;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumVerties = m_iNumRows * m_iNumCols;
	m_iNumFace = m_iNumCellRows * m_iNumCellCols * 2;
	m_fCellDistance = desc.fDistance;
	m_fScaleHeight = desc.fScaleHeight;
	return true;
}
bool xMap::Load(ID3D11Device *pDevice, xMapDesc desc)
{
	m_pd3dDevice = pDevice;

	if ( !CreateMap(desc) )
	{
		return false;
	}
	
	TShape::Create(m_pd3dDevice, desc.szShaderFile, desc.szTextureFile);
	return true;
}

bool xMap::Init()
{
	return true;
}
bool xMap::Frame()
{
	return true;
}
bool xMap::Render(ID3D11DeviceContext* pContext)
{
	TShape::Render(pContext);
	return true;
}
bool xMap::Release()
{
	return true;
}

xMap::xMap()
{
	m_fScaleHeight = 1.0f;
}


xMap::~xMap()
{
}
