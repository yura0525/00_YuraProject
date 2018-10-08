#include "xCore.h"

#pragma comment(lib, "imm32.lib")

// input method manager
// input method editor
class Sample : public xCore
{
	T_STR			m_CompString;
	T_STR			m_CompWord;
	T_STR			m_CurrentString;
	vector<T_STR>	m_StringList;

	DWORD	m_dwConversion;
	HIMC	m_hIMC;
public:
	void ResetCompositionString()
	{
		m_CompString.clear();
	}

	bool Init()
	{
		xCore::Init();

		m_StringList.clear();

		m_dwConversion = 0;		//������� �ʱ�ȭ
		m_hIMC = ImmGetContext(m_hWnd);
		
		//�������� ���ڸ� ����Ѵ�.
		ImmNotifyIME(m_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		
		return true;
	}
	bool Render()
	{
		RECT rt = g_rtClient;
		rt.top += 45;
		m_Font.DrawText(rt, m_CompString.c_str(), D2D1::ColorF(0, 0, 0, 1.0f));
		rt.top += 45;
		m_Font.DrawText(rt, m_CompWord.c_str(), D2D1::ColorF(0, 0, 0, 1.0f));
		rt.top += 45;

		m_CurrentString.clear();
		m_CurrentString = m_CompString + m_CompWord;

		m_Font.DrawText(rt, m_CurrentString.c_str(), D2D1::ColorF(0, 0, 0, 1.0f));
		return true;
	}
	bool Release()
	{
		xCore::Release();
		ImmReleaseContext(m_hWnd, m_hIMC);
		return true;
	}
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_IME_NOTIFY:
		{
			if (wParam == IMN_SETCONVERSIONMODE)	//��/��Ű�� ��������
			{
				DWORD dwConversion;
				DWORD dwSentence;
				ImmGetConversionStatus(m_hIMC, &dwConversion, &dwSentence);
				if (m_dwConversion != dwConversion)
				{
					if (dwConversion & IME_CMODE_NATIVE)//�ѱ��������̸� �ѱ��� IME_CMODE_NATIVE
					{
						MessageBox(hWnd, L"�ѱ�", L"status", MB_OK);
					}
					else
					{
						//����
						MessageBox(hWnd, L"����", L"status", MB_OK);
					}
					m_dwConversion = dwConversion;
				}
			}
		}
		break;
		/*
		WM_IME_STARTCOMPOSITION; ó������ return; �� ��� break;�� ����ϰ� �Ǹ� 
		WM_IME_COMPOSITION(������ �Ϸ�� ���尡 �߻���)���߰������� �߻��ϰ� �ȴ�. 
		WM_IME_COMPOSITION �޽����� WM_IME_COMPOSITION �޼��� ó�� ��ΰ� break;������ �����ԵǸ�
		�ѱ��� ���� �ϼ� �� �������� �� WM_CHAR �޽����� �߻��ϰ� �ȴ�. 
		�̷��� �Ǹ� �ѱ۰� ���� �� �Ϲ�Ű�� �������� ���ϰ� ó�� �ؾ� �ϱ� ������ �����ϰ� �ȴ�.
		*/
		case WM_IME_STARTCOMPOSITION:	//���ս����ߴ�.
		{
			m_CompWord.clear();
		}
		return 1;
		case WM_IME_ENDCOMPOSITION:		//���ճ�����.
		{
			m_CompWord = wParam;
		}
		return 1;
		/*case WM_IME_CHAR:			//������ �Ϸ�Ǹ� ȣ��ȴ�.
		{
			m_CompString += wParam;
		}
		return 1;*/
		case WM_IME_COMPOSITION:		//�������϶�
		{
			if (lParam & GCS_RESULTSTR)
			{
				TCHAR szBuffer[256] = { 0, };
				int size = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, szBuffer, sizeof(TCHAR) * 256);
				if (size > 0)
				{
					m_CompString += szBuffer;
				}
			}

			if (lParam & GCS_COMPSTR)
			{
				TCHAR szBuffer[256] = { 0, };
				int size = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, szBuffer, sizeof(TCHAR)* 256);
				if (size > 0)
				{
					m_CompWord = szBuffer;
				}
			}
		}
		//�ݵ�� return�� �ؾ��Ѵ�. break;�� �ϸ� window�� ó������ �𸣰� WM_CHAR�޼����� �� �߻��Ͽ�
		//���� default ���� m_CompString += wParam;�� �� ����ȴ�.
		//�츮 ��� ������ ��츮�� �ȴ�.
		return 1;
		case WM_CHAR:					//�����϶�
		{
			switch ((WCHAR)wParam)
			{
			case VK_BACK:				//�齺���̽�
			{
				if (!m_CompString.empty())
				{
					//���������ڸ� �����.
					T_ITOR	it = m_CompString.begin() + (m_CompString.size() - 1);
					m_CompString.erase(it);
				}
			}
			break;
			case VK_RETURN:				//enter�� ��������
			{
				m_StringList.push_back(m_CompString);
				ResetCompositionString();
			}
			break;
			default:
			{
				m_CompString += wParam;
			}
			break;
			}
			return 1;
		}
		}
		return xWindow::MsgProc(hWnd, msg, wParam, lParam);
	}
};
GAMERUN("Input Method Editor", 800, 600);

