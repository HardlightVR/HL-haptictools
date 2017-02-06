#pragma once
#include "HapticDirectoryTools.h"
#include "HapticAssetTools.h"



class AssetToolsLibrary
{
public:

	AssetToolsLibrary(const char* dir);
	~AssetToolsLibrary();

	void Rescan();
	bool IsPackage(const char* dir, PackageInfo& info);
	char* GetError();
private:
	HapticDirectoryTools::HapticEnumerator _fileEnumerator;
	HapticDirectoryTools::PackageNode _rootPackage;
	std::unordered_map<boost::filesystem::path, HapticDirectoryTools::HapticConfig> _packageMap;

	std::string _lastErrorString;
};

