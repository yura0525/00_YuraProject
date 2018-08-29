#include "TScene.h"
#include "TInput.h"

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
	m_bNextSceneStart = false;

	m_npcList.resize(g_iMaxNpcCount);

	m_BackGround.Init();
	m_BackGround.Set((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Load(L"../../data/bk.bmp");

	m_Hero.Set(300, 300, 90, 2, 42, 58);
	m_Hero.Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");

	for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
	{
		m_npcList[inpc].Set(rand() % 800, rand() % 600, 46, 62, 68, 79);
		m_npcList[inpc].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
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
		}
	}

	for (int inpc = 0; inpc < m_iMaxNpcCount; inpc++)
	{
		if (TCollision::RectInRect(m_npcList[inpc].m_rtCollision, m_Hero.m_rtCollision))
		{
			m_npcList[inpc].m_bDead = true;
		}
	}
	bool bChangeScene = true;
	for (int inpc = 0; inpc < m_iMaxNpcCount; inpc++)
	{
		if( !m_npcList[inpc].m_bDead)
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

TGameScene::TGameScene()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;
}


TGameScene::~TGameScene()
{
}
