#pragma once
#include "TBkObj.h"
#include "THeroObj.h"
#include "TNPCObject.h"
#include "TButton.h"
#include "TEffectObject.h"
#include <vector>
using namespace std;

typedef std::vector<RECT> RECT_ARRAY;

const int g_iMaxLevel = 10;
const int g_iMaxNpcCount = 100;

enum TGameState
{
	GAME_SCENE_LOBBY = 0,
	GAME_SCENE_PLAY,
	GAME_SCENE_END
};
class TScene
{
public:
	int m_iSceneID;

	TBkObj m_BackGround;
	
	bool m_bNextSceneStart;
	int m_iMaxNpcCount;

	virtual bool Init() { return true; }
	virtual bool Frame() { return true; }
	virtual bool Render() { return true; }
	virtual bool Release() { return true; }
	virtual bool Reset()	{	return true;	}

	virtual bool IsDead() { return false; }
	virtual bool SetNPCCount(int iNPC);
public:
	TScene()
	{
		m_bNextSceneStart = false;
		m_iMaxNpcCount = 10;
	}
	virtual ~TScene() {}
};

class TLobbyScene : public TScene
{
public:
	TButton m_btnStart;
	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool IsDead() { return m_btnStart.m_bDead; }

	virtual bool FadeOut() { return m_BackGround.FadeOut(); }
	virtual bool FadeIn() { return m_BackGround.FadeIn(); }

public:
	TLobbyScene();
	virtual ~TLobbyScene();
};

class TGameScene : public TScene
{
public:
	THeroObj m_Hero;
	vector<TNPCObject> m_npcList;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Reset();

public:
	std::vector<RECT_ARRAY> m_rtSpriteList;
	int						m_iSpriteIndex;

	float						m_fAngle;
	std::vector<TEffectObject>	m_effectObjList;

public:

	bool GameDataLoad(const TCHAR* pszFileName);
	void AddEffect(POINT pos);

	TGameScene();
	virtual ~TGameScene();
};


class TEndScene : public TScene
{
public:
	TButton m_btnStart;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool IsDead() { return m_btnStart.m_bDead; }
public:
	TEndScene();
	virtual ~TEndScene();
};