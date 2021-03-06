#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>

#include <iostream>
#include <map>
#include <string>
#include <list>

//프로젝트 속성에서 추가 종속성에 넣을걸 코드로 넣을수 있다.
#pragma comment(lib, "winmm.lib")

typedef std::basic_string <char>	C_STR;	//multi-byte
typedef std::basic_string <wchar_t>	W_STR;	//unicode byte
typedef std::basic_string <TCHAR>	T_STR;	//프로젝트속성이 멀티바이트이면 멀티바이트, 유니코드이면 유니코드로 바뀜.

//string a;		//multi-byte
//wstring b;	//unicode byte

#define GAMESTART int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) {
//
////L#s ==> L"" s는 문자열이므로 #을 붙인다. 유니코드라서 L을 붙인다.
//#define GAMERUN(s) TWindow win; win.SetWindow(hInstance, L#s, 600, 800); win.Run();
//
#define GAME_END }


//L#s ==> L"" s는 문자열이므로 #을 붙인다. 유니코드라서 L을 붙인다.
#define GAMERUN(s, w, h) int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) { Sample win; win.SetWindow(hInst, L#s, w, h); win.Run(); }

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern float g_fSecPerFrame;
extern HDC g_hOffScreenDC;

template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingletonInstance;
		return theSingletonInstance;
	}
};