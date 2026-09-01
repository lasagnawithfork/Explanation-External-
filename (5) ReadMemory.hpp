#pragma once

#include <windows.h>
#include <cstdint>

class memoryt
{
private:
    HANDLE hProcess = nullptr;

public:
    memoryt(HANDLE process)
        : hProcess(process)
    {
    }

    template<typename T>
    T Read(uintptr_t address)
    {
        T value{};
        ReadProcessMemory(
            hProcess,
            reinterpret_cast<LPCVOID>(address),
            &value,
            sizeof(T),
            nullptr
        );
        return value;
    }

    template<typename T>
    bool Write(uintptr_t address, const T& value)
    {
        return WriteProcessMemory(
            hProcess,
            reinterpret_cast<LPVOID>(address),
            &value,
            sizeof(T),
            nullptr
        );
    }
};
