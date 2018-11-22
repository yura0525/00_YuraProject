#include "TCameraModelView.h"

LRESULT	TCameraModelView::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//WM_LBUTTONDOWN은 오브젝트 회전
	if (msg == WM_LBUTTONDOWN)
	//if (msg == g_Input.bButton0)
	{
		short iMouseX = LOWORD(lParam);
		short iMouseY = HIWORD(lParam);

		m_WorldArcBall.OnBegin(iMouseX, iMouseY);
	}
	if (msg == WM_RBUTTONDOWN)
	//if (msg == g_Input.bButton1)
	{

	}
	if (msg == WM_MOUSEMOVE)
	{
		short iMouseX = LOWORD(lParam);
		short iMouseY = HIWORD(lParam);

		m_WorldArcBall.OnMove(iMouseX, iMouseY);
	}
	if (msg == WM_LBUTTONUP)
	{
		m_WorldArcBall.OnEnd();
	}
	if (msg == WM_RBUTTONUP)
	{

	}
	
	return 1;
}

bool TCameraModelView::Update(D3DXVECTOR4 vValue)
{
	TCamera::Update(vValue);
	D3DXMATRIX mModelRot, mModelLastRotInv, mInvView;

	D3DXMatrixInverse(&mInvView, NULL, &m_matView);
	mInvView._41 = mInvView._42 = mInvView._43 = 0.0f;
	D3DXMatrixInverse(&mModelLastRotInv, NULL, &m_mModelLastRot);

	mModelRot = m_WorldArcBall.GetRotationMatrix();
	m_mModelRot = m_mModelRot * m_matView * mModelLastRotInv * mModelRot * mInvView;

	m_mModelLastRot = m_mModelRot;
	m_mModelRot._41 = m_mModelRot._42 = m_mModelRot._43 = 0.0f;
	m_matWorld = m_mModelLastRot;
	
	return true;
}
TCameraModelView::TCameraModelView()
{
}


TCameraModelView::~TCameraModelView()
{
}
