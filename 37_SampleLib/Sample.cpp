#include "TCore.h"
#include "TBitmap.h"

//copy "*.h" "../../include"
//copy "*.lib" "../../lib"

struct TPoint
{
	float x;
	float y;
};

class Sample : public TCore
{
	TBitmap	m_bitmap;
	TPoint	m_pos;
public:
	bool Init()
	{
		m_pos.x = 100;
		m_pos.y = 100;
		//m_fSpeed = 100.0f;
		//m_bitmap.Init();
		//m_bitmap.Load(L"bitmap1.bmp");
		//m_bitmap.SetPosition(100, 100);

		m_bitmap.Load(L"../../data/bitmap1.bmp");
		return true;
	}
	bool Frame()
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
		//m_bitmap.Frame();
		return true;
	}
	bool Render()
	{
		//각종오브젝트 드로우
		BitBlt(m_hOffScreenDC, m_pos.x, m_pos.y, 800, 600, m_bitmap.m_hMemDC, 0, 0, SRCCOPY);

		//m_bitmap.Render();
		return true;
	}
	bool Release()
	{
		//m_bitmap.Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("SampleLib", 800, 600);