#include "TCore.h"

class Sample : public TCore
{
public:
	bool Init()
	{
		return true;
	}
	bool Frame()
	{
		//마우스 왼쪽 클릭을 체크하는 3가지 방법.
		if (I_Input.Key(VK_LBUTTON) == KEY_PUSH)
		{
			MessageBox(NULL, L"0", L"m_dwMouseState", MB_OK);
		}

		SHORT sKey = GetAsyncKeyState(VK_LBUTTON);
		if (sKey & 0x8001)
		{
			MessageBox(NULL, L"1", L"m_dwMouseState", MB_OK);
		}

		DWORD dwState = I_Input.m_dwMouseState[0];
		if (dwState == KEY_PUSH)
		{
			MessageBox(NULL, L"2", L"m_dwMouseState", MB_OK);
		}
		if (I_Input.m_dwMouseState[1] == KEY_PUSH)
		{
			MessageBox(NULL, L"1", L"m_dwMouseState", MB_OK);
		}
		if (I_Input.m_dwMouseState[2] == KEY_PUSH)
		{
			MessageBox(NULL, L"2", L"m_dwMouseState", MB_OK);
		}

		if (I_Input.Key(VK_END) == KEY_UP)
		{
			MessageBox(NULL, L"VK_END", L"KEY_UP", MB_OK);
		}
		if (I_Input.Key(VK_END) == KEY_PUSH)
		{
			MessageBox(NULL, L"VK_END", L"KEY_PUSH", MB_OK);
		}
		if (I_Input.Key(VK_END) == KEY_HOLD)
		{
			MessageBox(NULL, L"VK_END", L"KEY_HOLD", MB_OK);
		}
		bool bComboKey = (I_Input.Key(VK_LCONTROL) == KEY_HOLD) &&
			I_Input.Key('P') == KEY_PUSH;

		if (bComboKey)
		{
			MessageBox(NULL, L"ComboKey", L"KEY_HOLD", MB_OK);
		}
		return true;
	}
	bool Render()
	{
		return true;
	}
	bool Release()
	{
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("SampleInput", 800, 600);