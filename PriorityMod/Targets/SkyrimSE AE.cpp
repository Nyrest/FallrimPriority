#include "../common.h"

#if _WIN64
extern "C"
{
	struct SKSEInterface
	{
		UInt32	skseVersion;
		UInt32	runtimeVersion;
		UInt32	editorVersion;
		UInt32	isEditor;
		void* (*QueryInterface)(UInt32 id);

		// call during your Query or Load functions to get a PluginHandle uniquely identifying your plugin
		// invalid if called at any other time, so call it once and save the result
		PluginHandle(*GetPluginHandle)(void);

		// returns the SKSE build's release index
		UInt32(*GetReleaseIndex)(void);

		// Minimum SKSE version 2.0.18
		// returns the plugin info structure for a plugin by name, only valid to be called after PostLoad message
		const PluginInfo* (*GetPluginInfo)(const char* name);
	};

	DllExport bool SKSEPlugin_Query(const SKSEInterface* se, PluginInfo* info)
	{
		info->name = ModName;
		info->infoVersion = PluginInfo::kInfoVersion;
		info->version = ModVersion;
		return true;
	}

	DllExport bool SKSEPlugin_Load(const SKSEInterface* se)
	{
		return true;
	}
};
#endif