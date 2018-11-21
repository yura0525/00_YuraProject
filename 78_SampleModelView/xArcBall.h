#pragma once
#include "TCamera.h"

class xArcBall
{
public:
	bool			m_bDrag;

	D3DXVECTOR3		m_vDownPt;
	D3DXVECTOR3		m_vCurrentPt;

	D3DXQUATERNION	m_qNow;
	D3DXQUATERNION	m_qDown;
public:
	D3DXMATRIX GetRotationMatrix();
	D3DXQUATERNION QuatFromBallPoints(D3DXVECTOR3 m_vDownPt, D3DXVECTOR3 m_vCurrentPt);
	D3DXVECTOR3	ScreenToVector(float fX, float fY);
	
	void OnBegin(int x, int y);
	void OnMove(int x, int y);
	void OnEnd();
public:
	xArcBall();
	virtual ~xArcBall();
};

