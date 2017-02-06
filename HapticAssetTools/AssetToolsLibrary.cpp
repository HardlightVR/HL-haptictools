#include "stdafx.h"
#include "AssetToolsLibrary.h"
#include "Package.h"
#include <chrono>
#include <iostream>
#include "StringPointerException.h"

AssetToolsLibrary::AssetToolsLibrary(const char* dir):
	_fileEnumerator(dir)
{
	
	Rescan();

	
}


AssetToolsLibrary::~AssetToolsLibrary()
{
}

void AssetToolsLibrary::Rescan()
{
	//smart logic later
	try {
		auto packages = _fileEnumerator.EnumeratePackages();
		_rootPackage = _fileEnumerator.GeneratePackageTree(_fileEnumerator.GetAllFiles(packages));
		_packageMap = HapticDirectoryTools::GetPackageMap(packages);
	}
	catch (const StringPointerException& ex) {
		
	}
}

bool AssetToolsLibrary::IsPackage(const char * dir, PackageInfo & info)
{
	if (_packageMap.find(boost::filesystem::path(dir)) != _packageMap.end()) {
		std::fill(info.Namespace, info.Namespace + 128, 0);
		std::fill(info.Studio, info.Studio +128, 0);
		std::fill(info.Version, info.Version+128, 0);

		const auto& package = _packageMap.at(dir);
		//take care not to overwrite the size of our structs, and make it null-terminated
		std::copy(package.Studio.cbegin(), package.Studio.cend(), info.Studio);
		info.Studio[127] = 0;
		std::copy(package.Version.cbegin(), package.Version.cend(), info.Version);
		info.Version[127] = 0;
		std::copy(package.Package.cbegin(), package.Package.cend(), info.Namespace);
		info.Namespace[127] = 0;
		return true;
	}
	else {
		return false;
	}
}

char * AssetToolsLibrary::GetError()
{
	
	const char* sampleString = _lastErrorString.c_str();
	std::size_t len = strlen(sampleString) + sizeof(char);
	char* newString = new char[len];
	strcpy_s(newString, len, sampleString);
	newString[len - 1] = 0;


	return newString;
	
}

