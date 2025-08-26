#include "get_keyname.h"

std::wstring GetKeyName(UINT vk) {
    UINT scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC) << 16;

    if (vk == VK_LEFT || vk == VK_RIGHT || vk == VK_UP || vk == VK_DOWN ||
        vk == VK_HOME || vk == VK_END || vk == VK_INSERT || vk == VK_DELETE ||
        vk == VK_PRIOR || vk == VK_NEXT) {
        scanCode |= 0x01000000;
    }

    wchar_t keyName[128];
    if (GetKeyNameTextW(scanCode, keyName, 128) > 0) {
        return keyName;
    }

    return L"Unknown";
}