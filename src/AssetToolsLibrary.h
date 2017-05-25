#pragma once
#include "HapticDirectoryTools.h"
#include "HapticAssetTools.h"

#include "rapidjson\document.h"
#include <sstream>
#include <string>
#include <iomanip>
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
class AssetToolsLibrary
{
public:
	typedef std::unordered_map<std::string, boost::filesystem::path> PackagePathMap;
	struct OutputPackageInfo {
		std::string Version;
		std::string Studio;
		std::string Namespace;
		std::string Path;
		std::string Description;
		std::vector<std::string> Dependencies;
	public:
		std::string to_readable_string() const {
			std::stringstream output;
			auto parentDir = boost::filesystem::path(Path).filename().string();

			output <<
				std::left << std::setw(20) << Namespace <<
				std::left << std::setw(20) << Studio <<
				std::left << std::setw(20) << Version <<
				parentDir;
			return output.str();
		}

		void to_parsable_string(rapidjson::Document& doc) const{
			using namespace rapidjson;
			auto& alloc = doc.GetAllocator();
			Value d(kObjectType);
			{

				d.AddMember("version", StringRef(Version), alloc);
				d.AddMember("studio", StringRef(Studio), alloc);
				d.AddMember("namespace", StringRef(Namespace), alloc);
				d.AddMember("path", StringRef(Path), alloc);
				d.AddMember("description", StringRef(Description), alloc);
			}
			assert(doc.IsArray());
			doc.PushBack(d, alloc);
		}
	};
	AssetToolsLibrary();
	~AssetToolsLibrary();
	void AssetToolsLibrary::CreateJsonAsset(std::string filePath, std::string outPath);
	int InitializeFromDirectory(const char* dir);
	int Rescan();
	int CheckIfPackage(const char* dir, PackageInfo& info, bool& isPackage);
	char* GetError();
	std::string SafeGetError();
	std::vector<OutputPackageInfo> GetPackages();
	void CreateMetaFile(std::string fileName, std::string fileType, std::string outPath);
	void CreateBinaryAsset(std::string filePath, std::string outPath);
	void CreateBinaryAsset(std::string fileName, std::string fileType, std::string outPath);
	bool PackageExists(const std::string& packageId);
	bool CreateHDFPackageStructure(std::string outDir);
	bool ConvertPackageToHDFs(std::string packageId, const std::string& outPath);
private:
	std::unique_ptr<HapticDirectoryTools::HapticEnumerator> _fileEnumerator;
	HapticDirectoryTools::PackageNode _rootPackage;
	std::unordered_map<boost::filesystem::path, HapticDirectoryTools::HapticConfig> _packageMap;
	std::vector<HapticDirectoryTools::HapticEnumerator::package> _packageList;
	std::string _lastErrorString;

	void buildPackagePaths(HapticDirectoryTools::PackageNode node, std::string prefix);
	std::unordered_map<std::string, boost::filesystem::path> m_paths;
};

