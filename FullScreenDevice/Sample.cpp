#include "xCore.h"

class Sample : public xCore
{
public:
	bool Init()
	{
		return true;
	}
	bool Frame()
	{
		return true;
	}
	bool Render()
	{
		return true;
	}
	bool Release()
	{
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("FullScreenDevice", 800, 600);