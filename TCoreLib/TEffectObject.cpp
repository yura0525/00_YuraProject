#include "TEffectObject.h"

void TEffectObject::SetSpriteList(vector<RECT_ARRAY>& rtSpriteList)
{
	m_rtSpriteList = rtSpriteList;
}

bool TEffectObject::Init()
{
	if (!m_rtSpriteList.empty())
		m_fOffSet = 1.0f / m_rtSpriteList[0].size();
	return true;
}

bool TEffectObject::Frame()
{
	static float fSpriteTime = 0.0f;
	fSpriteTime += g_fSecPerFrame;

	if (fSpriteTime >= m_fOffSet)
	{
		m_iCurrentSprite++;
		if (!m_rtSpriteList.empty())
		{
			if (m_iCurrentSprite >= m_rtSpriteList[0].size())
			{
				m_iCurrentSprite = 0;
			}
		}

		fSpriteTime -= m_fOffSet;
	}

	if (!m_rtSpriteList.empty())
	{
		RECT rt = m_rtSpriteList[0][m_iCurrentSprite];
		m_rtDraw = rt;
	}
	return true;
}

TEffectObject::TEffectObject()
{
	m_iCurrentSprite = 0;
	m_fOffSet = 1.0f;
}


TEffectObject::~TEffectObject()
{
}
