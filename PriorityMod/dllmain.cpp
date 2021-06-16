// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "dllmain.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

unsigned long affinity = 0;
int priority = HIGH_PRIORITY_CLASS;

toml::parse_result config;

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

			if (ParseConfig())
			{
				int priorityValue = config["PriorityMod"]["priority"].value_or(4);
				if (priorityValue != -1)
					priority = priority_classes[(priorityValue >= 0 && priorityValue <= 5) ? priorityValue : 4];
				affinity = config["PriorityMod"]["affinity"].value_or(0);
			}

			if (priority != -1)
				SetPriorityClass(GetCurrentProcess(), priority);
			if (affinity != 0)
				SetProcessAffinityMask(GetCurrentProcess(), affinity);
			break;
		case DLL_THREAD_ATTACH:break;
		case DLL_THREAD_DETACH:break;
		case DLL_PROCESS_DETACH:break;
	}
	return TRUE;
}


bool ParseConfig()
{
	LPTSTR buffer = new TCHAR[_MAX_PATH];
	GetModuleFileNameW((HINSTANCE)&__ImageBase, buffer, _MAX_PATH);

	std::wstring filename = buffer;

	std::wstring configPath = filename.substr(0, filename.find_last_of('.')) + L".toml";
	if (std::filesystem::exists(configPath))
	{
		config = toml::parse_file(configPath);
		return true;
	}
	return false;
}