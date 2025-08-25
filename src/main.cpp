// DEFAULT IMPORTS

#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <cstdlib>
#include <algorithm>


// UTILS

#include "utils/system_hook/system_hook.h"
#include "utils/process_kill/process_kill.h"
#include "utils/config_gen/config_gen.h"

// GLOBAL INIT

HHOOK hHook = nullptr;
DWORD g_mainThreadId = GetCurrentThreadId();



int Run() {
    AllocConsole(); 
    SetConsoleTitle("WFRestarter");
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    SetConsoleOutputCP(65001);
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);

    HWND hwndConsole = GetConsoleWindow();
    if (hwndConsole != nullptr) {
        ShowWindow(hwndConsole, SW_SHOW);
        SetForegroundWindow(hwndConsole);
    }

    char* localappdata = nullptr;
    size_t len = 0;
    const wchar_t* configFile = L"config.ini";
    std::wstring configPath = GetExecutableDir() + L"\\" + configFile;
    std::wstring method;
    std::wstring logpath;
    std::wstring steampath;


    if (!FileExists(configFile)) {
        std::wcout << L"Config file not found. Creating default config.\n";

        CreateDefaultConfig(configPath.c_str());
                
        while (true) {
            std::wcout << L"Please specify installation method (steam/client): ";
            std::getline(std::wcin, method);
        
            method.erase(std::remove(method.begin(), method.end(), L'\r'), method.end());
            method.erase(std::remove(method.begin(), method.end(), L'\n'), method.end());
        
            std::wstring method_lower = method;
            std::transform(method_lower.begin(), method_lower.end(), method_lower.begin(), ::towlower);
        
            if (method_lower == L"steam" || method_lower == L"client") {
                WritePrivateProfileStringW(L"Settings", L"install_method", method_lower.c_str(), configPath.c_str());
                std::wcout << L"Installation method saved in config.ini\n";
                break;
            } else {
                std::wcout << L"Invalid input. Please enter 'steam' or 'client'.\n";
            }
        }
    }

    if (FileExists(configFile)) {
        wchar_t buffer[256];
        GetPrivateProfileStringW(L"Settings", L"install_method", L"", buffer, sizeof(buffer) / sizeof(wchar_t), configPath.c_str());

        method = std::wstring(buffer);

        if (method != L"steam" && method != L"client") {
            if (DeleteFileW(configFile)) {
                std::wcout << L"Invalid installation method. Configuration file has been removed.\n";
            } 
            else {
                std::wcerr << L"Failed to delete configuration file!\n";
            }
            return 1;

        }
        std::wcout << L"Installation method loaded: " << buffer << L"\n";
    }


    if (method == L"client"){
        wchar_t* localappdata = nullptr;
        size_t len = 0;
        errno_t err = _wdupenv_s(&localappdata, &len, L"LOCALAPPDATA");
        if (err || localappdata == nullptr) {
            std::wcerr << L"Failed to get LOCALAPPDATA\n";
        } else {
            std::wcout << L"LOCALAPPDATA = " << localappdata << "\n";
            logpath = std::wstring(localappdata) + L"\\Warframe\\Downloaded\\Public\\Tools\\Launcher.exe";
            free(localappdata);
        }
    }
    else if (method == L"steam"){
        steampath = L"steam://rungameid/230410";
    }
    


    std::wcout << L"Waiting for Home key press for closing Warframe...\n";

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
    if (!hHook) {
        std::wcerr << L"Failed to install keyboard hook\n";
        return 1;
    }

    MSG msg;
    bool running = true;

// while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    // TranslateMessage(&msg);
    // DispatchMessage(&msg);

//}
    while (running) {
        while (GetMessage(&msg, nullptr, 0, 0)) {
            if (msg.message == WM_USER + 1) {
                std::wcout << L"Home key pressed! Closing Warframe...\n";
                kill_process_by_id();
                
                if (!method.empty()){
                    if(method == L"steam"){
                        ShellExecuteW(
                            NULL,
                            L"open",
                            steampath.c_str(),
                            NULL,
                            NULL,
                            SW_SHOWNORMAL
                        );
                    }
                    else if(method == L"client"){
                        ShellExecuteW(
                            NULL,
                            L"open",
                            logpath.c_str(),
                            NULL,
                            NULL,
                            SW_SHOWNORMAL
                        );
                    }
                } 
            }
        }
        running = false;
        break;
    }

    UnhookWindowsHookEx(hHook);

    std::wcout << L"Closing app...\n";
    
    for (int i = 3; i > 0; --i) {
        std::wcout << L"\rClosing app in: " << i << L"   "; 
        std::wcout.flush();
        Sleep(1000);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return Run();
}