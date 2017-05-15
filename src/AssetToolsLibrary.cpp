#include "stdafx.h"
#include "AssetToolsLibrary.h"
#include "Package.h"
#include <chrono>
#include <iostream>
#include "HapticsLoadingException.h"
#include "Node.h"
#include "JsonParsers.h"

#include "MetaFile.h"

#include "rapidjson\stringbuffer.h"
#include "MetaResolver.h"
#include <fstream>

#include <boost\filesystem.hpp>
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
		_packageList = packages;

		for (auto child : _rootPackage.Children)
		{
			buildPackagePaths(child.second, _rootPackage.Namespace);
		}

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
		std::fill(info.Studio, info.Studio + 128, 0);
		std::fill(info.Version, info.Version + 128, 0);

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

std::string AssetToolsLibrary::SafeGetError()
{
	return _lastErrorString;
}

std::vector<AssetToolsLibrary::OutputPackageInfo> AssetToolsLibrary::GetPackages()
{
	std::vector<OutputPackageInfo> output;
	for (const auto& package : _packageList) {
		OutputPackageInfo p;
		p.Dependencies = {};
		p.Path = std::get<0>(package).string();
		p.Namespace = std::get<1>(package).Package;
		p.Studio = std::get<1>(package).Studio;
		p.Version = std::get<1>(package).Version;
		p.Description = std::get<1>(package).Description;
		output.push_back(p);
	}
	return output;
}

void AssetToolsLibrary::CreateJsonAsset(std::string filePath, std::string outPath) {
	MetaResolver r(m_paths);

	boost::filesystem::path path(filePath);
	if (!path.has_extension()) {
		throw HapticsLoadingException("Path does not have an extension");
	}

	if (this->_packageMap.find(path.parent_path().parent_path()) != this->_packageMap.end()) {

		auto extension = path.extension();
		auto package = _packageMap[path.parent_path().parent_path()];
		auto nameOnly = path.stem().string();
		CreateMetaFile(package.Package + "." + nameOnly,
			extension.string().substr(1, extension.string().length() - 1), outPath);


	}
	else {
		throw HapticsLoadingException("Couldn't find the package associated with " + filePath);
	}
}
void AssetToolsLibrary::CreateMetaFile(std::string fileName, std::string fileType, std::string outpath)
{
	using namespace rapidjson;

	MetaResolver r(m_paths);
	std::unique_ptr<MetaFile> result;
	switch (StringFileType(fileType)) {
	case HapticFileType::Sequence:
		result = std::make_unique<MetaFile>(r.Resolve(SequenceFileInfo(fileName)));
		break;
	case HapticFileType::Pattern:
		result = std::make_unique<MetaFile>(r.Resolve(PatternFileInfo(fileName)));
		break;
	case HapticFileType::Experience:
		result = std::make_unique<MetaFile>(r.Resolve(ExperienceFileInfo(fileName)));
		break;
	default:
		std::cout << "Unknown file type\n";
		return;
	}



	Document d(kObjectType);

	result->Serialize(d);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);

	d.Accept(writer);
	std::cout << buffer.GetString();

	if (outpath != "") {
		std::ofstream outfile(outpath, std::ofstream::binary);

		if (!outfile.is_open()) {
			std::cout << "Couldn't open the path " << outpath << " for writing\n";

		}
		outfile.write(buffer.GetString(), buffer.GetSize());
	}

}

//deprecated
void AssetToolsLibrary::CreateBinaryAsset(std::string filePath, std::string outPath)
{
	MetaResolver r(m_paths);

	boost::filesystem::path path(filePath);
	if (!path.has_extension()) {
		throw HapticsLoadingException("Path does not have an extension");
	}

	if (this->_packageMap.find(path.parent_path().parent_path()) != this->_packageMap.end()) {

		auto extension = path.extension();
		auto package = _packageMap[path.parent_path().parent_path()];
		auto nameOnly = path.stem().string();
		CreateBinaryAsset(package.Package + "." + nameOnly,
			extension.string().substr(1, extension.string().length() - 1), outPath);


	}
	else {
		throw HapticsLoadingException("Couldn't find the package associated with " + filePath);
	}
}



void AssetToolsLibrary::CreateBinaryAsset(std::string fileName, std::string fileType, std::string path)
{
	using namespace rapidjson;

	MetaResolver r(m_paths);
	std::unique_ptr<MetaFile> result;
	switch (StringFileType(fileType)) {
	case HapticFileType::Sequence:
		result = std::make_unique<MetaFile>(r.Resolve(SequenceFileInfo(fileName)));
		break;
	case HapticFileType::Pattern:
		result = std::make_unique<MetaFile>(r.Resolve(PatternFileInfo(fileName)));
		break;
	case HapticFileType::Experience:
		result = std::make_unique<MetaFile>(r.Resolve(ExperienceFileInfo(fileName)));
		break;
	default:
		std::cout << "Unknown file type\n";
		return;
	}


//	auto res = result->ToHDF();
	//std::cout << res.DebugString() << '\n';
	std::string bytes;
//	res.SerializeToString(&bytes);
	if (path != "") {
		std::ofstream outfile(path, std::ofstream::binary);

		if (!outfile.is_open()) {
			std::cout << "Couldn't open the path " << path << " for writing\n";

		}
		//outfile.write(bytes.c_str(), res.ByteSize());
	}
}

void AssetToolsLibrary::buildPackagePaths(HapticDirectoryTools::PackageNode node, std::string prefix)
{

	//if it has a namespace,
	//then check if it had a parent namespace. 
	if (node.Data.Namespace != "")
	{
		if (prefix == "")
		{
			m_paths[node.Namespace] = node.Data.Directory;
		}
		else
		{
			m_paths[prefix + "." + node.Namespace] = node.Data.Directory;
		}
	}

	if (node.Children.size() == 0)
	{
		return;
	}

	for (auto child : node.Children)
	{
		//if PARENT is a toplevel node, we want to pass in just the parent name as the prefix
		//if PARENT is a child node, we want to pass in the full history by adding it on
		buildPackagePaths(child.second, prefix == "" ? node.Namespace : prefix + "." + node.Namespace);
	}


}

