#pragma once
#include <vector>
#include <windows.h>

std::vector<DWORD>get_process_ids();
void kill_process_by_id();
