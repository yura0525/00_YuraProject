#pragma once
#include "TBkObj.h"
#include "TNPCObject.h"
#include "TButton.h"

#include <vector>
using namespace std;
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

public:
	TLobbyScene();
	virtual ~TLobbyScene();
};

class TGameScene : public TScene
{
public:
	TObject m_Hero;
	vector<TNPCObject> m_npcList;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Reset();

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