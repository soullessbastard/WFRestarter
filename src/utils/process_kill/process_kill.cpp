#include "process_kill.h"


std::vector<DWORD> get_process_ids_by_names(const std::vector<std::wstring>& names){
    std::vector<DWORD> pids;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to create snapshot\n";
        return pids;
    }

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(entry);

    if (Process32FirstW(snapshot, &entry)) {
        do {
            for (const auto& name : names) {
                if (name == entry.szExeFile) {
                    pids.push_back(entry.th32ProcessID);
                    break;
                }
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pids;
}

void kill_process_by_id() {
    const std::vector<std::wstring> processNames = {L"Warframe.x64.exe"};

    std::vector<DWORD> processIds = get_process_ids_by_names(processNames);

    if (processIds.empty()) {
        std::wcout << L"Processes not found or not running.\n";
        return;
    }

    for (DWORD pid : processIds){

        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
        if (hProcess == NULL) {
            std::wcerr << L"Failed to open process " << pid << "\n";
            return;
        }

        if (!TerminateProcess(hProcess, 1)) {
            std::wcerr << L"Failed to terminate process " << pid << "\n";
        } else {
            std::wcout << L"Process " << pid << " terminated successfully\n";
        }

        CloseHandle(hProcess);
    }
}
