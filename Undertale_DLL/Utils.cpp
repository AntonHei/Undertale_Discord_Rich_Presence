#include "Utils.h"

#include <fstream>
#include <iostream>
#include <windows.h>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

using namespace std;

std::string Utils::getExecutablePath() {

    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string pathString = std::string(buffer).substr(0, pos);

    return pathString;
}

HANDLE Utils::getHProcess() {

    DWORD procId = GetCurrentProcessId();
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, procId);

    return hProcess;
}

uintptr_t Utils::getMemoryAddressValue(uintptr_t memory_address, vector<unsigned int> offsets) {

    HANDLE hProcess = getHProcess();

    HANDLE handle = GetModuleHandleA(0);
    uintptr_t moduleBase = (uintptr_t)handle;

    uintptr_t address = moduleBase + memory_address;

    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        // Read and proceed
        ReadProcessMemory(hProcess, (BYTE*)address, &address, sizeof(address), 0);
        address += offsets[i];
    }

    return address;
}

json Utils::getJSONDataFromFile(string filePath) {

    std::string fullExePath = getExecutablePath();
    string jsonFilePath = fullExePath + "/data/" + filePath;
    std::ifstream ifs(jsonFilePath);

    json jsonData;
    try
    {
        jsonData = json::parse(ifs);
    }
    catch (nlohmann::detail::parse_error& ex)
    {
        cout << "JSON Parse failed: " << jsonFilePath << endl;
    }

    return jsonData;
}

string Utils::replace(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}
