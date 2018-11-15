#include "xCore.h"
#include "TObjectPlane.h"

class Sample : public xCore
{
public:
	TObjectPlane	m_ObjBox;

public:
	bool Init()
	{
		xCore::Init();
		m_ObjBox.Init();
		m_ObjBox.Set(m_pd3dDevice);

		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_ObjBox.Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();
		m_ObjBox.Render(m_pContext);

		return true;
	}
	bool Release()
	{
		xCore::Release();
		m_ObjBox.Release();

		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Vector_1", 800, 600);