#include "Undertale.h"

#include <fstream>
#include <iostream>

#include "Utils.h"

using namespace std;

Utils* g_Utils;

// Runtime Variables
json generalConfig;
json languageJSONData;
json locationMappingjsonData;
json discordDatajsonData;

void Undertale::init() {
    generalConfig = g_Utils->getJSONDataFromFile("general.json");

    // Path Traversal: You're welcome
    string langFilePath = string("lang/") + string(generalConfig["language"]) + string(".json");
    languageJSONData = g_Utils->getJSONDataFromFile(langFilePath);

    string locationMappingFilePath = string("lang/locations/") + string(generalConfig["language"]) + string(".json");
    locationMappingjsonData = g_Utils->getJSONDataFromFile(locationMappingFilePath);

    string discordDataFilePath = "discord_data.json";
    discordDatajsonData = g_Utils->getJSONDataFromFile(discordDataFilePath);
}

// Rich Presence specific

string Undertale::getDiscordRPDetails() {

    string details = getLanguageData("details");
    details = replacePlaceholders(details);

    return details;

}

string Undertale::getDiscordRPState() {

    string state = getLanguageData("state");
    state = replacePlaceholders(state);

    return state;

}

string Undertale::getDiscordRPLargeImageKey() {

    string locationID = getCurrentLocationID();
    string areaName = getAreaNameByLocationID(locationID);

    string largeImageKey = getRoomPictureByName(areaName);

    return largeImageKey;
}

string Undertale::getDiscordRPLargeImageText() {

    string locationID = getCurrentLocationID();
    string areaName = getAreaNameByLocationID(locationID);

    return areaName;
}

string Undertale::getDiscordRPSmallImageKey() {

    int route = getCurrentRoute();

    string routeString = "";

    switch (route) {
        case -1:
            routeString = "pacifist";
            break;

        case 0:
            routeString = "neutral";
            break;

        case 1:
            routeString = "genocide";
            break;
    }

    string smallImageKey = "";

    if (routeString != "") {

        if (discordDatajsonData["routeDiscordPicNames"][routeString] != NULL) {
            smallImageKey = discordDatajsonData["routeDiscordPicNames"][routeString];
        }

    }

    return smallImageKey;
}

string Undertale::getDiscordRPSmallImageText() {

    string smallImageKey = "";
    int route = getCurrentRoute();

    switch (route) {

        case -1:
            smallImageKey = getLanguageData("routes")["pacifist"];
            break;

        case 0:
            smallImageKey = getLanguageData("routes")["neutral"];
            break;

        case 1:
            smallImageKey = getLanguageData("routes")["genocide"];
            break;
    }

    return smallImageKey;
}

string Undertale::replacePlaceholders(string originalString) {

    json placeholders = {
        {"{LV}",                getLOVE()},
        {"{Kills_Global}",      getKillCount(0)},
        {"{Kills_Ruins}",       getKillCount(1)},
        {"{Kills_Snowdin}",     getKillCount(2)},
        {"{Kills_Waterfall}",   getKillCount(3)},
        {"{Kills_HotlandCore}", getKillCount(4)},
        {"{Location_ID}",       getCurrentLocationID()},
        {"{Location_Name}",     getLocationNameByLocationID(getCurrentLocationID())},
        {"{Area_Name}",         getAreaNameByLocationID(getCurrentLocationID())},
    };

    string newString = originalString;

    for (auto& placeholder : placeholders.items()) {
        newString = g_Utils->replace(newString, placeholder.key(), placeholder.value());
    }

    return newString;
}

json Undertale::getLanguageData(string key) {
    if (languageJSONData[key] != NULL) {
        return languageJSONData[key];
    }
    return "{" + key + "}";
}

json Undertale::getGeneralConfigData(string key) {
    if (generalConfig[key] != NULL) {
        return generalConfig[key];
    }
    return "{" + key + "}";
}

// Game specific

string Undertale::getCurrentLocationID() {

    uintptr_t address = g_Utils->getMemoryAddressValue(0x618EA0);

    // Get final value
    HANDLE hProcess = g_Utils->getHProcess();
    DWORD value;
    ReadProcessMemory(hProcess, (BYTE*)address, &value, sizeof(value), nullptr);

    string valueString = to_string(value);

    return valueString;
}

