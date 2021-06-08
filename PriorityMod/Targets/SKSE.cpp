#include "..\config.h"

#if Support_Skyrim
#include "..\pch.h"
DllExport bool SKSEPlugin_Load(const MasterInterface* se)
{
	return true;
}

extern "C" {
	DllExport bool SKSEPlugin_Query(const MasterInterface* se, PluginInfo* info)
	{
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PLUGIN_NAME;
		info->version = PLUGIN_VERSION;
		return true;
	}
};
#endif