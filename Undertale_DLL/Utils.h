#pragma once
#include <Windows.h>
#include <string>

#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class Utils {
public:
	string getExecutablePath();
	json getJSONDataFromFile(string filePath);
	uintptr_t getMemoryAddressValue(uintptr_t memory_address, vector<unsigned int> offsets = {});
	HANDLE getHProcess();
	string replace(std::string subject, const std::string& search, const std::string& replace);
};
