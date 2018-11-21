#include "xCore.h"
#include "TObjectPlane.h"

class Sample : public xCore
{
public:
	TObjectPlane	m_ObjBox;

public:
	bool Init()
	{
		m_ObjBox.Init();
		m_ObjBox.Set(m_pd3dDevice);

		return true;
	}
	bool Frame()
	{
		m_ObjBox.Frame();
		return true;
	}
	bool Render()
	{
		m_ObjBox.Render(m_pContext);

		return true;
	}
	bool Release()
	{
		m_ObjBox.Release();

		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Vector_1", 800, 600);