#pragma once
#include "xArcBall.h"

class TCameraModelView : public TCamera
{
public:
	xArcBall m_WorldArcBall;
public:
	LRESULT	MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool Update(D3DXVECTOR4 vValue);
public:
	TCameraModelView();
	virtual ~TCameraModelView();
};

