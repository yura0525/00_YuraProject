#include "xCore.h"

class Sample : public xCore
{
public:
	bool Init()
	{
		TDevice::Init();
		return true;
	}
	bool Frame()
	{
		TDevice::Frame();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("DirectInput", 800, 600);