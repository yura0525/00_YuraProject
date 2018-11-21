#include "xCore.h"
#include "xTriangleObj.h"
#include "xPlaneObj.h"

class Sample : public xCore
{
public:
	xPlaneObj		m_Object;
public:
	

public:
	bool Init()
	{
		m_Object.Init();
		m_Object.Create(m_pd3dDevice, L"vertexshader.txt", L"../../data/Dirt_Diff.dds");
		
		return true;
	}
	bool Frame()
	{
		m_Object.Frame(m_pContext);
		return true;
	}
	bool Render()
	{
		m_Object.Render(m_pContext);
		
		return true;
	}
	bool Release()
	{
		m_Object.Release();
		
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};


GAMERUN("Rotation", 800, 600);