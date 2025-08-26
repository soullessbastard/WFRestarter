#pragma once
#include <string>

class SettingsManager {
    public:
        SettingsManager(const std::wstring& path) : configPath(path) {}

        void PreLoad();
        void Load();
        void Save();
    
        void ShowMenu();
        void SetInstallMethod();
        void SetHotkey();
        void PrintCurrentSettings();
        void ReleaseWait();
    
        int GetHotkeyVK() const;
        std::wstring GetInstallMethod() const;
        std::wstring GetHotkeyName() const;
    
    private:
        std::wstring configPath;
        std::wstring installMethod;
        int hotkeyVK;
        std::wstring hotkeyName;
};