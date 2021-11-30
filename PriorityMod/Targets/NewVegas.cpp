#include "../common.h"

#if !_WIN64
extern "C"
{
	DllExport bool NVSEPlugin_Query(const OldMasterInterface* se, PluginInfo* info)
	{
		info->name = ModName;
		info->infoVersion = PluginInfo::kInfoVersion;
		info->version = ModVersion;
		return true;
	}

	DllExport bool NVSEPlugin_Load(const OldMasterInterface* se)
	{
		return true;
	}
};
#endif