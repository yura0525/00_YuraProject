#include "TScene.h"
#include "TCore.h"

class Sample : public TCore
{
public:
	std::shared_ptr<TScene> m_pGameScene;
	std::shared_ptr<TScene> m_pLobbyScene;
	std::shared_ptr<TScene> m_pEndScene;
	
	TScene* m_pCurrentScene;

	int m_iLevel;

public:
	bool Init()
	{
		m_iLevel = 1;
		
		m_pLobbyScene = std::make_shared<TLobbyScene>();
		m_pGameScene = std::make_shared<TGameScene>();
		m_pEndScene = std::make_shared<TEndScene>();

		m_pCurrentScene = m_pLobbyScene.get();

		m_pLobbyScene->Init();
		m_pGameScene->Init();
		m_pEndScene->Init();

		return true;
	}
	bool Frame()
	{
		switch (m_pCurrentScene->m_iSceneID)
		{
		case GAME_SCENE_LOBBY:
			if (m_pCurrentScene->m_bNextSceneStart == true)
			{
				m_pCurrentScene->m_bNextSceneStart = false;
				m_pCurrentScene = m_pGameScene.get();
			}
			break;

		case GAME_SCENE_PLAY:
			if (m_pCurrentScene->m_bNextSceneStart == true)
			{
				if (m_iLevel > 3)
				{
					m_pCurrentScene = m_pEndScene.get();
					m_pCurrentScene->SetNPCCount(10);
					m_pCurrentScene->Reset();
					m_iLevel = 0;
				}
				else
				{
					m_pCurrentScene = m_pGameScene.get();
					++m_iLevel;
					m_pCurrentScene->SetNPCCount((m_iLevel * 10));
					m_pCurrentScene->Reset();
				}
				
			}
			break;
		case GAME_SCENE_END:
			break;
		}
		
		m_pCurrentScene->Frame();
		return true;
	}
	bool Render()
	{
		m_pCurrentScene->Render();
		return true;
	}
	bool Release()
	{
		m_pLobbyScene->Release();
		m_pGameScene->Release();
		m_pEndScene->Release();

		return true;
	}
public:
	Sample() {}
	~Sample()
	{
	}
};

GAMERUN("SampleUI", 800, 600);