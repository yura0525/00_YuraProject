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

	m_fDir[0] = (rand() % 2) ? 1.0f : -1.0f;
	m_fDir[1] = (rand() % 2) ? 1.0f : -1.0f;

	m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	m_iMaxDistance = sqrt(m_rtDraw.right*m_rtDraw.right + 
							m_rtDraw.bottom*m_rtDraw.bottom);

	m_hColorRotateBitmap = CreateCompatibleBitmap(g_hScreenDC,
		m_iMaxDistance, m_iMaxDistance);
	m_hMaskRotateBitmap = CreateCompatibleBitmap(g_hScreenDC,
		m_iMaxDistance, m_iMaxDistance);

	m_hRotationDC = CreateCompatibleDC(g_hScreenDC);
	m_hMemMaskDC = CreateCompatibleDC(g_hScreenDC);
	m_hMemColorDC = CreateCompatibleDC(g_hScreenDC);
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
	m_hBrBack = CreateSolidBrush(RGB(255, 255, 255));
	m_fAngle = 0.0f;
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
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;
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
		StretchBlt(g_hOffScreenDC, m_posDraw.x + rtDraw.right, m_posDraw.y,
			-rtDraw.right, rtDraw.bottom,
			m_pColorBitmap->m_hMemDC,
			rtDraw.left, rtDraw.top, rtDraw.right, rtDraw.bottom,
			SRCCOPY);
			break;
	case TB_ROTATION:
		StretchBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y + rtDraw.bottom,
			rtDraw.right, -rtDraw.bottom, m_pColorBitmap->m_hMemDC,
			rtDraw.left, rtDraw.top, rtDraw.right, rtDraw.bottom, SRCCOPY);
		break;
	case LRTB_ROTATION:
		StretchBlt(g_hOffScreenDC, m_posDraw.x + rtDraw.right, m_posDraw.y + rtDraw.bottom,
			-rtDraw.right, -rtDraw.bottom, m_pColorBitmap->m_hMemDC,
			rtDraw.left, rtDraw.top, rtDraw.right, rtDraw.bottom, SRCCOPY);
		break;
	default:
		StretchBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y,
			rtDraw.right, rtDraw.bottom,
			m_pColorBitmap->m_hMemDC,
			rtDraw.left, rtDraw.top, rtDraw.right, rtDraw.bottom, SRCCOPY);
		break;
	}
	
	return true;
}
bool TObject::DrawColorKey(DWORD maskColor)
{
	TransparentBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right,
		m_rtDraw.bottom, m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, m_rtDraw.right,
		m_rtDraw.bottom, maskColor);
	return true;
}
bool TObject::Render()
{
	if (m_pMaskBitmap == NULL)
	{
		BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y,
			m_rtDraw.right, m_rtDraw.bottom,
			m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCCOPY);
		return true;
	}
	else
	{
		BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pMaskBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCAND);
		
		BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCINVERT);//XOR

		BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y, m_rtDraw.right, m_rtDraw.bottom,
			m_pMaskBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCINVERT);
	}

	if (m_bDebugRect)
	{
		int prevMode = SetROP2(g_hOffScreenDC, R2_XORPEN);

		Rectangle(g_hOffScreenDC, m_rtCollision.left, m_rtCollision.top, 
			m_rtCollision.right, m_rtCollision.bottom);

		SetROP2(g_hOffScreenDC, prevMode);
	}
	return true;
}
bool TObject::Release()
{
	DeleteObject(m_hBrBack);
	DeleteObject(m_hColorRotateBitmap);
	DeleteObject(m_hMaskRotateBitmap);
	ReleaseDC(g_hWnd, m_hRotationDC);
	ReleaseDC(g_hWnd, m_hMemMaskDC);
	ReleaseDC(g_hWnd, m_hMemColorDC);
	return true;
}
void TObject::RotationBlt(float fAngle)
{
	if (m_bDead) return;

	m_fAngle = fAngle;
	GetRotateBitmap(fAngle, m_hMaskRotateBitmap, m_pMaskBitmap);
	GetRotateBitmap(fAngle, m_hColorRotateBitmap, m_pColorBitmap);

	HBITMAP hOldMask = (HBITMAP)SelectObject(m_hMemMaskDC, m_hMaskRotateBitmap);
	HBITMAP hOldColor = (HBITMAP)SelectObject(m_hMemColorDC, m_hColorRotateBitmap);

	BitBlt(g_hOffScreenDC,
		m_pos.x - (m_iMaxDistance / 2),
		m_pos.y - (m_iMaxDistance / 2),
		m_iMaxDistance, m_iMaxDistance,
		m_hMemMaskDC, 0, 0, SRCAND);
	BitBlt(g_hOffScreenDC,
		m_pos.x - (m_iMaxDistance / 2),
		m_pos.y - (m_iMaxDistance / 2),
		m_iMaxDistance, m_iMaxDistance,
		m_hMemColorDC, 0, 0, SRCINVERT);
	BitBlt(g_hOffScreenDC,
		m_pos.x - (m_iMaxDistance / 2),
		m_pos.y - (m_iMaxDistance / 2),
		m_iMaxDistance, m_iMaxDistance,
		m_hMemMaskDC, 0, 0, SRCINVERT);

	//m_tmpObj.Render();
	SelectObject(m_hMemMaskDC, hOldMask);
	SelectObject(m_hMemColorDC, hOldColor);
}
void TObject::GetRotateBitmap(float fAngle, HBITMAP hBitmap, TBitmap* pSrcBitmap)
{
	float fRadian = fAngle * 3.141592f / 180.0f;
	float fCosine = cos(fRadian);
	float fSin = sin(fRadian);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hRotationDC, hBitmap);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hRotationDC, m_hBrBack);

	PatBlt(m_hRotationDC, 0, 0, m_iMaxDistance, m_iMaxDistance, PATCOPY);
	SelectObject(m_hRotationDC, hOldBrush);


	int iOldGraphic = SetGraphicsMode(m_hRotationDC, GM_ADVANCED);

	XFORM	xform;	//x,y,1 * (3 by 2) = x,y,1 동차좌표계
	xform.eM11 = fCosine;	xform.eM12 = -fSin;
	xform.eM21 = fSin;		xform.eM22 = fCosine;
	xform.eDx = m_iMaxDistance / 2;
	xform.eDy = m_iMaxDistance / 2;
	SetWorldTransform(m_hRotationDC, &xform);


	//원본을 뿌리기.
	BitBlt(m_hRotationDC,
		-(m_rtDraw.right / 2),
		-(m_rtDraw.bottom / 2),
		m_rtDraw.right,
		m_rtDraw.bottom,
		pSrcBitmap->m_hMemDC,
		m_rtDraw.left,
		m_rtDraw.top, SRCCOPY);

	//이거 안해주서 안나왔다. 비트맵이 제대로 안나온다.
	SelectObject(m_hRotationDC, hOldBitmap);
	SelectObject(m_hRotationDC, hOldBrush);

	xform.eM11 = 1;		xform.eM12 = 0;
	xform.eM21 = 0;		xform.eM22 = 1;
	xform.eDx = 0;
	xform.eDy = 0;

	//모든픽셀이 xform에 의해서 회전된다.
	SetWorldTransform(m_hRotationDC, &xform);
	SetGraphicsMode(m_hRotationDC, iOldGraphic);
}

