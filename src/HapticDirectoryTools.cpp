#include "stdafx.h"
#include <iostream>

#include "HapticDirectoryTools.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"


using namespace HapticDirectoryTools;
using namespace boost::filesystem;






PackageNode::PackageNode(HapticFileNameList data)
{
	this->Data = data;
	this->Namespace = data.Namespace;

}


HapticEnumerator::HapticEnumerator(const std::string& path) :
	_validExtensions{
	".pattern",
	".sequence",
	".experience"
},
_basePath(path)
{
}





void HapticConfig::Deserialize(const rapidjson::Value& root)
{

	this->Package = parseKeyOrThrow<const char*>(root, "package");

	//normalize the package name to lowercase
	std::transform(this->Package.begin(), this->Package.end(), this->Package.begin(), ::tolower);

	this->Studio = parseKeyOrThrow<const char*>(root, "studio");
	this->Version = parseKeyOrDefault<const char*>(root, "version", "1.0");
	this->Description = parseKeyOrDefault<const char*>(root, "description", "");
}

void HapticConfig::Serialize(const rapidjson::Value& root)
{

}


const std::string HapticEnumerator::MISSING_OR_INVALID_KEY = "INVALID";
const std::string HapticEnumerator::EXPERIENCE_DIR = "experiences";
const std::string HapticEnumerator::PATTERN_DIR = "patterns";
const std::string HapticEnumerator::SEQUENCE_DIR = "sequences";

PackageNode HapticEnumerator::GeneratePackageTree(const std::vector<HapticFileNameList>& enumLists) const
{
	PackageNode root;
	for (auto list : enumLists)
	{
		insert(root, list);
	}
	return root;

}

std::vector<boost::filesystem::path> HapticEnumerator::GetDirectories(const std::string& path)
{
	std::vector<boost::filesystem::path> dirs;
	if (is_directory(path)) {
		directory_iterator it{ path };
		while (it != directory_iterator{}) {
			if (is_directory(*it)) {
				dirs.push_back(it->path());
			}
			*it++;
		}
		return dirs;
	}
	else {
		throw HapticsRootDirectoryNotFound("The root directory " + path + " was not found");
	}

}


std::unordered_map<boost::filesystem::path, HapticConfig> HapticDirectoryTools::GetPackageMap(const std::vector<HapticEnumerator::package>& packages)
{
	std::unordered_map<path, HapticConfig> map;
	for (const auto& package : packages) {
		map[std::get<0>(package)] = std::get<1>(package);
	}
	return map;
}

PackageNode HapticDirectoryTools::GetAllPackages(const HapticEnumerator& enumerator) {
	return enumerator.GeneratePackageTree(enumerator.GetAllFiles(enumerator.EnumeratePackages()));
}


const char* HapticEnumerator::DirectoryNotFoundString = "The given root haptics directory was not found";


rapidjson::Document readJsonFromStream(const boost::filesystem::path& path)
{
	rapidjson::Document document;
	std::ifstream configStream(path.string());
	rapidjson::IStreamWrapper isw(configStream);
	document.ParseStream(isw);
	return document;
}
std::vector<std::tuple<path, HapticConfig>> HapticEnumerator::EnumeratePackages() const
{
	auto potentialPackages = GetDirectories(_basePath);
	std::vector<std::tuple<path, HapticConfig>> packages;
	for (const auto& path : potentialPackages)
	{
		auto configPath = path;
		configPath /= "config.json";
		if (exists(configPath)) {
			auto configJson = readJsonFromStream(configPath);
			if (configJson.IsObject()) {
				HapticConfig tempConfig;
				try {
					tempConfig.Deserialize(configJson);
					packages.push_back(std::make_tuple(path, tempConfig));
				}
				catch (JsonValueInvalidException e)
				{
					std::cout << "In config file for directory " << path.filename() << ": " << e.what() << "\n";
					std::cout << "Not loading the haptics found in directory " << path.filename() << "\n";
				}
			}
			else {
				std::cout << "Malformed config file at path: ["<< configPath << "]\n";
			}

		}
	}

	return packages;

}

boost::filesystem::path PathCombine(const path& p1, const std::string& p2)
{
	path result = p1;
	result /= path(p2);
	return result;
}

HapticFileNameList HapticEnumerator::GetFilesInPackage(package configTuple)  const
{
	HapticFileNameList fileList;
	path directory;
	HapticConfig config;

	std::tie(directory, config) = configTuple;

	fileList.Patterns = GetFileNames(PathCombine(directory, PATTERN_DIR));
	fileList.Sequences = GetFileNames(PathCombine(directory, SEQUENCE_DIR));
	fileList.Experiences = GetFileNames(PathCombine(directory, EXPERIENCE_DIR));
	fileList.Namespace = config.Package;
	fileList.Studio = config.Studio;
	fileList.Directory = directory;
	fileList.Description = config.Description;

	return fileList;

}


std::vector<HapticFileNameList> HapticEnumerator::GetAllFiles(const std::vector<package>& configs) const
{
	std::vector<HapticFileNameList> hapticEnums;
	hapticEnums.reserve(configs.size());

	for (auto configTuple : configs) {
		hapticEnums.push_back(GetFilesInPackage(configTuple));
	}

	return hapticEnums;
}

bool HapticEnumerator::isValidFileExtension(const std::string& extension) const
{
	return std::find(_validExtensions.begin(), _validExtensions.end(), extension) != _validExtensions.end();

}

std::vector<std::string> HapticEnumerator::GetFileNames(path path) const {
	if (!is_directory(path)) {
		return std::vector<std::string>();
	}
	std::vector<std::string> names;
	directory_iterator end_itr;

	for (directory_iterator itr(path); itr != end_itr; ++itr)
	{
		if (is_regular_file(itr->path()))
		{
			if (!isValidFileExtension(itr->path().extension().string()))
			{
				continue;
			}
			names.push_back(itr->path().filename().string());
		}
	}


	return names;

}
std::vector<std::string> GetNamespaceComponents(const std::string& nameSpace)
{
	std::vector<std::string> result;
	boost::split(result, nameSpace, boost::algorithm::is_any_of("."));
	return result;
}
void HapticEnumerator::insert(PackageNode& node, HapticFileNameList& list) const
{
	auto components = GetNamespaceComponents(list.Namespace);
	std::string topLevel = components[0];
	if (components.size() == 1)
	{
		node.Children[topLevel] = PackageNode(list);

	}
	else
	{
		list.Namespace = list.Namespace.substr(list.Namespace.find_first_of(".") + 1);

		if (node.Children.find(topLevel) != node.Children.end())
		{
			insert(node.Children[topLevel], list);
		}
		else
		{
			node.Children[topLevel] = PackageNode();
			insert(node.Children[topLevel], list);
		}
	}

}

void findKeyOrThrow(const rapidjson::Value & root, const char * key)
{
	rapidjson::Value::ConstMemberIterator itr = root.FindMember(key);
	if (itr == root.MemberEnd()) {

		throw JsonValueInvalidException(key);
	}
}
