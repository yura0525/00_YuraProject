#include "THeroObj.h"

bool THeroObj::FadeOut()
{
	m_fAlpha -= g_fSecPerFrame * 100;
	if (m_fAlpha < 0)
	{
		m_fAlpha = 0.0f;
		return true;
	}

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
	//	//m_pColorBitmap->m_hMemDC, m_pMaskBitmap->m_hMemDC,
	//	bFadeStart = false;
	//}

	//if ( !bFadeStart && FadeIn())
	//{
	//	//m_pColorBitmap->m_hMemDC, m_pMaskBitmap->m_hMemDC,
	//	bFadeStart = true;
	//}

	if (m_fAlpha < 255)
	{
		AlphaBlend(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_posDraw.x, m_posDraw.y,
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
	
	//블렌딩하는 부분
//	static float fAlpha = 255;
//	fAlpha -= g_fSecPerFrame * 20;
//	if (fAlpha < 0)
//		return true;
//
//	static BLENDFUNCTION blend;
//	blend.BlendOp = AC_SRC_OVER;
//	blend.BlendFlags = 0;
//	blend.SourceConstantAlpha = fAlpha;
//	blend.AlphaFormat = AC_SRC_OVER;
//
//	/*AlphaBlend(g_hOffScreenDC, m_posDraw.x, m_posDraw.y,
//		m_rtDraw.right, m_rtDraw.bottom,
//		m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, m_rtDraw.right, m_rtDraw.bottom, blend);
//*/
//
//	BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
//		m_pMaskBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCAND);
//
//	BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
//		m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCINVERT);//XOR
//
//	BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
//		m_pMaskBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCINVERT);
//
	return true;
}
THeroObj::THeroObj()
{
	bFadeStart = true;
}


THeroObj::~THeroObj()
{
}
