#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

DWORD GetProcessID(const wchar_t* processName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);

    if (Process32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(entry.szExeFile, processName) == 0)
            {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}

uintptr_t GetModuleBase(DWORD processId, const wchar_t* moduleName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
        processId
    );

    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    MODULEENTRY32W entry{};
    entry.dwSize = sizeof(entry);

    if (Module32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(entry.szModule, moduleName) == 0)
            {
                CloseHandle(snapshot);
                return reinterpret_cast<uintptr_t>(entry.modBaseAddr);
            }
        } while (Module32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}
