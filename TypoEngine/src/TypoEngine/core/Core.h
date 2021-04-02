#pragma once

#ifdef TE_PLATFORM_WINDOWS
	#ifdef TE_DLL
		#define TE_API _declspec(dllexport)
	#else
		#define TE_API _declspec(dllimport)
	#endif
#else
	// TODO: handle other platforms
#endif
