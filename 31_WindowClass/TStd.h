#pragma once

#include <windows.h>
#include <assert.h>

#define GAMESTART int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) {
//
////L#s ==> L"" s는 문자열이므로 #을 붙인다. 유니코드라서 L을 붙인다.
//#define GAMERUN(s) TWindow win; win.SetWindow(hInstance, L#s, 600, 800); win.Run();
//
#define GAME_END }


//L#s ==> L"" s는 문자열이므로 #을 붙인다. 유니코드라서 L을 붙인다.
#define GAMERUN(s, w, h) int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) { TWindow win; win.Set(WS_POPUPWINDOW); win.SetWindow(hInst, L#s, w, h); win.Run(); }
