#pragma once
#include <string>

class SettingsValidator {
public:
    static bool ValidateInstallMethod(const std::wstring& method);
    static bool ValidateHotkey(int hotkeyVK);
};

