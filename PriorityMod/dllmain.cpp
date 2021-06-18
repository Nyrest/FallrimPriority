// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <WinUser.h>
#include <thread>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

unsigned long affinity = 0;
int priority = HIGH_PRIORITY_CLASS;
unsigned long idle_affinity = 0;
int idle_priority = BELOW_NORMAL_PRIORITY_CLASS;

toml::parse_result config;

bool ParseConfig()
{
	LPTSTR buffer = new TCHAR[_MAX_PATH];
	GetModuleFileNameW((HINSTANCE)&__ImageBase, buffer, _MAX_PATH);

	wstring filename = buffer;

	wstring configPath = filename.substr(0, filename.find_last_of('.')) + L".toml";
	if (filesystem::exists(configPath))
	{
		config = toml::parse_file(configPath);
		return true;
	}
	return false;
}

void ApplyPriority(bool high = false)
{
	auto val = high ? priority : idle_priority;
	if (val != -1)
		SetPriorityClass(GetCurrentProcess(), val);
}

void ApplyAffinity(bool high = false)
{
	auto val = high ? affinity : idle_affinity;
	if (val != 0)
		SetProcessAffinityMask(GetCurrentProcess(), val);
}

#pragma region Dynamic Priority
HWND game_window;
ULONG currentProcessId = GetCurrentProcessId();

BOOL is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}
BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (currentProcessId != process_id || !is_main_window(handle))
		return TRUE;
	game_window = handle;
	return FALSE;
}

void GetGameWindowHandle()
{
	do
	{
		EnumWindows(enum_windows_callback, NULL);
		Sleep(100);
	} while (game_window == 0);
}

bool isHighPriority = true;
void DynamicPriority(LPVOID)
{
	GetGameWindowHandle();
	do
	{
		if (GetForegroundWindow() == game_window) // focus
		{
			if (isHighPriority) goto conti; // already focused
		highPrio:
			ApplyPriority(true);
			ApplyAffinity(true);
			isHighPriority = true;
		}
		else
		{
			// if not responding go high
			// more accurate than IsHungAppWindow
			if (SendMessageTimeout(game_window, WM_NULL, NULL, NULL, SMTO_ABORTIFHUNG, 500, NULL) == 0)
				goto highPrio;

			if (isHighPriority) // lost focus
			{
				ApplyPriority(false);
				ApplyAffinity(false);
				isHighPriority = false;
			}
		}
	conti:
		Sleep(1000);
	} while (true);
}
#pragma endregion

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		static HMODULE current;
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPTSTR)&current, &current);

		int priority_classes[] = {
			IDLE_PRIORITY_CLASS,
			BELOW_NORMAL_PRIORITY_CLASS,
			NORMAL_PRIORITY_CLASS,
			ABOVE_NORMAL_PRIORITY_CLASS,
			HIGH_PRIORITY_CLASS,
			REALTIME_PRIORITY_CLASS
		};

		if (ParseConfig())
		{
#pragma region PriorityMod
			int priorityValue = config["PriorityMod"]["priority"].value_or(4);
			if (priorityValue != -1)
				priority = priority_classes[(priorityValue >= 0 && priorityValue <= 5) ? priorityValue : 4];
			affinity = config["PriorityMod"]["affinity"].value_or(0);
#pragma endregion

#pragma region DynamicPriority
			int idle_priorityValue = config["DynamicPriority"]["idle_priority"].value_or(1);
			if (idle_priorityValue != -1)
				idle_priority = priority_classes[(idle_priorityValue >= 0 && idle_priorityValue <= 5) ? idle_priorityValue : 1];
			idle_affinity = config["DynamicPriority"]["idle_affinity"].value_or(0);
#pragma endregion

		}

		ApplyPriority();
		ApplyAffinity();

		if (config["DynamicPriority"]["enabled"].value_or(1))
			_beginthread(DynamicPriority, NULL, NULL);
	}
	return TRUE;
}