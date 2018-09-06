#pragma once
#include "xWindow.h"
#include "TDirectWrite.h"
#include "TTimer.h"

class xCore : public xWindow
{
public:
	TTimer			m_Timer;
	TDirectWrite	m_Font;
public:
	virtual bool GamePreInit() { return false; }

	bool GameInit();
	bool GameRun();
	bool GameRelease();

	bool GameFrame();
	bool GameRender();
	bool GamePreRender();
	bool GamePostRender();
	void DeleteDeviceResources(UINT iWidth, UINT iHeight);
	HRESULT CreateDeviceResources(UINT iWidth, UINT iHeight);

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool PostRender();
	virtual bool Render();
	virtual bool Release();

public:
	xCore();
	virtual ~xCore();
};

