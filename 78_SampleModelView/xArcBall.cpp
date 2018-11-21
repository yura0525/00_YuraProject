#include "xArcBall.h"

D3DXMATRIX xArcBall::GetRotationMatrix()
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &m_qNow);
	return matRot;
}
//2D -> 3D
D3DXVECTOR3	xArcBall::ScreenToVector(float fX, float fY)
{
	// Scale to screen X*X + Y*Y + Z*Z = R * R(구면방정식)
	//fx * fx + fy * fy + fz * fz = r*r = 1.0f;
	//0~800 -> -400 ~+400
	float fHalfX = (g_rtClient.right / 2.0f);
	float fHalfY = (g_rtClient.bottom / 2.0f);
	float x = -(fX - fHalfX) / fHalfX;
	float y = (fY - fHalfY) / fHalfY;
	float z = 0.0f;
	float fFlag = (x*x + y*y);

	if (fFlag > 1.0f)
	{
		x *= (1.0f / sqrtf(fFlag));
		y *= (1.0f / sqrtf(fFlag));
	}
	else
	{
		z = sqrtf(1.0f - fFlag);
	}
	return D3DXVECTOR3(x, y, z);
}

D3DXQUATERNION xArcBall::QuatFromBallPoints(D3DXVECTOR3 vDownPt, D3DXVECTOR3 vCurrentPt)
{
	float fDot = D3DXVec3Dot(&vDownPt, &vCurrentPt);
	D3DXVECTOR3 vPart;
	D3DXVec3Cross(&vPart, &vDownPt, &vCurrentPt);
	D3DXQUATERNION qRet(vPart.x, vPart.y, vPart.z, fDot);
	return qRet;
}

void xArcBall::OnBegin(int x, int y)
{
	m_bDrag = true;
	m_qDown = m_qNow;
	m_vDownPt = ScreenToVector(x, y);
}
void xArcBall::OnMove(int x, int y)
{
	if (m_bDrag)
	{
		m_vCurrentPt = ScreenToVector(x, y);
		m_qNow = m_qDown * QuatFromBallPoints(m_vDownPt, m_vCurrentPt);
	}
}
void xArcBall::OnEnd()
{
	m_bDrag = false;
}
xArcBall::xArcBall()
{
	m_bDrag = false;
	m_vDownPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vCurrentPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXQuaternionIdentity(&m_qNow);
	D3DXQuaternionIdentity(&m_qDown);
}


xArcBall::~xArcBall()
{
}
