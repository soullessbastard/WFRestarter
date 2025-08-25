#include "system_hook.h"

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN) {
            if (p->vkCode == VK_HOME) {
                PostThreadMessage(g_mainThreadId, WM_USER + 1, 0, 0);
            }

            if (p->vkCode == VK_END) {
                PostQuitMessage(0);
            }
        }
    } 
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}