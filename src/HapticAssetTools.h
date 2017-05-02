#pragma once


#ifdef NS_ASSETTOOLS_EXPORTS
#define NS_ASSETTOOLS_API __declspec(dllexport) 
#else
#define NS_ASSETTOOLS_API __declspec(dllimport) 
#endif

#define NS_ASSETTOOLS_VERSION_MAJOR 0
#define NS_ASSETTOOLS_VERSION_MINOR 1
#define NS_ASSETTOOLS_VERSION ((NS_ASSETTOOLS_VERSION_MAJOR << 16) | NS_ASSETTOOLS_VERSION_MINOR)

#ifdef __cplusplus 
extern "C" {
#endif

	struct NSAT_Context;
	typedef struct NSAT_Context NSAT_Context_t;

	struct PackageInfo {
		char Studio[128];
		char Namespace[128];
		char Version[128];
	};

	unsigned int NS_ASSETTOOLS_API NSAT_GetVersion(void);
	int NSAT_IsCompatibleDLL(void);

	NS_ASSETTOOLS_API NSAT_Context_t* __stdcall NSAT_Create();

	NS_ASSETTOOLS_API int __stdcall  NSAT_InitializeFromDirectory(NSAT_Context_t* context, const char* string);
	
	NS_ASSETTOOLS_API void __stdcall NSAT_Delete(NSAT_Context_t* context);

	NS_ASSETTOOLS_API int __stdcall NSAT_RescanFilesystem(NSAT_Context_t* context);

	NS_ASSETTOOLS_API int __stdcall NSAT_CheckIfPackage(NSAT_Context_t* context, const char* dir, PackageInfo& info, bool& isPackage);
		
	NS_ASSETTOOLS_API char* __stdcall NSAT_GetError(NSAT_Context_t* context);
	NS_ASSETTOOLS_API void __stdcall NSAT_FreeError(char* stringPointer);


#ifdef __cplusplus 
}
#endif