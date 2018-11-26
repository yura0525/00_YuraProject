#include "TSkyBox.h"

HRESULT TSkyBox::LoadPixelShader(T_STR szShaderName)
{
	HRESULT hr = S_OK;
	m_dxObj.m_pPixelShader.Attach(
		DX::LoadPixelShaderFile(m_pd3dDevice, szShaderName.c_str(), NULL, "PSCube"));
	return hr;
}
bool TSkyBox::Create(ID3D11Device *pDevice, T_STR szShaderName, T_STR szTextureName)
{
	D3DXMatrixScaling(&m_matWorld, 100.0f, 100.0f, 100.0f);
	return TShape::Create(pDevice, szShaderName, szTextureName);
}

//HRESULT TSkyBox::LoadTexture(T_STR szTextureName)
//{
//	HRESULT hr = S_OK;
//
//	const TCHAR* szSkyTexture[] =
//	{
//		L"../../data/st00_cm_front.bmp",
//		L"../../data/st00_cm_back.bmp",
//		L"../../data/st00_cm_right.bmp",
//		L"../../data/st00_cm_left.bmp",
//		L"../../data/st00_cm_up.bmp",
//		L"../../data/st00_cm_down.bmp"
//	};
//	for(int iTex = 0; iTex < MAX_SKYBOX_TEXTURE; iTex++)
//	{
//		m_pTexSRV[iTex].Attach(CreateShaderResourceView(m_pd3dDevice, szSkyTexture[iTex]));
//	}
//	return hr;
//}

bool TSkyBox::Render(ID3D11DeviceContext* pContext)
{
	//g_pSSWrapLinear로하면 모서리부분에 엣지가 보이게 되서
	//g_pSSWrapPoint로 하고 원래의 값으로 되돌려준다.
	ApplySS(pContext, TDxState::g_pSSWrapPoint);
	ApplyDSS(pContext, TDxState::g_pDSVStateDepthWriteDisable);

	D3DXMATRIX matView = m_matView;
	matView._41 = 0;
	matView._42 = 0;
	matView._43 = 0;
	SetMatrix(&m_matWorld, &matView, &m_matProj);
	pContext->UpdateSubresource(m_dxObj.m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	pContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)m_Primitive);

	PreRender(pContext);

	if (m_bRenerType == 0)
	{
		for (int iTex = 0; iTex < MAX_SKYBOX_TEXTURE; iTex++)
		{
			//0번레지스터에 텍스처를 한장 한장씩 넘긴다.
			pContext->PSSetShaderResources(0, 1, m_pTexSRV[iTex].GetAddressOf());
			pContext->DrawIndexed(6, 6 * iTex, 0);
		}
	}
	else if(m_bRenerType == 1)
	{
		//1번 레지스터에 텍스처 배열을 넘긴다.
		pContext->PSSetShaderResources(1, MAX_SKYBOX_TEXTURE, m_pTexSRV[0].GetAddressOf());
		pContext->PSSetShader(m_dxObj.m_pPixelShader.Get(), NULL, 0);
		pContext->DrawIndexed(m_iNumIndex, 0, 0);
	}
	else
	{
		//1번 레지스터에 텍스처 배열을 넘긴다.
		pContext->PSSetShaderResources(2, 1, m_dxObj.m_pTextureRV.GetAddressOf());
		pContext->PSSetShader(m_dxObj.m_pPixelShader.Get(), NULL, 0);
		pContext->DrawIndexed(m_iNumIndex, 0, 0);
	}

	ApplyDSS(pContext, TDxState::g_pDSVStateEnableLessEqual);
	ApplySS(pContext, TDxState::g_pSSWrapLinear);
	return true;
}

bool TSkyBox::Release()
{
	return TShape::Release();
}
TSkyBox::TSkyBox()
{
	m_bRenerType = 2;
}


TSkyBox::~TSkyBox()
{
}
