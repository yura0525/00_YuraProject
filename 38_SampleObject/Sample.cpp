#include "TCore.h"
#include "TBkObj.h"
#include "TNPCObject.h"

#include <vector>
using namespace std;
#define g_iMaxNpcCount 10

class Sample : public TCore
{
public:
	TBkObj m_BackGround;
	TObject m_Hero;
	vector<TNPCObject> m_npcList;

public:
	bool Init()
	{
		m_npcList.resize(g_iMaxNpcCount);

		m_BackGround.Init();
		m_BackGround.Set(0, 0, 0, 0, 800, 600);
		m_BackGround.Load(L"../../data/bk.bmp");

		m_Hero.Set(300, 300, 90, 2, 42, 58);
		m_Hero.Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");

		for (int iObj = 0; iObj < m_npcList.size(); iObj++)
		{
			m_npcList[iObj].Set(rand() % 800, rand() % 600, 46, 62, 68, 79);
			m_npcList[iObj].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
		}
		

		return true;
	}
	bool Frame()
	{
		if (I_Input.Key('0') == KEY_PUSH)
		{
			m_Hero.m_bDebugRect = !m_Hero.m_bDebugRect;
			for (int i = 0; i < m_npcList.size(); i++)
			{
				m_npcList[i].m_bDebugRect = !m_npcList[i].m_bDebugRect;
			}
		}
		m_BackGround.Frame();
		m_Hero.Frame();

		for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
		{
			if (!m_npcList[inpc].m_bDead)
			{
				m_npcList[inpc].Frame();
			}
		}

		for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
		{
			if (TCollision::SphereInSphere(m_npcList[inpc].m_rtDraw, m_Hero.m_rtDraw))
			{
				m_npcList[inpc].m_bDead = true;
			}
		}

		for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
		{
			if (I_Input.Key(VK_LBUTTON) && 
				TCollision::SphereInPoint(m_npcList[inpc].m_rtCollision, I_Input.m_MousePos))
			{
				m_npcList[inpc].m_bDead = true;
			}
		}

		return true;
	}
	bool Render()
	{
		m_BackGround.Render();
		m_Hero.Render();
		for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
		{
			if ( !m_npcList[inpc].m_bDead )
			{
				m_npcList[inpc].Render();
			}
		}

		return true;
	}
	bool Release()
	{
		m_BackGround.Release();
		m_Hero.Release();
		for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
		{
			m_npcList[inpc].Release();
		}
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("SampleObject", 800, 600);