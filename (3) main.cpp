#include <iostream>
#include "Module.hpp"

int main()
{
    const wchar_t* processName = L"RobloxPlayerBeta.exe";

    DWORD pid = GetProcessID(processName);

    if (!pid)
    {
        std::wcout << L"Process not found.\n";
        return 1;
    }

    uintptr_t base = GetModuleBase(pid, processName);

    if (!base)
    {
        std::wcout << L"Module not found.\n";
        return 1;
    }

    std::wcout << L"PID: " << pid << L'\n';
    std::wcout << L"Base Address: 0x"
               << std::hex << base << L'\n';

    return 0;
}
