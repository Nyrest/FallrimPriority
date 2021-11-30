#include "../common.h"

#if !_WIN64
struct MasterInterface
{
	UInt32	obseVersion;
	UInt32	oblivionVersion;
	UInt32	editorVersion;
	UInt32	isEditor;
};

extern "C"
{
	DllExport bool OBSEPlugin_Query(const OldMasterInterface* se, PluginInfo* info)
	{
		info->name = ModName;
		info->infoVersion = PluginInfo::kInfoVersion;
		info->version = ModVersion;
		return true;
	}

	DllExport bool OBSEPlugin_Load(const OldMasterInterface* se)
	{
		return true;
	}
};
#endif