#include "TCore.h"


bool TCore::GameInit()
{
	m_Timer.Init();
	I_Input.Init();
	Init();

	return true;
}

bool TCore::GameFrame()
{
	m_Timer.Frame();
	I_Input.Frame();
	Frame();
	return true;
}

bool TCore::GameRender()
{
	Render();
	m_Timer.Render();
	I_Input.Render();
	return true;
}
bool TCore::GameRun()
{
	GameFrame();
	GameRender();

	return true;
}
bool TCore::GameRelease()
{
	Release();
	m_Timer.Release();
	I_Input.Release();
	return true;
}
TCore::TCore()
{
}


TCore::~TCore()
{
}
