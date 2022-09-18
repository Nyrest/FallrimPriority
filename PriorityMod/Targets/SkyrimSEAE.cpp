#include "../common.h"

#if _WIN64
extern "C"
{
	struct SKSEInterface
	{
		UInt32 skseVersion;
		UInt32 runtimeVersion;
		UInt32 editorVersion;
		UInt32 isEditor;
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

	struct SKSEPluginVersionData
	{
		enum
		{
			kVersion = 1,
		};

		enum
		{
			// set this if you are using a (potential at this time of writing) post-AE version of the Address Library
			kVersionIndependent_AddressLibraryPostAE = 1 << 0,
			// set this if you exclusively use signature matching to find your addresses and have NO HARDCODED ADDRESSES
			kVersionIndependent_Signatures = 1 << 1,
			// set this if you are using 1.6.629+ compatible structure layout (likely provided by CommonLib/SKSE)
			// this also marks you as incompatible with pre-1.6.629. see kVersionIndependentEx_NoStructUse if you have a
			// plugin that only does code patches and works across many versions
			kVersionIndependent_StructsPost629 = 1 << 2,
		};

		enum
		{
			// set this if your plugin either doesn't use any game structures or has put in extraordinary effort
			// to work with pre and post 1.6.629 structure layout
			kVersionIndependentEx_NoStructUse = 1 << 0,
		};

		UInt32	dataVersion;			// set to kVersion

		UInt32	pluginVersion;			// version number of your plugin
		char	name[256];				// null-terminated ASCII plugin name

		char	author[256];			// null-terminated ASCII plugin author name (can be empty)
		char	supportEmail[252];		// null-terminated ASCII support email address (can be empty)

		// version compatibility
		UInt32	versionIndependenceEx;	// set to one of the kVersionIndependentEx_ enums or zero
		UInt32	versionIndependence;	// set to one of the kVersionIndependent_ enums or zero
		UInt32	compatibleVersions[16];	// zero-terminated list of RUNTIME_VERSION_ defines your plugin is compatible with

		UInt32	seVersionRequired;		// minimum version of the script extender required, compared against PACKED_SKSE_VERSION
										// you probably should just set this to 0 unless you know what you are doing
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

	// Fuck
	DllExport SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,

		3,
		"FallrimPriority",

		"Boring3",
		"", // no support :)

		SKSEPluginVersionData::kVersionIndependentEx_NoStructUse, // version independent
		SKSEPluginVersionData::kVersionIndependent_Signatures, // version independent
		{MAKE_EXE_VERSION(1, 6, 318), MAKE_EXE_VERSION(1, 6, 629)},					   // compatible with 1.6.318, 1.6.629


		0, // works with any version of the script extender. you probably do not need to put anything here
	};
};
#endif
