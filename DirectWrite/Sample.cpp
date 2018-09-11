#include "xCore.h"

class Sample : public xCore
{
public:
	bool Init()
	{
		xCore::Init();

		IDXGISurface1*		m_pBackBuffer;

		HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&m_pBackBuffer);
		m_Font.Set(m_hWnd, m_sd.BufferDesc.Width, m_sd.BufferDesc.Height, m_pBackBuffer);
		if (m_pBackBuffer)	m_pBackBuffer->Release();
		
		TTextArray s0, s1, s2;
		s0.pText = L"aaa";
		s1.pText = L"bbb";
		s2.pText = L"ccc";

		m_Font.m_TextList.push_back(s0);
		m_Font.m_TextList.push_back(s1);
		m_Font.m_TextList.push_back(s2);

		return true;
	}
	bool Frame()
	{
		xCore::Frame();
		m_Timer.Frame();
		m_Font.Frame();
		return true;
	}
	bool Render()
	{
		xCore::Render();

		m_Font.Render();
		m_Timer.Render();

		RECT rt = g_rtClient;

		m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_Font.DrawText(g_rtClient, L"kgca!!!!!", D2D1::ColorF(1, 0, 0, 0.5f));

		TCHAR	m_csBuffer[256];
		_stprintf_s(m_csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f", 
			m_Timer.m_iFramePerSecond, m_Timer.m_fGameTime, m_Timer.m_fSecPerFrame);

		rt.top += 50;
		m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		m_Font.DrawText(g_rtClient, m_csBuffer, D2D1::ColorF(1, 0, 0, 1));
		return true;
	}
	bool Release()
	{
		xCore::Release();
		m_Font.Release();
		m_Timer.Release();
		return true;
	}
public:
	Sample() {}
	~Sample() {}
};

GAMERUN("DirectWrite", 800, 600);