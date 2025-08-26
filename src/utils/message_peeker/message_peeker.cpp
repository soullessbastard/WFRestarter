#include <windows.h>
#include <thread>
#include <atomic>

std::atomic<bool> running{ true };
bool localCopy = running.load(); 

void MessageLoopThread() {
    MSG msg;
    while (running.load()) {
        if (GetMessage(&msg, NULL, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            break;
        }
    }
}