#include <iostream>
#include <Windows.h>
#include <conio.h>

#include "discordIntegration.h"

using namespace std;

discordIntegration * g_discordIntegration;

DWORD APIENTRY MainThread(HMODULE param) {

	AllocConsole();
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	cout << "DLLMain started" << endl;

	g_discordIntegration->init();

	while (true) {

		g_discordIntegration->update();
		Sleep(500);

	}

	FreeConsole();

	return 0;
}

bool APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {

	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
		case DLL_PROCESS_DETACH:
			break;
	}

	return true;
}
