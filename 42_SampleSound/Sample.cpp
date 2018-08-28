#include "TScene.h"
#include "TCore.h"
#include "TSound.h"

class Sample : public TCore
{
public:
	//TSound m_Sound;

	bool Init()
	{
		TSound::GetInstance()->Init();
		TSound::GetInstance()->Load("../../data/Dance The Night Away.flac", true);
		TSound::GetInstance()->Load("../../data/DDU-DU DDU-DU.flac", false);
		TSound::GetInstance()->Load("../../data/Forever Young.flac", false);
		TSound::GetInstance()->Load("../../data/004.flac", false);
		TSound::GetInstance()->Load("../../data/Egotistic.flac", false);
		
		return true;
	}
	bool Frame()
	{
		//m_pSystem->update();를 하지 않으면 소리가 이상하게 중복된다.
		//다른걸 플레이하기전에 update()를 호출해야한다.
		TSound::GetInstance()->Frame();
		if (I_Input.Key('1') == KEY_PUSH)
		{
			TSound::GetInstance()->Play(3);
		}
		if (I_Input.Key('2') == KEY_PUSH)
		{
			TSound::GetInstance()->Paused(0);
		}
		if (I_Input.Key('3') == KEY_PUSH)
		{
			TSound::GetInstance()->Stop(0);
		}
		if (I_Input.Key('4') == KEY_PUSH)
		{
			TSound::GetInstance()->PlayEffect(1);
		}
		if (I_Input.Key(VK_INSERT) == KEY_HOLD)
		{
			TSound::GetInstance()->Volume(0, g_fSecPerFrame*1.0f, true);
		}
		if (I_Input.Key(VK_DELETE) == KEY_HOLD)
		{
			TSound::GetInstance()->Volume(0, g_fSecPerFrame*1.0f, false);
		}
		return true;
	}
	bool Render()
	{
		TSound::GetInstance()->Render();
		return true;
	}
	bool Release()
	{
		TSound::GetInstance()->Release();
		return true;
	}

	Sample() {}
	~Sample()
	{
	}
};

GAMERUN("SampleSound", 800, 600);