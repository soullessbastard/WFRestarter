#pragma once
#include <string>

bool FileExists(const wchar_t* filename);
void CreateDefaultConfig(const wchar_t* filename);
void CreateEmptyFileIfNotExists(const wchar_t* filename);
std::wstring GetExecutableDir();