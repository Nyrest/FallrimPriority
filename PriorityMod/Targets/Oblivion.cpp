#include "../common.h"

#if !_WIN64
extern "C"
{
	DllExport bool OBSEPlugin_Query(const MasterInterface* se, PluginInfo* info)
	{
		info->name = ModName;
		info->infoVersion = PluginInfo::kInfoVersion;
		info->version = ModVersion;
		return true;
	}

	DllExport bool OBSEPlugin_Load(const MasterInterface* se)
	{
		return true;
	}
};
#endif