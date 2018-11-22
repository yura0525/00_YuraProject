#include "TShape.h"
HRESULT TSphereShape::LoadVertexShader(T_STR  szName) {
	HRESULT hr = S_OK;
	m_dxObj.m_pVertexShader.Attach(
		DX::LoadVertexShaderFile(m_pd3dDevice,
			szName.c_str(),
			m_dxObj.m_pVSBlob.GetAddressOf(), "VS_NoMatrix"));
	return hr;
}
HRESULT TSphereShape::LoadGeometryShader(T_STR  szName)
{
	return TShape::LoadGeometryShader(szName);
}
TSphereShape::TSphereShape()
{

}

TSphereShape::~TSphereShape()
{

}