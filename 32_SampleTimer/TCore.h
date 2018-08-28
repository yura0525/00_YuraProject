#pragma once
#include "TWindow.h"
#include "TTimer.h"

class TCore : public TWindow
{
public:
	TTimer m_Timer;

public:
	bool GameInit();
	bool GameRun();
	bool GameRelease();


public:
	TCore();
	virtual ~TCore();
};

