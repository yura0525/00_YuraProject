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

		m_dwConversion = 0;		//영어모드로 초기화
		m_hIMC = ImmGetContext(m_hWnd);
		
		//조합중인 글자를 취소한다.
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
			if (wParam == IMN_SETCONVERSIONMODE)	//한/영키를 눌렀을때
			{
				DWORD dwConversion;
				DWORD dwSentence;
				ImmGetConversionStatus(m_hIMC, &dwConversion, &dwSentence);
				if (m_dwConversion != dwConversion)
				{
					if (dwConversion & IME_CMODE_NATIVE)//한글윈도우이면 한글이 IME_CMODE_NATIVE
					{
						MessageBox(hWnd, L"한글", L"status", MB_OK);
					}
					else
					{
						//영문
						MessageBox(hWnd, L"영문", L"status", MB_OK);
					}
					m_dwConversion = dwConversion;
				}
			}
		}
		break;
		/*
		WM_IME_STARTCOMPOSITION; 처리에서 return; 문 대신 break;를 사용하게 되면 
		WM_IME_COMPOSITION(조합이 완료된 워드가 발생시)가추가적으로 발생하게 된다. 
		WM_IME_COMPOSITION 메시지와 WM_IME_COMPOSITION 메세지 처리 모두가 break;문으로 끝나게되면
		한글의 조합 완성 및 조합중일 때 WM_CHAR 메시지가 발생하게 된다. 
		이렇게 되면 한글과 영문 및 일반키을 구별하지 못하고 처리 해야 하기 때문에 불편하게 된다.
		*/
		case WM_IME_STARTCOMPOSITION:	//조합시작했다.
		{
			m_CompWord.clear();
		}
		return 1;
		case WM_IME_ENDCOMPOSITION:		//조합끝났다.
		{
			m_CompWord = wParam;
		}
		return 1;
		/*case WM_IME_CHAR:			//조합이 완료되면 호출된다.
		{
			m_CompString += wParam;
		}
		return 1;*/
		case WM_IME_COMPOSITION:		//조합중일때
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
		//반드시 return을 해야한다. break;을 하면 window가 처리한지 모르고 WM_CHAR메세지를 또 발생하여
		//밑의 default 문의 m_CompString += wParam;가 또 실행된다.
		//우리 라고 적으면 우우리가 된다.
		return 1;
		case WM_CHAR:					//영문일때
		{
			switch ((WCHAR)wParam)
			{
			case VK_BACK:				//백스페이스
			{
				if (!m_CompString.empty())
				{
					//마지막글자를 지운다.
					T_ITOR	it = m_CompString.begin() + (m_CompString.size() - 1);
					m_CompString.erase(it);
				}
			}
			break;
			case VK_RETURN:				//enter를 눌렀을때
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

