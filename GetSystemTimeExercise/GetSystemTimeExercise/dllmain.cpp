#include "pch.h"
#include <Windows.h>
#include <detours.h>
#include <iostream>

//function pointer for actual time
static VOID(WINAPI* Real_GetSystemTime)(LPSYSTEMTIME lpSystemTime) = GetSystemTime;

//hooking function
VOID WINAPI Hooked_GetSystemTime(LPSYSTEMTIME lpSystemTime) {

 //Call the actual time function
    Real_GetSystemTime(lpSystemTime);

 //Modify the time
    lpSystemTime->wHour = (lpSystemTime->wHour + 1) % 24;

    std::cout << "Hooked GetSystemTime called!" << std::endl;
}


//hooking the dll
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {

    case DLL_PROCESS_ATTACH:
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)Real_GetSystemTime, Hooked_GetSystemTime);
        if (DetourTransactionCommit() != NO_ERROR) {
            std::cerr << "DetourTransactionCommit failed!" << std::endl;
        }

        break;

    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)Real_GetSystemTime, Hooked_GetSystemTime);
        DetourTransactionCommit();

        break;
    }

    return TRUE;
}
