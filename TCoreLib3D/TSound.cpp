#include "TSound.h"

bool TSound::Init()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(g_iMaxSound, FMOD_INIT_NORMAL, 0);
	
	/*TSound::GetInstance()->Load("../../data/Dance The Night Away.flac", true);
	TSound::GetInstance()->Load("../../data/DDU-DU DDU-DU.flac", false);
	TSound::GetInstance()->Load("../../data/Forever Young.flac", false);
	TSound::GetInstance()->Load("../../data/004.flac", false);
	TSound::GetInstance()->Load("../../data/Egotistic.flac", false);*/

	//m_pSystem->createSound("../../data/abel_leaf.asf", FMOD_DEFAULT, 0, &m_pSound[m_iSoundList++]);
	//m_pSystem->playSound(m_pSound[m_iSoundList-1], 0, false, &m_Channel[m_iSoundList-1]);

	return true;
}
bool TSound::Frame()
{
	m_pSystem->update();

	unsigned int total = 0;
	unsigned int ms = 0;

	m_pSound[0]->getLength(&total, FMOD_TIMEUNIT_MS);
	m_pChannel[0]->getPosition(&ms, FMOD_TIMEUNIT_MS);

	_stprintf_s(m_szBuffer, L"[%02d : %02d : %02d] <---> [%02d : %02d : %02d]",
		total / 1000 / 60, total / 1000 % 60, total / 10 % 60,
		ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 60);
	/*OutputDebugString(m_szBuffer);*/
	return true;
}

bool TSound::Release()
{
	for (int iSound = 0; iSound < g_iMaxSound; iSound++)
	{
		if (m_pSound[iSound] != NULL)
			m_pSound[iSound]->release();
	}
	m_pSystem->close();
	m_pSystem->release();
	return true;
}
bool TSound::Render()
{
	SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
	SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
	//SetBkMode(hdc, TRANSPARENT);			//배경색을 투명하게 한다.


	//유니코드의 wcslen 글자수를 구하는 함수 strlen이랑 같음
	TextOut(g_hOffScreenDC, 0, 100, m_szBuffer, wcslen(m_szBuffer));

	//OutputDebugString(m_szBuffer);
	return true;
}
int TSound::Load(const char* pLoadName, bool bPlay)
{
	Channel* pChannel = NULL;
	m_pSystem->createSound(pLoadName, FMOD_DEFAULT, 0, &m_pSound[m_iSoundList++]);

	if (bPlay == true)
	{
		m_pSystem->playSound(m_pSound[m_iSoundList - 1], 0, false, &pChannel);
		m_pChannel[m_iSoundList - 1] = pChannel;
	}
	
	return m_iSoundList-1;
}

void TSound::PlayEffect(int iIndex)
{
	Channel* pChannel = NULL;

	//빈채널을 찾아서 pChannel여기에 채워서 리턴해준다.(이펙트일경우엔)
	m_pSystem->playSound(m_pSound[iIndex], false, false, &pChannel);
	m_pChannel[iIndex] = pChannel;
}

void TSound::Play(int iIndex, bool bPlay, bool bLoop)
{
	bool playing = false;
	if (m_pChannel[iIndex] != NULL)
	{
		playing = m_pChannel[iIndex]->isPlaying(&playing);
	}

	if (playing == false)
	{
		if (bLoop)
		{
			m_pSound[iIndex]->setMode(FMOD_LOOP_NORMAL);
		}
		else
		{
			m_pSound[iIndex]->setMode(FMOD_LOOP_OFF);
		}
		m_pSystem->playSound(m_pSound[iIndex], false, false, &m_pChannel[iIndex]);
	}
}

void TSound::Stop(int iIndex)
{
	m_pChannel[iIndex]->stop();
}
void TSound::Paused(int iIndex)
{
	bool paused;
	m_pChannel[iIndex]->getPaused(&paused);
	m_pChannel[iIndex]->setPaused(!paused);
}

void TSound::Volume(int iIndex, float fVolume, bool bUp)
{
	float fCurrentVolume;
	m_pChannel[iIndex]->getVolume(&fCurrentVolume);

	if (bUp)
	{
		fCurrentVolume += fVolume;
		fCurrentVolume = min(1.0f, fCurrentVolume);
	}
	else
	{
		fCurrentVolume -= fVolume;
		fCurrentVolume = max(0.0f, fCurrentVolume);
	}
	m_pChannel[iIndex]->setVolume(fCurrentVolume);
}
TSound::TSound()
{
	m_iSoundList = 0;
}


TSound::~TSound()
{
}
