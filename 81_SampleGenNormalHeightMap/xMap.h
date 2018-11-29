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
//정점에 겹치는 페이스
struct TIndexTable
{
	int faceIndex[6];
	TIndexTable() {
		faceIndex[0] = -1;
		faceIndex[1] = -1;
		faceIndex[2] = -1;
		faceIndex[3] = -1;
		faceIndex[4] = -1;
		faceIndex[5] = -1;
	}
};
class xMap : public TShape
{
public:
	xMapDesc		m_xMapDesc;
	int				m_iNumRows;	//정점의 행 갯수
	int				m_iNumCols;	//정점의 열 갯수
	int				m_iNumCellRows;	//정점의 행 갯수
	int				m_iNumCellCols;	//정점의 행 갯수
	int				m_iNumFace;
	int				m_iNumVerties;
	float			m_fCellDistance;
	float			m_fScaleHeight;

	vector<float>		m_fHeightList;
	vector<D3DXVECTOR3> m_FaceNormalList;
	vector<TIndexTable> m_NormalLookupTable;
public:
	
	virtual HRESULT CreateVertexData() override;
	virtual HRESULT CreateIndexData() override;

	float		GetHeightOfVertex(int iIndex);
	D3DXVECTOR2 GetTextureOfVertex(float fU, float fV);
	D3DXVECTOR3 GetNormalOfVertex(int iIndex);
	D3DXVECTOR4 GetColorOfVertex(int iIndex);
	
	// 기하쉐이더(기본값은 구를 생성) 생략함.
	HRESULT LoadGeometryShader(T_STR  szName) {
		return S_OK;
	};

	bool CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, T_STR szName);
	bool CreateMap(xMapDesc desc);
	bool Load(ID3D11Device *pDevice, xMapDesc desc);
	bool Init();
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();

public:
	void GenVertexNormal();

	void InitFaceNormals();
	void CalcFaceNormals();
	D3DXVECTOR3 ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	void GenNormalLookupTable();
	void CalcPerVertexFormalsFastLookup();
public:
	xMap();
	virtual ~xMap();
};

