#include "stdafx.h"
#include "AssetToolsLibrary.h"
#include "Package.h"
#include <chrono>
#include <iostream>
#include "HapticsLoadingException.h"

AssetToolsLibrary::AssetToolsLibrary()
{
	

	
}


AssetToolsLibrary::~AssetToolsLibrary()
{
}

int AssetToolsLibrary::InitializeFromDirectory(const char * dir)
{
	//calling this multiple times will destroy and recreate the enumerator
	_fileEnumerator = std::make_unique<HapticDirectoryTools::HapticEnumerator>(dir);

	//after creation, tell the enumerator to scan the directories
	return Rescan();
}

int AssetToolsLibrary::Rescan()
{
	//smart logic later
	try {
		auto packages = _fileEnumerator->EnumeratePackages();
		_rootPackage = _fileEnumerator->GeneratePackageTree(_fileEnumerator->GetAllFiles(packages));
		_packageMap = HapticDirectoryTools::GetPackageMap(packages);
		return true;
	}
	catch (const HapticsLoadingException& ex) {
		_lastErrorString = ex.what();
		return false;
	}
}

int AssetToolsLibrary::CheckIfPackage(const char * dir, PackageInfo & info, bool& isPackage)
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
		isPackage = true;
	}
	else {
		isPackage = false;
	}

	return true;
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

