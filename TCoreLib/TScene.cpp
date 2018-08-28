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
	m_fAngle = 0.0f;
	m_BackGround.Init();
	m_BackGround.Set((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Load(L"../../data/bk.bmp");

	m_Hero.Set(300, 300, 90, 2, 42, 58);
	m_Hero.Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");

	m_npcList.resize(g_iMaxNpcCount);
	for (int inpc = 0; inpc < g_iMaxNpcCount; inpc++)
	{
		m_npcList[inpc].Set(rand() % 800, rand() % 600, 46, 62, 68, 79);
		m_npcList[inpc].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	}

	GameDataLoad(L"../../data/SpriteList.txt");
	m_iSpriteIndex = 0;
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
		if (I_Input.Key(VK_LBUTTON) &&
			TCollision::RectInPoint(m_npcList[inpc].m_rtCollision, I_Input.m_MousePos))
		{
			m_npcList[inpc].m_bDead = true;
		}
		if( !m_npcList[inpc].m_bDead)
		{
			bChangeScene = false;
		}
	}

	if (bChangeScene)
	{
		m_bNextSceneStart = true;
	}

	if (I_Input.Key(VK_LBUTTON) == KEY_PUSH)
	{
		static float fAddTime = 0.0f;
		fAddTime += g_fSecPerFrame;

		if (fAddTime >= 0.003f);
		{
			AddEffect(I_Input.m_MousePos);
			fAddTime -= 0.003f;
		}

		for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
		{
			m_effectObjList[iObj].Init();
			m_effectObjList[iObj].Set(I_Input.m_MousePos.x, I_Input.m_MousePos.y, 1, 142, 41, 41);
			m_effectObjList[iObj].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
		}
	}

	for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	{
		m_effectObjList[iObj].Frame();
	}
	return true;

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

	m_fAngle += g_fSecPerFrame * 100.0f;

	for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	{
		m_effectObjList[iObj].RotationBlt(m_fAngle);
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

	for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	{
		m_effectObjList[iObj].Release();
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

bool TGameScene::GameDataLoad(const TCHAR* pszFileName)
{
	TCHAR pBuffer[256] = { 0, };
	TCHAR pTemp[256] = { 0, };

	int iNumSprite = 0;
	FILE* fp_src;

	_wfopen_s(&fp_src, pszFileName, _T("rt"));

	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumSprite);

	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s%d"), pTemp, _countof(pTemp), &iNumFrame);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, _countof(pTemp), &rt.left, &rt.top, &rt.right, &rt.bottom);

			m_rtSpriteList[iCnt].push_back(rt);
		}
	}
	fclose(fp_src);
	return true;
}

void TGameScene::AddEffect(POINT pos)
{
	TEffectObject obj;
	obj.SetSpriteList(m_rtSpriteList);
	obj.Init();
	obj.Set(pos.x, pos.y, 1, 142, 41, 42);
	obj.Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	obj.m_iIndexSprite = rand() % m_rtSpriteList.size();
	obj.m_fSpeed = 1000.0f;
	m_effectObjList.push_back(obj);
}


TGameScene::TGameScene()
{
	m_iSceneID = GAME_SCENE_PLAY;
	m_bNextSceneStart = false;
	m_fAngle = 0.0f;
}


TGameScene::~TGameScene()
{
}
