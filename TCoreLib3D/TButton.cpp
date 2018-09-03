#include "TButton.h"
#include "TInput.h"

bool TButton::Frame()
{
	m_hBtnCurrentBitmap = m_hColorBitmap[Btn_Normal];
	if (m_hColorBitmap[Btn_Hover] != NULL && TCollision::RectInPoint(m_rtCollision, I_Input.m_MousePos))
	{
		m_hBtnCurrentBitmap = m_hColorBitmap[Btn_Hover];
		if (m_hColorBitmap[Btn_Click] != NULL && I_Input.Key(VK_LBUTTON))
		{
			m_hBtnCurrentBitmap = m_hColorBitmap[Btn_Click];
		}
	}

	return true;
}

bool TButton::LoadUI(const TCHAR* pszColor, const TCHAR* pszMask, int iState)
{
	int iIndex = I_BitmapMgr.Load(pszColor);

	m_hColorBitmap[iState] = I_BitmapMgr.GetPtr(iIndex);
	if (pszMask != NULL)
	{
		iIndex = I_BitmapMgr.Load(pszMask);
		m_hMaskBitmap[iState] = I_BitmapMgr.GetPtr(iIndex);
	}

	m_hBtnCurrentBitmap = m_hColorBitmap[0];
	return true;
}

bool TButton::Render()
{
	BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y,
			m_rtDraw.right, m_rtDraw.bottom,
		m_hBtnCurrentBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCCOPY);
	return true;
}

TButton::TButton()
{
}


TButton::~TButton()
{
}
