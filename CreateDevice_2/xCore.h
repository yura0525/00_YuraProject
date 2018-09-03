#pragma once
#include "xWindow.h"

class xCore : public xWindow
{
public:
	bool GameInit();
	bool GameRun();
	bool GameRelease();

	bool GameFrame();
	bool GameRender();
	bool GamePreRender();
	bool GamePostRender();
//public:
//	virtual bool Init() { return true; }
//	virtual bool Frame() { return true; }
//	virtual bool PreRender() { return true; }
//	virtual bool PostRender() { return true; }
//	virtual bool Render() { return true; }
//	virtual bool Release() { return true; }

public:
	xCore();
	virtual ~xCore();
};

