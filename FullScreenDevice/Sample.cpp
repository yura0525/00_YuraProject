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

GAMERUN("FullScreenDevice", 800, 600);