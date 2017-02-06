#include "stdafx.h"
#include "HapticAssetTools.h"
#include "AssetToolsLibrary.h"

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)


unsigned int NS_ASSETTOOLS_API NSAT_GetVersion(void)
{
	return NS_ASSETTOOLS_VERSION;
}

int NSAT_IsCompatibleDLL(void)
{
	unsigned int major = NSAT_GetVersion() >> 16;
	return major == NS_ASSETTOOLS_VERSION_MAJOR;
}

NS_ASSETTOOLS_API NSAT_Context_t * __stdcall NSAT_InitializeFromDirectory(const char * string)
{
	return AS_TYPE(NSAT_Context_t, new AssetToolsLibrary(string));
}

NS_ASSETTOOLS_API void __stdcall NSAT_Delete(NSAT_Context_t * context)
{
	if (!context) {
		return;
	}

	delete AS_TYPE(AssetToolsLibrary, context);

}

NS_ASSETTOOLS_API void __stdcall NSAT_RescanFilesystem(NSAT_Context_t * context)
{
	AS_TYPE(AssetToolsLibrary, context)->Rescan();
}

NS_ASSETTOOLS_API int __stdcall NSAT_IsPackage(NSAT_Context_t* context, const char* dir, PackageInfo& info)
{
	return AS_TYPE(AssetToolsLibrary, context)->IsPackage(dir, info);
}

NS_ASSETTOOLS_API char * __stdcall GetError(NSAT_Context_t * context)
{
	return AS_TYPE(AssetToolsLibrary, context)->GetError();
}

NS_ASSETTOOLS_API void __stdcall FreeError(char * stringPointer)
{
	delete[] stringPointer;
	stringPointer = nullptr;
}




