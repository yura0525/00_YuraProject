#include "THeroObj.h"

POINT g_pHeroPos;
bool THeroObj::FadeOut()
{
	m_fAlpha -= g_fSecPerFrame * 100;
	if (m_fAlpha < 0)
	{
		m_fAlpha = 0.0f;
		return true;
	}

	g_pHeroPos.x = m_pos.x;
	g_pHeroPos.y = m_pos.y;
	return false;
}
bool THeroObj::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 100;
	if (m_fAlpha > 255)
	{
		m_fAlpha = 255.0f;
		return true;
	}

	return false;
}
bool THeroObj::Render()
{
	// fade out <-> fade in
	//if (bFadeStart && FadeOut())
	//{
	//	bFadeStart = false;
	//}

	//if ( !bFadeStart && FadeIn())
	//{
	//	bFadeStart = true;
	//}

	if (m_fAlpha < 255)
	{
		AlphaBlend(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pColorBitmap->m_hMemDC, m_pMaskBitmap->m_hMemDC,
			m_rtDraw.left, m_rtDraw.top, m_rtDraw.right, m_rtDraw.bottom, m_fAlpha, SRCCOPY, RGB(0, 0, 0));
	}
	else
	{
		BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pMaskBitmap->m_hMemDC,
			m_rtDraw.left,m_rtDraw.top, SRCAND);

		BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pColorBitmap->m_hMemDC,
			m_rtDraw.left, m_rtDraw.top, SRCINVERT);//XOR

		BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pMaskBitmap->m_hMemDC, m_rtDraw.left,m_rtDraw.top, SRCINVERT);
	}

	g_pHeroPos.x = m_pos.x;
	g_pHeroPos.y = m_pos.y;
	return true;
}
THeroObj::THeroObj()
{
	bFadeStart = true;
}


THeroObj::~THeroObj()
{
}
