// Thread_0.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Thread_0.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND hWnd;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THREAD0, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THREAD0));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THREAD0));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THREAD0);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//


//이제까지 사용한 스레드는 메인스레드 1개였음.
//메인스레드는 WinMain함수가 메인함수. 
//메인함수가 끝나면 모든스레드가 종료되고 응용프로그램이 종료된다.
//새로운 스레드1명을 고용, 작업자 고용.
//새로운 스레드는 threadFunc1()함수만 돈다.
//스레드 시작함수의 약속은 DWORD 리턴값이고 WINAPI가 붙는다.
//새로운 스레드 함수가 닫혀도 메인스레드는 종료되지 않는다.

//중요:스레드만 여러개인거지 동시에 작업하는건 아니다.
//     시간을 엄청 작게 분할(퀀텀 단위로 분할해서 = 0.003초)해서 스레드가 교차적으로 작업하는것이다.
//스레드 컨텍스트 : a스레드가 작업하다가 스레드 스위칭되서 b스레드로 넘어갔을경우에,
//					작업한 중간 정보를 저장하고 이후로 작업한다.
//스레드 스위칭 :	a스레드가 작업하다가 b스레드가 작업하는것.
//					스위칭하는 이유는 한 스레드가 독점하는것을 막기 위해서이다.
//스레드 스위칭은 OS가 관할한다. 스레드 스케줄링은 OS가 한다.
//스레드의 장점 : 스레드를 사용하면 블록형함수가 해결된다.
//스레드의 단점 : 스레드를 사용하면 브레이크 포인트가 여러 개일 경우에, 스레드 스위칭에 의해, 
//                다른 스레드가 브레이크 포인터 걸릴수있으므로, 디버깅이 어려워진다.
//임계영역 : 전역변수이거나 스태틱변수일경우, A스레드와 B스레드가 동시에 메모리를 수정할때,
//			임계영역으로 설정한구간을 수행시에는 컨텍스트 스위칭이 발생하지 않도록 해준다.
//			ex) A스레드에서 벡터의 유저리스트를 for문으로 돌고있을때 스위칭 발생. B스레드에서 유저리스트 삭제시.
//			A스레드 벡터의 유저리스트의 갯수가 변경되어 A스레드는 죽게된다. 그걸 방지하고자 할때 임계영역을 설정한다.
//Mutex, Semaphore, Critical Section...
//int g_iSum = 0;
int g_iX = 0;
DWORD WINAPI threadFunc1(LPVOID param)
{
	HDC hdc = GetDC(hWnd);
	while (1)
	{
		//g_iSum++;
		g_iX = 100;
		//Sleep(1);
		TextOut(hdc, g_iX, 100, L"KGCA1", 5);
		
		/*if (g_iSum > 1000000)
		{
			break;
		}*/
	}
	TextOut(hdc, 100, 100, L"out", 4);
	ReleaseDC(hWnd, hdc);
	return 0;
}

DWORD WINAPI threadFunc2(LPVOID param)
{
	HDC hdc = GetDC(hWnd);
	while (1)
	{
		//g_iSum++;
		g_iX = 200;
		//Sleep(1);
		TextOut(hdc, g_iX, 200, L"KGCA2", 5);
		
		/*if (g_iSum > 1000000)
		{
			break;
		}*/
	}

	TextOut(hdc, g_iX, 100, L"out", 4);
	ReleaseDC(hWnd, hdc);
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_LBUTTONDOWN:
	{
		DWORD threadID;
		HANDLE hThread = CreateThread(NULL, 0, threadFunc1, NULL, 0, &threadID);
		CloseHandle(hThread);

		DWORD threadID2;
		HANDLE hThread2 = CreateThread(NULL, 0, threadFunc2, NULL, 0, &threadID2);
		CloseHandle(hThread2);
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			//TextOut(hdc, 200, 200, L"GAME", 4);

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
