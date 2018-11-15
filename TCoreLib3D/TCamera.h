#pragma once
#include "xStd.h"

class TCamera
{
public:
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;

	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vTarget;
	D3DXVECTOR3		m_vUp;

	float			m_fFov;
	float			m_fAspect;
	float			m_fNear;
	float			m_fFar;
public:
	D3DMATRIX SetViewMatrix(D3DXVECTOR3 vPos = D3DXVECTOR3(0, 0, -10), D3DXVECTOR3 vTarget = D3DXVECTOR3(0,0,1), D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0));
	D3DMATRIX SetProjMatrix(float fFov, float fAspect, float fNear = 1.0f, float fFar = 1000.0f);
	void UpdateProjMatrix(UINT width, UINT height);
public:
	TCamera();
	virtual ~TCamera();
};

