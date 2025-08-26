#include "settings_manager.h"
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <algorithm>
#include <thread>
#include "../../utils/config_gen/config_gen.h"
#include "../../utils/get_keyname/get_keyname.h"


// SettingsManager::SettingsManager(const std::wstring& path) : configPath(path){}


void SettingsManager::PreLoad(){
    if (!FileExists(configPath.c_str())) {
        std::wcout << L"Config file not found. Creating default config.\n";
        CreateDefaultConfig(configPath.c_str());
        SetInstallMethod();
        std::wcin.clear();
        
        SetHotkey();
        Save();
    }
}

void SettingsManager::Load() {
    wchar_t buffer[256];
    GetPrivateProfileStringW(L"Settings", L"install_method", L"", buffer, 256, configPath.c_str());
    
    installMethod = buffer;

    GetPrivateProfileStringW(L"Settings", L"hotkey", L"0", buffer, 256, configPath.c_str());
    hotkeyVK = _wtoi(buffer);
    hotkeyName = GetKeyName(hotkeyVK);
}

void SettingsManager::Save() {
    WritePrivateProfileStringW(L"Settings", L"install_method", installMethod.c_str(), configPath.c_str());

    wchar_t keyStr[16];
    swprintf_s(keyStr, L"%d", hotkeyVK);
    WritePrivateProfileStringW(L"Settings", L"hotkey", keyStr, configPath.c_str());
}

void SettingsManager::SetInstallMethod() {
    std::wstring input;
    system("cls");
    while (true) {
        std::wcout << L"Please specify installation method (steam/client):  ";
        std::getline(std::wcin, input);

        std::transform(input.begin(), input.end(), input.begin(), ::towlower);

        if (input == L"steam" || input == L"client") {
            installMethod = input;
            break;
        } else {
            std::wcout << L"Invalid input. Please try again.\n";
        }
    }
}


void SettingsManager::ReleaseWait(){
    while (true) {
        bool anyKeyPressed = false;
        for (int code = 1; code <= 254; ++code) {
            if (GetAsyncKeyState(code) & 0x8000) {
                anyKeyPressed = true;
                break;
            }
        }
        if (!anyKeyPressed) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void SettingsManager::SetHotkey() {
    system("cls");
    std::wcout << L"Press a key to assign as hotkey... ";

    ReleaseWait();
    
    int vk = 0;
    while (true) {
        for (int code = 1; code <= 254; ++code) {
            if (GetAsyncKeyState(code) & 0x8000) {
                vk = code;
                break;
            }
        }
        if (vk != 0) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    hotkeyVK = vk;
    std::wcout << L"\nA key with a VK code is assigned: " << hotkeyVK << L"\n";
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void SettingsManager::PrintCurrentSettings() {
    system("cls");

    std::wcout << L"\nCurrent settings:\n";
    std::wcout << L"=======================================\n";
    std::wcout << L"Installation method: " << installMethod << L"\n";

    hotkeyName = GetKeyName(hotkeyVK);


    std::wcout << L"Hotkey VK: " << hotkeyName << L"\n";
    std::wcout << L"=======================================\n";
    std::wcout << L"1. Exit\n";

    std::wstring input;
    while (true) {
        std::getline(std::wcin, input);

        if (input == L"1") {
            break;
        } else {
            std::wcout << L"Please enter a valid value: ";
        }
    }
}

int SettingsManager::GetHotkeyVK() const {
    return hotkeyVK;
}
std::wstring SettingsManager::GetHotkeyName() const {
    return hotkeyName;
}

std::wstring SettingsManager::GetInstallMethod() const {
    return installMethod;
}


void SettingsManager::ShowMenu() {
    while (true) {
        system("cls");

        std::wcout << L"\n--- Settings menu ---\n";
        std::wcout << L"1. Assign hotkey\n";
        std::wcout << L"2. Specify installation method\n";
        std::wcout << L"3. Show current settings\n";
        std::wcout << L"4. Start script\n";
        std::wcout << L"---------------------\n";
        std::wcout << L"\nSelect an item:\n ";

        //ReleaseWait();

        int choice = 0;

        std::wstring line;
        std::getline(std::wcin, line);
        if (line.empty()) {
            std::wcout << L"Input cannot be empty. Please enter a number.\n";
            continue;
        }
        // std::wcin.clear();
        // std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');

        // std::wcin >> choice;
        // std::wcin.ignore();
        try{
            choice = std::stoi(line);
            switch (choice) {
            case 1:
                SetHotkey();
                break;
            case 2:
                SetInstallMethod();
                break;
            case 3:
                PrintCurrentSettings();
                break;
            case 4:
                Save();
                return;
            default:
                std::wcout << L"Wrong choice.\n";
                break;
            }
        }catch (...) {
            std::wcout << L"Invalid input, please enter a number.\n";
            continue;
        }
    }
}