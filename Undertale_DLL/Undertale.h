#pragma once
#include <Windows.h>
#include <string>

#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class Undertale {
public:
	void   init();
	string getRoomPictureByName(string name);
	string getCurrentLocationID();
	int	   getCurrentRoute();
	string getKillCount(int targetArea);
	string getLOVE();
	string getAreaNameByLocationID(string locationID);
	string getLocationNameByLocationID(string locationID);
	string replacePlaceholders(string originalString);
	json   getGeneralConfigData(string key);
	json   getLanguageData(string key);
	string getDiscordRPDetails();
	string getDiscordRPState();
	string getDiscordRPLargeImageKey();
	string getDiscordRPSmallImageKey();
	string getDiscordRPLargeImageText();
	string getDiscordRPSmallImageText();
};
