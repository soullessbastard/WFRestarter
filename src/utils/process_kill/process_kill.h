#pragma once
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

std::vector<DWORD>get_process_ids();
void kill_process_by_id();
