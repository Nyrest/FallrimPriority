#include "../common.h"

#if _WIN64
extern "C"
{
	DllExport bool F4SEPlugin_Query(const MasterInterface* se, PluginInfo* info)
	{
		info->name = ModName;
		info->infoVersion = PluginInfo::kInfoVersion;
		info->version = ModVersion;
		return true;
	}

	DllExport bool F4SEPlugin_Load(const MasterInterface* se)
	{
		return true;
	}
};
#endif