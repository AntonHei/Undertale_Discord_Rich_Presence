#include "discordIntegration.h"
#include "Undertale.h"

#include <iostream>
#include <Windows.h>

using namespace std;

Undertale * g_Undertale;

// Runtime Data
time_t discordrp_time_start;

void discordIntegration::init() {
	cout << "Discord Integration started" << endl;

	g_Undertale->init();

	DiscordEventHandlers Handler;
	memset(&Handler, 0, sizeof(Handler));
	string discordAppId = g_Undertale->getGeneralConfigData("discord_app_id");
	Discord_Initialize(discordAppId.c_str(), &Handler, 1, NULL);

	discordrp_time_start = time(0);
}

void discordIntegration::update() {
	//cout << "Discord Integration updated" << endl;

	string details = g_Undertale->getDiscordRPDetails();
	string state = g_Undertale->getDiscordRPState();

	string largeImageKey = g_Undertale->getDiscordRPLargeImageKey();
	string smallImageKey = g_Undertale->getDiscordRPSmallImageKey();

	string largeImageText = g_Undertale->getDiscordRPLargeImageText();
	string smallImageText = g_Undertale->getDiscordRPSmallImageText();

	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));

	discordPresence.state = state.c_str();
	discordPresence.details = details.c_str();

	discordPresence.largeImageKey = largeImageKey.c_str();
	discordPresence.largeImageText = largeImageText.c_str();

	discordPresence.smallImageKey = smallImageKey.c_str();
	discordPresence.smallImageText = smallImageText.c_str();

	discordPresence.startTimestamp = discordrp_time_start;

	Discord_UpdatePresence(&discordPresence);
}
