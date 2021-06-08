#include "..\config.h"

#if Support_Fallout4
#include "..\pch.h"

DllExport bool F4SEPlugin_Load(const MasterInterface* se)
{
	return true;
}

extern "C" {
	DllExport bool F4SEPlugin_Query(const MasterInterface* se, PluginInfo* info)
	{
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PLUGIN_NAME;
		info->version = PLUGIN_VERSION;
		return true;
	}
};
#endif