int Undertale::getCurrentRoute() {

    // Routes: -1: Pacifist; 0: Neutral; 1: Genocide
    int curRoute = -2;

    string locationID = getCurrentLocationID();
    string areaName = getAreaNameByLocationID(locationID);

    int kills_ruins = stoi(getKillCount(1));
    int kills_snowdin = stoi(getKillCount(2));
    int kills_waterfall = stoi(getKillCount(3));
    int kills_hotlandcore = stoi(getKillCount(4));

    int genocide_kills_ruins = 20;
    int genocide_kills_snowdin = 16;
    int genocide_kills_waterfall = 18;
    int genocide_kills_hotlandcore = 40;

    // Sadly can't switch natively on strings, so if's it is

    if (areaName == "Ruins") {
        // Genocide
        if (kills_ruins >= genocide_kills_ruins) {
            curRoute = 1;
        }
    }

    if (areaName == "Snowdin") {
        // Genocide
        if (kills_ruins >= genocide_kills_ruins) {
            curRoute = 1;
        }

        //Pacifist
        if (kills_ruins <= 0) {
            curRoute = -1;
        }
    }

    if (areaName == "Waterfall") {
        // Genocide
        if (kills_ruins >= genocide_kills_ruins && kills_snowdin >= genocide_kills_snowdin) {
            curRoute = 1;
        }

        //Pacifist
        if (kills_ruins <= 0 && kills_snowdin <= 0) {
            curRoute = -1;
        }
    }

    if (areaName == "Hotland" || areaName == "CORE") {
        // Genocide
        if (kills_ruins >= genocide_kills_ruins && kills_snowdin >= genocide_kills_snowdin && kills_waterfall >= genocide_kills_waterfall) {
            curRoute = 1;
        }

        //Pacifist
        if (kills_ruins <= 0 && kills_snowdin <= 0 && kills_waterfall <= 0) {
            curRoute = -1;
        }
    }

    if (areaName == "End") {
        // Genocide
        if (kills_ruins >= genocide_kills_ruins && kills_snowdin >= genocide_kills_snowdin && kills_waterfall >= genocide_kills_waterfall && kills_hotlandcore >= genocide_kills_hotlandcore) {
            curRoute = 1;
        }

        //Pacifist
        if (kills_ruins <= 0 && kills_snowdin <= 0 && kills_waterfall <= 0 && kills_hotlandcore <= 0) {
            curRoute = -1;
        }
    }

    return curRoute;
}

string Undertale::getKillCount(int targetArea) {

    uintptr_t baseAddress = 0x0;
    vector<unsigned int> offsets = {};

    switch (targetArea) {

        // Global
        case 0:
            baseAddress = 0x0040894C;
            offsets = {0x44, 0x10, 0x97C, 0x0};
            break;

        // Ruins
        case 1:
            baseAddress = 0x0040894C;
            offsets = {0x44, 0x10, 0x544, 0x20, 0x44, 0xC, 0xCA0};
            break;

        // Snowdin
        case 2:
            baseAddress = 0x0040894C;
            offsets = {0x44, 0x10, 0x544, 0x20, 0x44, 0xC, 0xCB0};
            break;

        // Waterfall
        case 3:
            baseAddress = 0x0040894C;
            offsets = {0x44, 0x10, 0x544, 0x20, 0x44, 0xC, 0xCC0};
            break;

        // HotlandCore
        case 4:
            baseAddress = 0x0040894C;
            offsets = {0x44, 0x10, 0x544, 0x20, 0x44, 0xC, 0xCD0};
            break;

        // Fallback
        default:
            return "0";
            break;

    }

    uintptr_t address = g_Utils->getMemoryAddressValue(baseAddress, offsets);

    // Get final value
    HANDLE hProcess = g_Utils->getHProcess();
    double value;
    ReadProcessMemory(hProcess, (BYTE*)address, &value, sizeof(value), nullptr);

    string valueString = to_string((int)value);

    return valueString;
}

string Undertale::getLOVE() {

    uintptr_t address = g_Utils->getMemoryAddressValue(0x003F9F44, {0x0, 0x44, 0x10, 0x364, 0x3E0});

    // Get final value
    HANDLE hProcess = g_Utils->getHProcess();
    double value;
    ReadProcessMemory(hProcess, (BYTE*)address, &value, sizeof(value), nullptr);

    string valueString = to_string((int)value);

    return valueString;
}

string Undertale::getRoomPictureByName(string areaName) {

    string roomPictureName = "";

    if (discordDatajsonData["areaDiscordAppPicNames"][areaName] != NULL) {
        roomPictureName = discordDatajsonData["areaDiscordAppPicNames"][areaName];
    } else {
        roomPictureName = getLanguageData("unknown");
    }

    return roomPictureName;
}

string Undertale::getAreaNameByLocationID(string locationID) {

    string areaName = "";

    if (locationMappingjsonData[locationID] != NULL) {
        areaName = locationMappingjsonData[locationID]["Area"];
    } else {
        areaName = getLanguageData("unknown");
    }

    return areaName;
}

string Undertale::getLocationNameByLocationID(string locationID) {

    string locationName = "";

    if (locationMappingjsonData[locationID] != NULL) {
        locationName = locationMappingjsonData[locationID]["Name"];
    } else {
        locationName = getLanguageData("unknown");
    }

    return locationName;
}
