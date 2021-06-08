// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		int priority;
		if (std::filesystem::exists("Data/SKSE/Plugins/PriorityMod.toml"))
		{
			auto config = toml::parse_file("Data/SKSE/Plugins/PriorityMod.toml");
			priority = config["PriorityMod"]["priority"].value_or(HIGH_PRIORITY_CLASS);
		}
		else if (std::filesystem::exists("Data/F4SE/Plugins/PriorityMod.toml"))
		{
			auto config = toml::parse_file("Data/F4SE/Plugins/PriorityMod.toml");
			priority = config["PriorityMod"]["priority"].value_or(HIGH_PRIORITY_CLASS);
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


