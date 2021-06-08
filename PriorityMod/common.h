#pragma once
#include "config.h"

#define DllExport   __declspec( dllexport )

typedef unsigned char		UInt8;		//!< An unsigned 8-bit integer value
typedef unsigned short		UInt16;		//!< An unsigned 16-bit integer value
typedef unsigned long		UInt32;		//!< An unsigned 32-bit integer value
typedef unsigned long long	UInt64;		//!< An unsigned 64-bit integer value
typedef signed char			SInt8;		//!< A signed 8-bit integer value
typedef signed short		SInt16;		//!< A signed 16-bit integer value
typedef signed long			SInt32;		//!< A signed 32-bit integer value
typedef signed long long	SInt64;		//!< A signed 64-bit integer value
typedef float				Float32;	//!< A 32-bit floating point value
typedef double				Float64;	//!< A 64-bit floating point value

typedef UInt32	PluginHandle;	// treat this as an opaque type

struct PluginInfo
{
	enum
	{
		kInfoVersion = 1
	};

	UInt32			infoVersion;
	const char* name;
	UInt32			version;
};

struct MasterInterface
{
	UInt32	version;
	UInt32	runtimeVersion;
	UInt32	editorVersion;
	UInt32	isEditor;
	void* (*QueryInterface)(UInt32 id);

	// call during your Query or Load functions to get a PluginHandle uniquely identifying your plugin
	// invalid if called at any other time, so call it once and save the result
	PluginHandle(*GetPluginHandle)(void);

	// returns the SKSE build's release index
	UInt32(*GetReleaseIndex)(void);

#if _WIN64
	// Minimum SKSE version 2.0.18
	// returns the plugin info structure for a plugin by name, only valid to be called after PostLoad message
	const PluginInfo* (*GetPluginInfo)(const char* name);
#endif
};