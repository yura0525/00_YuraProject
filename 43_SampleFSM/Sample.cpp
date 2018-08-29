#include "TCore.h"
#include "TScene.h"
#include "TEnemy.h"
#include "TFSMMgr.h"

const int g_iMaxNPC = 30;

class Sample : public TCore
{
	std::vector<TEnemy>		m_npcList;
	THeroObj  m_Hero;
public:
	bool	Init()
	{
		int iIndexFSM = I_FSMMgr.Load(L"../../data/fsmDefault.txt");
		int iIndexFSM2 = I_FSMMgr.Load(L"../../data/fsmLevel1.txt");

		m_Hero.Set(300, 600, 90, 2, 42, 58);
		m_Hero.Load(
			L"../../data/bitmap1.bmp",
			L"../../data/bitmap2.bmp");

		m_npcList.resize(g_iMaxNPC);
		for (int iNpc = 0; iNpc < g_iMaxNPC; iNpc++)
		{
			m_npcList[iNpc].SetFSM(1 + (rand() % I_FSMMgr.Size()));

			if (m_npcList[iNpc].m_iFSMID == 1)
			{
				m_npcList[iNpc].Set(
					100 + rand() % 300,
					100 + rand() % 200,
					46, 62, 68, 79);
			}
			else
			{
				m_npcList[iNpc].Set(
					100 + rand() % 300,
					100 + rand() % 200,
					115, 62, 37, 35);
			}
			m_npcList[iNpc].Load(
				L"../../data/bitmap1.bmp",
				L"../../data/bitmap2.bmp");
			m_npcList[iNpc].m_bDebugRect = true;
			m_npcList[iNpc].m_fSpeed = 10 + rand() % 200;
			m_npcList[iNpc].m_fAttackRadius = 30 + rand() % 10;
			
		}
		return true;
	}

	bool	Frame()
	{
		m_Hero.Frame();
		for (int iNpc = 0; iNpc < g_iMaxNPC; iNpc++)
		{
			m_npcList[iNpc].Frame();
			m_npcList[iNpc].Process(&m_Hero);
		}
		return true;
	}
	bool	Render()
	{
		m_Hero.Render();
		for (int iNpc = 0; iNpc < g_iMaxNPC; iNpc++)
		{
			m_npcList[iNpc].Render();
		}
		return true;
	}
	bool	Release()
	{
		m_Hero.Release();
		for (int iNpc = 0; iNpc < g_iMaxNPC; iNpc++)
		{
			m_npcList[iNpc].Release();
		}
		return true;
	}
public:
	Sample() {
	}
	virtual ~Sample() {}
};

GAMERUN("SampleFSM", 800, 600)