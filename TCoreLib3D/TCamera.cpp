#include "TCamera.h"

void TCamera::SetSpeed(float fValue)
{
	m_fSpeed += fValue;
}

void TCamera::MoveLook(float fValue)
{
	m_vPos += m_vLook * fValue * m_fSpeed;
}
void TCamera::MoveSide(float fValue)
{
	m_vPos += m_vSide * fValue * m_fSpeed;
}
D3DXMATRIX TCamera::SetViewMatrix(
	D3DXVECTOR3 vPos,
	D3DXVECTOR3 vTarget,
	D3DXVECTOR3 vUp)
{
	m_vPos = vPos;
	m_vTarget = vTarget;
	m_vDefaultUp = vUp;
	D3DXMatrixLookAtLH(&m_matView,
		&vPos,
		&vTarget,
		&vUp);
	UpdateVector();
	return m_matView;
}

D3DXMATRIX TCamera::SetProjMatrix(float fFov,
	float fAspect,
	float fNear,
	float fFar)
{
	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	D3DXMatrixPerspectiveFovLH(&m_matProj,
		fFov,
		fAspect,
		fNear, fFar);
	return m_matProj;
}

void  TCamera::UpdateProjMatrix(UINT width, UINT height)
{
	m_fAspect = (float)width / (float)height;
	SetProjMatrix(m_fFov,
		m_fAspect,
		m_fNear, m_fFar);
}
bool TCamera::Frame()
{
	m_fSpeed -= g_fSecPerFrame;
	if (m_fSpeed < 0.5f)
		m_fSpeed = 0.5f;
	return true;
}

bool TCamera::UpdateVector()
{
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;

	m_vSide.x = m_matView._11;
	m_vSide.y = m_matView._21;
	m_vSide.z = m_matView._31;

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
	return true;
}

bool TCamera::Update(D3DXVECTOR4 vValue)
{
	//yaw, pitch, roll, radius
	D3DXMATRIX matRotation;
	D3DXQUATERNION qRotation;
	D3DXQuaternionRotationYawPitchRoll(&qRotation, vValue.y, vValue.x, vValue.z);

	m_vPos += m_vLook * vValue.w * m_fSpeed;

	D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &qRotation, &m_vPos);
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);
	UpdateVector();
	return true;
}
TCamera::TCamera()
{
	m_fSpeed = 1.0f;
}


TCamera::~TCamera()
{
}
