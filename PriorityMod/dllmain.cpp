// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	int priority_classes[] = {
		IDLE_PRIORITY_CLASS,
		BELOW_NORMAL_PRIORITY_CLASS,
		NORMAL_PRIORITY_CLASS,
		ABOVE_NORMAL_PRIORITY_CLASS,
		HIGH_PRIORITY_CLASS,
		REALTIME_PRIORITY_CLASS
	};

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		int priority;

		if (std::filesystem::exists("Data/SKSE/Plugins/PriorityMod.toml"))
		{
			auto config = toml::parse_file("Data/SKSE/Plugins/PriorityMod.toml");
			int value = config["PriorityMod"]["priority"].value_or(4);
			priority = priority_classes[(value >= 0 && value <= 5) ? value : 4];
		}
		else if (std::filesystem::exists("Data/F4SE/Plugins/PriorityMod.toml"))
		{
			auto config = toml::parse_file("Data/F4SE/Plugins/PriorityMod.toml");
			int value = config["PriorityMod"]["priority"].value_or(4);
			priority = priority_classes[(value >= 0 && value <= 5) ? value : 4];
		}
		else
		{
			priority = HIGH_PRIORITY_CLASS;
		}
		SetPriorityClass(GetCurrentProcess(), priority);
		break;
	case DLL_THREAD_ATTACH:break;
	case DLL_THREAD_DETACH:break;
	case DLL_PROCESS_DETACH:break;
	}
	return TRUE;
}


