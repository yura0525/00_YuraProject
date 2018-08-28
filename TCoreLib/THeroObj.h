#pragma once
#include "TObject.h"
class THeroObj : public TObject
{
public:
	bool bFadeStart;
public:
	bool Render();

public:
	bool FadeOut();
	bool FadeIn();
public:
	THeroObj();
	virtual ~THeroObj();
};