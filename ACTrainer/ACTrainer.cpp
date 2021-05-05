#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"


int main()
{
    //Get Process ID of Assault Cube
    DWORD procId = GetProcId(L"ac_client.exe");

    //Get the Module's Base Adress
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    //Get a Handle to the Process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    //Resolve the Base Adress of Pointer Chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10f4f4;

    std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //Resolve the Ammo Pointer Chain
    std::vector<unsigned int> ammoOffsets = {0x374, 0x14, 0x0};
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;
    //Print Ammo Value
    int ammoValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;
    //Write to Ammo Value
    int newAmmo = 1337;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    //Read out new Ammo Value
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

    std::cout << "New Ammo = " << std::dec << ammoValue << std::endl;

    getchar();
    return 0;
}