bool TObject::AlphaBlend(HDC hdcDest, int x, int y, int cx, int cy,
	HDC hdcSrc, HDC hdcMaskSrc, int sx, int sy, int scx, int scy, int alpha,
	DWORD opMode, COLORREF rgbMask)
{
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression
	BMI.biSizeImage = 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed = 0;           // Always use the whole palette.
	BMI.biClrImportant = 0;

	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// Create DIB section in shared memory
	hbmSrc = CreateDIBSection(hdcSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	// Create DIB section in shared memory
	hbmDest = CreateDIBSection(hdcDest, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);


	BYTE * pMaskBits;
	HBITMAP hbmMask;
	// Create DIB section in shared memory
	hbmMask = CreateDIBSection(hdcMaskSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pMaskBits, 0, 0l);

	HDC dc = CreateCompatibleDC(g_hScreenDC);

	HBITMAP dcOld = (HBITMAP)SelectObject(dc, hbmSrc);

	if (!StretchBlt(dc, 0, 0, cx, cy, hdcSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	dcOld = (HBITMAP)SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, hdcDest, x, y, cx, cy, SRCCOPY))
		return false;

	dcOld = (HBITMAP)SelectObject(dc, hbmMask);
	if (!StretchBlt(dc, 0, 0, cx, cy, hdcMaskSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	SelectObject(dc, dcOld);
	DeleteDC(dc);


	int red = GetRValue(rgbMask);
	int green = GetGValue(rgbMask);
	int blue = GetBValue(rgbMask);

	for (int j = 0; j < cy; ++j)
	{
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];
		LPBYTE pbMaskRGB = (LPBYTE)&((DWORD*)pMaskBits)[j * cx];

		for (int i = 0; i < cx; ++i)
		{
			if (pbMaskRGB[0] == blue &&
				pbMaskRGB[1] == green &&
				pbMaskRGB[2] == red)
			{
				pbSrcRGB[0] = (pbDestRGB[0] * (255 - alpha) + pbSrcRGB[0] * alpha) >> 8;
				pbSrcRGB[1] = (pbDestRGB[1] * (255 - alpha) + pbSrcRGB[1] * alpha) >> 8;
				pbSrcRGB[2] = (pbDestRGB[2] * (255 - alpha) + pbSrcRGB[2] * alpha) >> 8;

			}
			else
			{
				pbSrcRGB[0] = pbDestRGB[0];
				pbSrcRGB[1] = pbDestRGB[1];
				pbSrcRGB[2] = pbDestRGB[2];
			}
			pbSrcRGB += 4;
			pbDestRGB += 4;
			pbMaskRGB += 4;
		}
	}

	dc = CreateCompatibleDC(g_hScreenDC);

	SelectObject(dc, hbmSrc);
	if (!BitBlt(hdcDest, x, y, cx, cy, dc, 0, 0, opMode))
		return false;
	DeleteDC(dc);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);
	DeleteObject(hbmMask);
	return true;
}

TObject::TObject()
{
	m_posDraw.x = 0;
	m_posDraw.y = 0;
	m_pColorBitmap = NULL;
	m_pMaskBitmap = NULL;

	m_fDir[0] = 1.0f;
	m_fDir[1] = 1.0f;
	m_fSpeed = 100.0f;
	m_fAttackRadius = 200.0f;

	m_bDead = false;
	m_bDebugRect = false;
	m_fAlpha = 255;
}


TObject::~TObject()
{
}
