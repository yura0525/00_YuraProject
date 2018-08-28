#include "TScene.h"
#include "TInput.h"

bool TLobbyScene::Init()
{
	m_bNextSceneStart = false;

	m_BackGround.Init();
	m_BackGround.Set((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Load(L"../../data/bk.bmp");

	m_btnStart.Init();
	m_btnStart.Set((g_rtClient.right/2), (g_rtClient.bottom / 2), 0, 0, 334,82);
	m_btnStart.Load(L"../../data/main_start_nor.bmp");

	return true;
}
bool TLobbyScene::Frame()
{
	m_BackGround.Frame();
	
	if (I_Input.Key(VK_LBUTTON) &&
			TCollision::RectInPoint(m_btnStart.m_rtCollision, I_Input.m_MousePos))
	{
		m_bNextSceneStart = true;
	}

	m_btnStart.Frame();
	return true;
}
bool TLobbyScene::Render()
{
	m_BackGround.Render();
	m_btnStart.Render();
	return true;
}
bool TLobbyScene::Release()
{
	m_BackGround.Release();
	m_btnStart.Release();
	return true;
}

TLobbyScene::TLobbyScene()
{
	m_iSceneID = GAME_SCENE_LOBBY;
}
TLobbyScene::~TLobbyScene()
{

}