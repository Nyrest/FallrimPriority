#include "../common.h"

#if _WIN64
extern "C"
{
	struct F4SEInterface
	{
		UInt32	f4seVersion;
		UInt32	runtimeVersion;
		UInt32	editorVersion;
		UInt32	isEditor;
		void* (*QueryInterface)(UInt32 id);

		// call during your Query or Load functions to get a PluginHandle uniquely identifying your plugin
		// invalid if called at any other time, so call it once and save the result
		PluginHandle(*GetPluginHandle)(void);

		// returns the F4SE build's release index
		UInt32(*GetReleaseIndex)(void);
	};

	DllExport bool F4SEPlugin_Query(const F4SEInterface* se, PluginInfo* info)
	{
		info->name = ModName;
		info->infoVersion = PluginInfo::kInfoVersion;
		info->version = ModVersion;
		return true;
	}

	DllExport bool F4SEPlugin_Load(const F4SEInterface* se)
	{
		return true;
	}
};
#endif