#include "config_gen.h"


std::wstring GetExecutableDir() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);

    std::wstring fullPath(path);
    size_t pos = fullPath.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return fullPath.substr(0, pos); 
    }
    return L".";
}


bool FileExists(const wchar_t* filename) {
    DWORD attrib = GetFileAttributesW(filename);
    return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CreateDefaultConfig(const wchar_t* filename) {

    WritePrivateProfileStringW(L"Settings", L"install_method", L"", filename);
}

void CreateEmptyFileIfNotExists(const wchar_t* filename) {
    DWORD attrib = GetFileAttributesW(filename);
    if (attrib == INVALID_FILE_ATTRIBUTES) {
        HANDLE hFile = CreateFileW(
            filename,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
        }
    }
}