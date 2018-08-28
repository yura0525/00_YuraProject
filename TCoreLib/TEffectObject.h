#pragma once
#include "TNPCObject.h"
#include <vector>

typedef std::vector<RECT> RECT_ARRAY;

class TEffectObject : public TNPCObject
{
public:
	int		m_iCurrentSprite;
	float	m_fOffSet;

	int		m_iIndexSprite;
	std::vector<RECT_ARRAY> m_rtSpriteList;
public:
	void SetSpriteList(vector<RECT_ARRAY>& rtSpriteList);

	bool Init();
	bool Frame();
public:
	TEffectObject();
	virtual ~TEffectObject();
};

