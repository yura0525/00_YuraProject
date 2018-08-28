#include "TCore.h"
#include "TScene.h"
#include "TEnemy.h"
#include "TFSMMgr.h"

const int g_iMaxNPC = 30;

class Sample : public TCore
{
	shared_ptr<TScene>	m_pGameScene;
	TEffectMgr				m_EffecMgr;
public:
	bool	Init()
	{
		m_pGameScene = make_shared<TGameScene>();
		m_pGameScene->Init();

		m_EffecMgr.Init();
		return true;
	}

	bool	Frame()
	{
		m_pGameScene->Frame();
		m_EffecMgr.Frame();
		return true;
	}
	bool	Render()
	{
		m_pGameScene->Render();
		m_EffecMgr.Render();
		return true;
	}
	bool	Release()
	{
		m_pGameScene->Release();
		m_EffecMgr.Release();
		return true;
	}
public:
	Sample() {
	}
	virtual ~Sample() {}
};

GAMERUN("SampleMap", 800, 600)