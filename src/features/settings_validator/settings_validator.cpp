#include "settings_validator.h"
#include <windows.h>

bool SettingsValidator::ValidateInstallMethod(const std::wstring& method) {
    return (method == L"steam" || method == L"client");
}

bool SettingsValidator::ValidateHotkey(int hotkeyVK) {

    if (hotkeyVK < 8)
        return false;

    if (hotkeyVK > 190)
        return false;

    return true;
}