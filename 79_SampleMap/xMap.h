#pragma once
#include "xStd.h"
#include "TShape.h"
struct xMapDesc
{
	int iNumCols;
	int iNumRows;
	float fDistance;
	float fScaleHeight;
	T_STR szTextureFile;
	T_STR szShaderFile;
};

class xMap : public TBoxShape
{
	xMapDesc		m_xMapDesc;
	int				m_iNumRows;	//������ �� ����
	int				m_iNumCols;	//������ �� ����
	int				m_iNumCellRows;	//������ �� ����
	int				m_iNumCellCols;	//������ �� ����
	int				m_iNumFace;
	int				m_iNumVerties;
	float			m_fCellDistance;

public:
	virtual HRESULT CreateVertexData() override;
	virtual HRESULT CreateIndexData() override;

	float		GetHeightOfVertex(int iIndex);
	D3DXVECTOR2 GetTextureOfVertex(float fU, float fV);
	D3DXVECTOR3 GetNormalOfVertex(int iIndex);
	D3DXVECTOR4 GetColorOfVertex(int iIndex);
	


	bool CreateMap(xMapDesc desc);
	bool Load(ID3D11Device *pDevice, xMapDesc desc);
	bool Init();
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	xMap();
	virtual ~xMap();
};

