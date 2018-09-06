#include "TTimer.h"

float g_fSecPerFrame = 0.0f;

bool TTimer::Init()
{
	//���� �ð� ����.
	QueryPerformanceCounter(&m_BeforeTick);
	QueryPerformanceCounter(&m_Start);

	// ���ػ� Ÿ�̸� �������� �Ǵ�
	if (m_BeforeTick.QuadPart == 0)
	{
		return false;
	}

	return true;
}
bool TTimer::Frame()
{
	// 1�������� �ð� üũ 
	// ���� �ð��� ã�´�. 
	QueryPerformanceCounter(&m_CurrentTime);

	m_fSecPerFrame = static_cast<float>(m_CurrentTime.QuadPart - m_BeforeTick.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
	g_fSecPerFrame = m_fSecPerFrame;

	if (((m_CurrentTime.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1)
	{
		m_iFramePerSecond = m_iFrameCount;
		m_iFrameCount = 0;
		m_FPS = m_CurrentTime;

		//���â�� ����ϴ� �Լ� 
		//�����ڵ��϶� ������ ������ ä��� �Լ�(_stprintf_s)
		_stprintf_s(m_csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f", m_iFramePerSecond, m_fGameTime, m_fSecPerFrame);
		//OutputDebugString(m_csBuffer);
	}

	m_iFrameCount++;
	m_BeforeTick = m_CurrentTime;

	m_fGameTime = static_cast<float>(m_CurrentTime.QuadPart - m_Start.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
	return true;
}
bool TTimer::Render()
{
	////HDC hdc = GetDC(g_hWnd);

	//assert(g_hOffScreenDC != NULL);

	//SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
	//SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
	////SetBkMode(hdc, TRANSPARENT);			//������ �����ϰ� �Ѵ�.


	////�����ڵ��� wcslen ���ڼ��� ���ϴ� �Լ� strlen�̶� ����
	//TextOut(g_hOffScreenDC, 0,0, m_csBuffer, wcslen(m_csBuffer));
	////ReleaseDC(g_hWnd, hdc);

	return true;
}
bool TTimer::Release()
{
	return true;
}


TTimer::TTimer()
{
	m_iFramePerSecond = 0;
	m_iFrameCount = 0;
	m_fSecPerFrame = 0.0f;
	m_fGameTime = 0.0f;		//1) 1�������� ��� �ð�

	// �ý����� ���ļ� �������� ��� �´�. �̴� �ð��� ǥ���� ��� ���ؼ� ����Ѵ�.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
}


TTimer::~TTimer()
{
}