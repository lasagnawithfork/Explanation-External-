#  ModuleBase for now the very beggining of what we need to learn 


Roblox externals as of now are super easy to make here's a list of how to make a simple esp just follow the tutorial;

---
### GET MODULE BASE 

Every `.exe` has a base address, basically where the program starts in memory. Think of it like a house: the **PID is the house number**, and the **module base is the address where the house is located**.

Our `GetModuleBase` function is what finds that address, but we’ll use it later. First, we need `GetProcessID` to find the `.exe` we want and give us its PID. So basically, we find the house, get its house number, and then use that to find the address of the house.


---


```c++ 
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

uintptr_t GetModuleBase(DWORD processId, const wchar_t* moduleName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
        processId
    );

    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    MODULEENTRY32W module{};
    module.dwSize = sizeof(module);

    if (Module32FirstW(snapshot, &module))
    {
        do
        {
            if (_wcsicmp(module.szModule, moduleName) == 0)
            {
                CloseHandle(snapshot);
                return reinterpret_cast<uintptr_t>(module.modBaseAddr);
            }
        } while (Module32NextW(snapshot, &module));
    }

    CloseHandle(snapshot);
    return 0;

```

---
### GET PROCESS ID

After making `GetModuleBase`, we can move on to `GetProcessID` like we talked about. Keep in mind that both of these functions are going in the same `.cpp` file for now. We’ll worry about splitting things into `.hpp` header files later.

So, in `Module.cpp`:


---

```c++
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
```

---
### The header file

Here we make the header file after making the `.cpp` file. Think of it like connecting it to `main.cpp`. We're basically telling `main.cpp`:

"Hey, these functions exist, you can use them here."

Then we can include the header in our other `.cpp` files and use the functions.


---

```c++
#pragma once

#include <Windows.h>
#include <cstdint>

DWORD GetProcessID(const wchar_t* processName);

uintptr_t GetModuleBase(
    DWORD processId,
    const wchar_t* moduleName
);
```

---
### The MAIN file

Now we go back to the `main.cpp` file. If you don't have one, create a new C++ file called `main.cpp`, and write exactly this:


---

```c++
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
```

---
### The startup

We build the project and check if anything works If you get any issues like errors, check whether your project is using the right character set since Unicode settings can sometimes cause issues. Also, make sure youre using a recent C++ standard, like ``C++20`` or newer

Once you're done, make sure the process is running (Roblox) which we wrote (RobloxPlayerBeta.exe) is running,  launch the program, and watch it find the base address.


---
