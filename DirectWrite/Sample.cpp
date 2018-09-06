#include "xCore.h"
#include "TDirectWrite.h"
#include "TTimer.h"

class Sample : public xCore
{
public:
	TDirectWrite	m_Write;
	TTimer			m_Timer;
public:
	bool Init()
	{
		TDevice::Init();

		IDXGISurface1*		m_pBackBuffer;

		m_Write.Init();
		m_Timer.Init();

		HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&m_pBackBuffer);
		m_Write.Set(m_hWnd, m_sd.BufferDesc.Width, m_sd.BufferDesc.Height, m_pBackBuffer);
		if (m_pBackBuffer)	m_pBackBuffer->Release();
		
		TTextArray s0, s1, s2;
		s0.pText = L"asdf";
		s1.pText = L"qwer";
		s2.pText = L"zxcv";

		m_Write.m_TextList.push_back(s0);
		m_Write.m_TextList.push_back(s1);
		m_Write.m_TextList.push_back(s2);

		return true;
	}
	bool Frame()
	{
		TDevice::Frame();
		m_Timer.Frame();
		
		return true;
	}
	bool Render()
	{
		TDevice::Render();

		m_Write.Render();
		m_Timer.Render();

		RECT rt = g_rtClient;

		m_Write.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_Write.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_Write.DrawText(g_rtClient, L"kgca!!!!!", D2D1::ColorF(1, 0, 0, 0.5f));

		//rt.top += 50;
		//m_Write.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		//m_Write.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

		//m_Write.DrawText(g_rtClient, L"kgca1111!!!!!", D2D1::ColorF(1, 0, 0, 0.7f));

		//rt.top += 50;
		//m_Write.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		//m_Write.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

		//m_Write.DrawText(g_rtClient, L"kgca2222!!!!!", D2D1::ColorF(1, 0, 0, 1));

		TCHAR	m_csBuffer[256];
		_stprintf_s(m_csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f", 
			m_Timer.m_iFramePerSecond, m_Timer.m_fGameTime, m_Timer.m_fSecPerFrame);

		rt.top += 50;
		m_Write.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_Write.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		m_Write.DrawText(g_rtClient, m_csBuffer, D2D1::ColorF(1, 0, 0, 1));
		return true;
	}
	bool Release()
	{
		TDevice::Release();
		m_Write.Release();
		m_Timer.Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("DirectWrite", 800, 600);