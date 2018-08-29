#include "TScene.h"
#include "TInput.h"
#include "TSound.h"

bool TScene::SetNPCCount(int iNPC)
{
	if (g_iMaxNpcCount <= iNPC)
	{
		m_iMaxNpcCount = g_iMaxNpcCount;
		return true;
	}

	m_iMaxNpcCount = iNPC;
	return true;
}
bool TGameScene::Init()
{
	int iIndexFSM = I_FSMMgr.Load(L"../../data/fsmDefault.txt");
	int iIndexFSM2 = I_FSMMgr.Load(L"../../data/fsmLevel1.txt");

	m_EffectMgr.GameDataLoad(L"../../data/SpriteList.txt");


	m_BackGround.Init();
	m_BackGround.Set((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Load(L"../../data/bk.bmp");

	m_Hero.Set(300, 900, 90, 2, 42, 58);
	m_Hero.Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	
	m_npcList.resize(g_iMaxNpcCount);
	for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
	{
		m_npcList[inpc].SetFSM(1 + (rand() % I_FSMMgr.Size()));

		if (m_npcList[inpc].m_iFSMID == 1)
		{
			m_npcList[inpc].Set(100 + rand() % 300, 100 + rand() % 200, 46, 62, 68, 79);
		}
		else
		{
			m_npcList[inpc].Set(100 + rand() % 300, 100 + rand() % 200,115, 62, 37, 35);
		}
		m_npcList[inpc].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
		m_npcList[inpc].m_bDebugRect = true;
		m_npcList[inpc].m_fSpeed = 10 + rand() % 200;
		m_npcList[inpc].m_fAttackRadius = 30 + rand() % 100;
	}
	return true;
}
bool TGameScene::Frame()
{
	if (I_Input.Key('0') == KEY_PUSH)
	{
		m_Hero.m_bDebugRect = !m_Hero.m_bDebugRect;
		for (int i = 0; i < g_iMaxNpcCount; i++)
		{
			m_npcList[i].m_bDebugRect = !m_npcList[i].m_bDebugRect;
		}
	}
	m_BackGround.Frame();
	m_Hero.Frame();

	for (int inpc = 0; inpc < m_iMaxNpcCount; inpc++)
	{
		if (!m_npcList[inpc].m_bDead)
		{
			m_npcList[inpc].Frame();
			m_npcList[inpc].Process(&m_Hero);
		}
	}
	m_EffectMgr.Frame();

	/*for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
	{
		if ( m_EffectMgr.IsCollision(GetCollider(inpc)) )
		{
			SetNPCDead(inpc, true);
		}
	}*/

	for (int inpc = 0; inpc < m_iMaxNpcCount; inpc++)
	{
		if (TCollision::SphereInSphere(m_npcList[inpc].m_rtCollision, m_Hero.m_rtCollision))
		{
			m_Hero.m_bDead = true;
		}
	}

	bool bChangeScene = true;
	for (int inpc = 0; inpc < m_iMaxNpcCount; inpc++)
	{
		if (m_npcList[inpc].m_bDead == false)
		{
			bChangeScene = false;
		}
	}

	if (bChangeScene)
	{
		m_bNextSceneStart = true;
	}

	return true;
}
bool TGameScene::Render()
{
	m_BackGround.Render();
	m_Hero.Render();
	m_EffectMgr.Render();
	for (int inpc = 0; inpc < m_iMaxNpcCount; inpc++)
	{
		if (!m_npcList[inpc].m_bDead)
		{
			m_npcList[inpc].Render();
		}
	}

	
	return true;
}
bool TGameScene::Release()
{
	m_BackGround.Release();
	m_Hero.Release();
	m_EffectMgr.Release();

	for (int inpc = 0; inpc < m_iMaxNpcCount; inpc++)
	{
		m_npcList[inpc].Release();
	}
	
	return true;
}
bool TGameScene::Reset()
{
	m_bNextSceneStart = false;
	for (int inpc = 0; inpc < m_iMaxNpcCount; inpc++)
	{
		m_npcList[inpc].m_bDead = false;
		m_npcList[inpc].Set(rand() % 800, rand() % 600, 46, 62, 68, 79);
	}
	return true;
}

bool TGameScene::GetNPCDead(int iNPC)
{
	return m_npcList[iNPC].m_bDead;
}
void TGameScene::SetNPCDead(int iNPC, bool bDead)
{
	m_npcList[iNPC].m_bDead = bDead;

	TCHAR	csBuffer[256];
	_stprintf_s(csBuffer, L"iNPC = %d, bDead  = %d, m_npcList[iNPC].m_bDead = %d\n", iNPC, bDead, m_npcList[iNPC].m_bDead);
	OutputDebugString(csBuffer);
}
RECT TGameScene::GetCollider(int iNPC)
{
	return m_npcList[iNPC].m_rtCollision;
}

TGameScene::TGameScene()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;
}


TGameScene::~TGameScene()
{
}
