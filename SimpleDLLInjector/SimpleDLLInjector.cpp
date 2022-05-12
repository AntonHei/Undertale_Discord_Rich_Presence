#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

DWORD GetProcId(const char* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                cout << "Checking: " << procEntry.szExeFile << endl;
                char output[255];
                // Needed for 32bit Execution:
                const WCHAR* wc = procEntry.szExeFile;
                sprintf(output, "%ws", procEntry.szExeFile);

                if (!_stricmp(output, procName))
                {
                    procId = procEntry.th32ProcessID;

                    cout << "Found target Process: " << procEntry.szExeFile << "; Process-ID: " << procId << endl;

                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;
}

int main()
{
    // Hide! Fast!
    FreeConsole();

    DWORD pid;
    HANDLE hProcess;
    LPVOID lpBaseAddress;

    string curPath = fs::current_path().u8string();
    string undertalePath = curPath + "\\UNDERTALE.exe";

    string cmd = "explorer " + undertalePath;
    system(cmd.c_str());

    string dllPath = curPath + "\\Undertale_DLL.dll";
    const char* procName = "UNDERTALE.exe";

    Sleep(10000);

    pid = GetProcId(procName);
    cout << "Target Process-ID: " << pid << endl;

    size_t sz = strlen(dllPath.c_str()) + 1;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    lpBaseAddress = VirtualAllocEx(hProcess, 0, sz, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    WriteProcessMemory(hProcess, lpBaseAddress, dllPath.c_str(), sz, NULL);

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), lpBaseAddress, 0, 0);

    WaitForSingleObject(hThread, INFINITE);

    if (hThread)
    {
        cout << "Injection succesful" << endl;
        cout << "Dll path allocated at: " << hex << lpBaseAddress << endl;

        VirtualFreeEx(hProcess, lpBaseAddress, strlen(dllPath.c_str()), MEM_RELEASE);
    }
    else
    {
        cout << "Injection unsuccesful" << endl;
    }

    CloseHandle(hThread);
    CloseHandle(hProcess);
}