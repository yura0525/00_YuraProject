#include "xCore.h"
#include "TObjectPlane.h"

class Sample : public xCore
{
public:
	TObjectPlane	m_Obj;

public:
	bool Init()
	{
		xCore::Init();
		m_Obj.Init();
		m_Obj.Set(m_pd3dDevice);

		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_Obj.Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();
		m_Obj.Render(m_pContext);

		return true;
	}
	bool Release()
	{
		xCore::Release();
		m_Obj.Release();

		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Vector_1", 800, 600);