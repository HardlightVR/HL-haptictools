#pragma once
#include "HapticDirectoryTools.h"
#include "HapticAssetTools.h"



class AssetToolsLibrary
{
public:

	AssetToolsLibrary();
	~AssetToolsLibrary();
	int InitializeFromDirectory(const char* dir);
	int Rescan();
	int CheckIfPackage(const char* dir, PackageInfo& info, bool& isPackage);
	char* GetError();
private:
	std::unique_ptr<HapticDirectoryTools::HapticEnumerator> _fileEnumerator;
	HapticDirectoryTools::PackageNode _rootPackage;
	std::unordered_map<boost::filesystem::path, HapticDirectoryTools::HapticConfig> _packageMap;

	std::string _lastErrorString;
};

