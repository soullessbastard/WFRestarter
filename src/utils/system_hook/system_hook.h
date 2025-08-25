#pragma once
#include <windows.h>


extern HHOOK hHook;
extern DWORD g_mainThreadId;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
