#pragma once
#include "TWindow.h"
#include "TTimer.h"
#include "TInput.h"

class TCore : public TWindow
{
public:
	TTimer m_Timer;

public:
	bool GameInit();
	bool GameRun();
	bool GameRelease();

	bool GameFrame();
	bool GameRender();
public:
	virtual bool Init() { return true; }
	virtual bool Frame() { return true; }
	virtual bool Render() { return true; }
	virtual bool Release() { return true; }
	
public:
	TCore();
	virtual ~TCore();
};

