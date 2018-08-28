#include "TScene.h"
#include "TInput.h"

bool TEndScene::Init()
{
	m_bNextSceneStart = false;

	m_BackGround.Init();
	m_BackGround.Set((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 800, 600);
	m_BackGround.Load(L"../../data/RAINBOW.bmp");

	m_btnStart.Init();
	m_btnStart.Set((g_rtClient.right / 2), (g_rtClient.bottom / 2), 0, 0, 334,82);
	m_btnStart.Load(L"../../data/end.bmp");
	return true;
}
bool TEndScene::Frame()
{
	m_BackGround.Frame();

	if (I_Input.Key(VK_LBUTTON) &&
		TCollision::RectInPoint(m_btnStart.m_rtCollision, I_Input.m_MousePos))
	{
		DestroyWindow(g_hWnd);
	}

	m_btnStart.Frame();
	return true;
}
bool TEndScene::Render()
{
	m_BackGround.Render();
	m_btnStart.Render();
	return true;
}
bool TEndScene::Release()
{
	m_BackGround.Release();
	m_btnStart.Release();
	return true;
}

TEndScene::TEndScene()
{
	m_iSceneID = GAME_SCENE_END;
}


TEndScene::~TEndScene()
{
}
