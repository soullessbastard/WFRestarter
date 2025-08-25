#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>

bool FileExists(const wchar_t* filename);
void CreateDefaultConfig(const wchar_t* filename);
void CreateEmptyFileIfNotExists(const wchar_t* filename);
std::wstring GetExecutableDir();