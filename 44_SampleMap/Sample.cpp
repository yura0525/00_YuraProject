#include "TCore.h"
#include "TScene.h"
#include "TEnemy.h"

const int g_iMaxNPC = 30;

class Sample : public TCore
{
	shared_ptr<TScene>	m_pGameScene;
	bool				m_bGameSceneChange;
	
public:
	bool	Init()
	{
		m_bGameSceneChange = false;

		m_pGameScene = make_shared<TGameScene>();
		m_pGameScene->Init();
		m_pGameScene->SetNPCCount(g_iMaxNPC);
		return true;
	}

	bool	Frame()
	{
		m_pGameScene->Frame();
		return true;
	}
	bool	Render()
	{
		if (m_bGameSceneChange == false)
			m_pGameScene->Render();
		else
			m_pGameScene->FadeOut();

		m_bGameSceneChange = true;
		for (int inpc = 0; inpc < m_pGameScene->m_iMaxNpcCount; inpc++)
		{
			if (!m_pGameScene->GetNPCDead(inpc))
			{
				m_bGameSceneChange = false;
			}
		}
		return true;
	}
	bool	Release()
	{
		m_pGameScene->Release();
		return true;
	}
public:
	Sample() {
	}
	virtual ~Sample() {}
};

GAMERUN("SampleMap", 800, 600)