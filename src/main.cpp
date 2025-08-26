// DEFAULT IMPORTS

#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <cstdlib>
#include <algorithm>
#include <future>
#include <chrono>

// UTILS

#include "utils/system_hook/system_hook.h"
#include "utils/process_kill/process_kill.h"
#include "utils/config_gen/config_gen.h"
#include "utils/message_peeker/message_peeker.h"

// FEATURES

#include "features/settings_manager/settings_manager.h"
#include "features/settings_validator/settings_validator.h"


// GLOBAL VAR INIT

HHOOK hHook = nullptr;
DWORD g_mainThreadId = GetCurrentThreadId();
int g_hotkeyVK;


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
    std::wstring hotkeyname;

    SettingsManager settings(configPath);
    SettingsValidator validator;

    std::thread msgThread(MessageLoopThread);

    settings.PreLoad();
    settings.Load();
    settings.ShowMenu();

    system("cls");

    if (FileExists(configFile)) {
        wchar_t buffer[256];
        // GetPrivateProfileStringW(L"Settings", L"install_method", L"", buffer, sizeof(buffer) / sizeof(wchar_t), configPath.c_str());

        method = settings.GetInstallMethod();
        g_hotkeyVK = settings.GetHotkeyVK();
        hotkeyname = settings.GetHotkeyName();


        std::wcout << L"Installation Method: " << method << L"\nHotkey: " <<  hotkeyname << "\n";

        if (!validator.ValidateInstallMethod(method) or !validator.ValidateHotkey(g_hotkeyVK)) {
            if (DeleteFileW(configFile)) {
                std::wcout << L"Invalid installation method. Configuration file has been removed.\n";
            } 
            else {
                std::wcerr << L"Failed to delete configuration file!\n";
            }
            return 1;

        }
    }
    else{
        return 1;
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
    


    std::wcout << L"Waiting for " << hotkeyname <<" key press for closing Warframe...\n";

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
    if (!hHook) {
        std::wcerr << L"Failed to install keyboard hook\n";
        return 1;
    }

    MSG msg;
    bool running = true;

    while (running) {
        while (GetMessage(&msg, nullptr, 0, 0)) {
            if (msg.message == WM_USER + 1) {
                std::wcout << hotkeyname << L" key pressed! Closing Warframe...\n";
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
    
    
    for (int i = 3; i > 0; --i) {
        std::wcout << L"\rClosing app in: " << i;
        std::wcout.flush();
        Sleep(1000);
    }

    return 0;
}

// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//     return Run();
// }


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    HANDLE hMutex = CreateMutex(NULL, FALSE, "WFRestarter_CustomMutex");

    if (GetLastError() == ERROR_ALREADY_EXISTS) {

        HWND hwndExisting = FindWindow(NULL, "WFRestarter.exe");

        if (hwndExisting) {
            ShowWindow(hwndExisting, SW_RESTORE);
            SetForegroundWindow(hwndExisting);
        }
        return 0;
    }

    int ret = Run();

    if (hMutex) {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

    return ret;
}