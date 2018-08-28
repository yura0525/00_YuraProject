#include "TCore.h"


bool TCore::GameInit()
{
	m_Timer.Init();
	return true;
}
bool TCore::GameRun()
{
	m_Timer.Frame();
	m_Timer.Render();
	return true;
}
bool TCore::GameRelease()
{
	m_Timer.Release();
	return true;
}
TCore::TCore()
{
}


TCore::~TCore()
{
}
