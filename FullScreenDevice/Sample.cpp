#include "xCore.h"

class Sample : public xCore
{
public:
	bool Init()
	{
		xCore::Init();
		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();
		return true;
	}
	bool Release()
	{
		xCore::Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("FullScreenDevice", 800, 600);