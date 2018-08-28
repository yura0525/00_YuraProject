#pragma once

#include "TBitmapMgr.h"
#include "TCollision.h"

#define LR_ROTATION 0x01
#define TB_ROTATION 0x02
#define LRTB_ROTATION 0x04

struct TPoint
{
	float x;
	float y;
};

class TObject
{
public:
	TBitmap* m_pColorBitmap;
	TBitmap* m_pMaskBitmap;

	TPoint	m_pos;
	TPoint	m_posDraw;

	RECT	m_rtDraw;
	RECT	m_rtCollision;

	float	m_fDir[2];
	float	m_fSpeed;
	bool	m_bDebugRect;
	bool	m_bDead;

public:
	virtual void Set(TPoint pos);
	virtual void Set(float x, float y,
		DWORD left, DWORD top, DWORD right, DWORD bottom);

	virtual bool Load(const TCHAR* pszColor, const TCHAR* pszMask = NULL);

	virtual bool Init();
	virtual bool Frame();
	virtual bool Draw(short sType, RECT* rt = NULL);
	virtual bool DrawColorKey(DWORD maskColor);
	virtual bool Render();
	virtual bool Release();

public:
	TObject();
	virtual ~TObject();
};