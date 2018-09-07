#include "xCore.h"
#include "xInput.h"

class Sample : public xCore
{
public:
	xInput		m_Input;
public:
	bool Init()
	{
		xCore::Init();
		m_Input.Init();
		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_Input.Frame();

		if (g_Input.bJump)
		{
			/*D2D1::Matrix3x2F matWorld;
			D2D1_POINT_2F center;
			center.x = 400;
			center.y = 300;

			float fScale = (cosf(g_fGameTimer) * 0.5f) + 0.5f;
			D2D1::Matrix3x2F scale = matWorld.Scale(fScale, fScale, center);
			D2D1::Matrix3x2F rot = matWorld.Translation(400, 300);
			m_matWorld = fScale * rot;
*/
			int kk = 0;
		}
		return true;
	}

	bool Render()
	{
		xCore::Render();
		m_Input.Render();

		//TCHAR	m_csBuffer[256];
		//_stprintf_s(m_csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f",
		//	m_Timer.m_iFramePerSecond, m_Timer.m_fGameTime, m_Timer.m_fSecPerFrame);

		//RECT rt = g_rtClient;

		//rt.top += 50;
		//m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		//m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		//m_Font.DrawText(g_rtClient, m_csBuffer, D2D1::ColorF(1, 0, 0, 1));
		return true;
	}

	bool Release()
	{
		TDevice::Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("DirectInput", 800, 600);