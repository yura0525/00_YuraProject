#include "TObject.h"
#include "TInput.h"

//TransparentBlt 외부기호
#pragma comment (lib, "msimg32.lib")


void TObject::Set(TPoint pos)
{
	m_pos = pos;
}

void TObject::Set(float x, float y,
	DWORD left, DWORD top, DWORD right, DWORD bottom)
{
	m_pos.x = x;
	m_pos.y = y;

	m_rtDraw.left	= left;
	m_rtDraw.top	= top;
	m_rtDraw.right	= right;
	m_rtDraw.bottom	= bottom;

	m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_fDir[0] = (rand() % 2) ? 1.0f : -1.0f;
	m_fDir[1] = (rand() % 2) ? 1.0f : -1.0f;
}

bool TObject::Load(const TCHAR* pszColor, const TCHAR* pszMask)
{
	int iIndex = I_BitmapMgr.Load(pszColor);

	m_pColorBitmap = I_BitmapMgr.GetPtr(iIndex);
	if (pszMask != NULL)
	{
		iIndex = I_BitmapMgr.Load(pszMask);
		m_pMaskBitmap = I_BitmapMgr.GetPtr(iIndex);
	}
	return true;
}

bool TObject::Init()
{
	return true;
}
bool TObject::Frame()
{
	if (I_Input.Key('W'))
	{
		m_pos.y += -1 * g_fSecPerFrame * 300.0f;
		//m_bitmap.SetOffeSet(0, 1 * g_fSecPerFrame * m_fSpeed);
	}
	if (I_Input.Key('S'))
	{
		m_pos.y += 1 * g_fSecPerFrame * 300.0f;
		//m_bitmap.SetOffeSet(0, -1 * g_fSecPerFrame * m_fSpeed);
	}
	if (I_Input.Key('A'))
	{
		m_pos.x += -1 * g_fSecPerFrame * 300.0f;
		//m_bitmap.SetOffeSet(-1 * g_fSecPerFrame * m_fSpeed, 0);
	}
	if (I_Input.Key('D'))
	{
		m_pos.x += 1 * g_fSecPerFrame * 300.0f;
		//m_bitmap.SetOffeSet(1 * g_fSecPerFrame * m_fSpeed, 0);
	}

	m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtDraw.right;
	m_rtCollision.bottom = m_rtDraw.bottom;
	//m_bitmap.Frame();
	return true;
}
bool TObject::Draw(short sType, RECT* rt)
{
	RECT rtDraw = m_rtDraw;
	if (rt != NULL)
	{
		rtDraw = *rt;
	}

	switch (sType)
	{
	case LR_ROTATION:
		StretchBlt(g_hOffScreenDC, m_pos.x + rtDraw.right, m_pos.y,
			-rtDraw.right, rtDraw.bottom,
			m_pColorBitmap->m_hMemDC,
			rtDraw.left, rtDraw.top, rtDraw.right, rtDraw.bottom,
			SRCCOPY);
			break;
	case TB_ROTATION:
		StretchBlt(g_hOffScreenDC, m_pos.x, m_pos.y + rtDraw.bottom,
			rtDraw.right, -rtDraw.bottom, m_pColorBitmap->m_hMemDC,
			rtDraw.left, rtDraw.top, rtDraw.right, rtDraw.bottom, SRCCOPY);
		break;
	case LRTB_ROTATION:
		StretchBlt(g_hOffScreenDC, m_pos.x + rtDraw.right, m_pos.y + rtDraw.bottom,
			-rtDraw.right, -rtDraw.bottom, m_pColorBitmap->m_hMemDC,
			rtDraw.left, rtDraw.top, rtDraw.right, rtDraw.bottom, SRCCOPY);
		break;
	default:
		StretchBlt(g_hOffScreenDC, m_pos.x, m_pos.y, 
			rtDraw.right, rtDraw.bottom,
			m_pColorBitmap->m_hMemDC,
			rtDraw.left, rtDraw.top, rtDraw.right, rtDraw.bottom, SRCCOPY);
		break;
	}

	if (m_bDebugRect)
	{
		int prevMode = SetROP2(g_hOffScreenDC, R2_XORPEN);
		
		Rectangle(g_hOffScreenDC, m_rtCollision.left, m_rtCollision.top,
			m_rtCollision.left + m_rtDraw.right, m_rtCollision.top + m_rtDraw.bottom);

		SetROP2(g_hOffScreenDC, prevMode);
	}
	return true;
}
bool TObject::DrawColorKey(DWORD maskColor)
{
	TransparentBlt(g_hOffScreenDC, m_pos.x, m_pos.y, m_rtDraw.right,
		m_rtDraw.bottom, m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, m_rtDraw.right,
		m_rtDraw.bottom, maskColor);
	return true;
}
bool TObject::Render()
{
	if (m_pMaskBitmap == NULL)
	{
		BitBlt(g_hOffScreenDC, m_pos.x, m_pos.y, 
			m_rtDraw.right, m_rtDraw.bottom,
			m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCCOPY);
		return true;
	}
	else
	{
		BitBlt(g_hOffScreenDC, m_pos.x, m_pos.y, m_rtDraw.right, m_rtDraw.bottom, 
			m_pMaskBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCAND);
		
		BitBlt(g_hOffScreenDC, m_pos.x, m_pos.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCINVERT);//XOR

		BitBlt(g_hOffScreenDC, m_pos.x, m_pos.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pMaskBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCINVERT);
	}
	return true;
}
bool TObject::Release()
{
	return true;
}

TObject::TObject()
{
	m_pos.x = 0;
	m_pos.y = 0;
	m_posDraw.x = 0;
	m_posDraw.y = 0;

	m_pColorBitmap = NULL;
	m_pMaskBitmap = NULL;

	m_fDir[0] = 1.0f;
	m_fDir[1] = 1.0f;
	m_fSpeed = 30.0f + (rand() % 50);

	m_bDead = false;
	m_bDebugRect = false;
}


TObject::~TObject()
{
}
