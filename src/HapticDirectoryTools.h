#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <boost\filesystem.hpp>
#include "IJsonSerializable.h"
#include "rapidjson\document.h"
#include <unordered_map>
#include <boost\functional\hash\hash.hpp>
#include "HapticsLoadingException.h"


class JsonValueInvalidException : public std::runtime_error {
public:
	JsonValueInvalidException(const std::string& key) :
		std::runtime_error(std::string("Key '" + key + "' has an invalid value or is not present").c_str()) {}
};

class HapticsRootDirectoryNotFound : public HapticsLoadingException {
public:
	HapticsRootDirectoryNotFound(std::string err) :
		HapticsLoadingException(err) {}
};

template<class T>
T parseKeyOrThrow(const rapidjson::Value& root, const char* key)
{
	rapidjson::Value::ConstMemberIterator itr = root.FindMember(key);
	if (itr != root.MemberEnd()) {
		return itr->value.Get<T>();
	}
	else {
		throw JsonValueInvalidException(key);
	}

}

void findKeyOrThrow(const rapidjson::Value& root, const char* key);
template<class T>
T parseKeyOrDefault(const rapidjson::Value& root, const char* key, T defaultVal) {
	rapidjson::Value::ConstMemberIterator itr = root.FindMember(key);
	if (itr != root.MemberEnd()) {
		return itr->value.Get<T>();
	}
	else {
		return defaultVal;
	}
}

namespace std
{
	template<> struct hash<boost::filesystem::path>
	{
		size_t operator()(const boost::filesystem::path& p) const
		{
			return boost::filesystem::hash_value(p);
		}
	};
}

namespace HapticDirectoryTools {


	class HapticFileNameList {
	public:
		std::vector<std::string> Patterns;
		std::vector<std::string> Sequences;
		std::vector<std::string> Experiences;

		std::string Namespace;
		std::string Studio;
		boost::filesystem::path Directory;

	};

	class PackageNode
	{
	public:
		PackageNode();
		PackageNode(HapticFileNameList data);
		~PackageNode();
		std::string Namespace;
		HapticFileNameList Data;
		std::unordered_map<std::string, PackageNode> Children;
	};

	class HapticConfig : public IJsonSerializable{
	public:
		HapticConfig();
		~HapticConfig();
		std::string Version;
		std::string Studio;
		std::string Package;
		virtual void Serialize(const rapidjson::Value& root);
		virtual void Deserialize(const rapidjson::Value& root);
	};

	
	class HapticEnumerator {
	public:
		static const char* DirectoryNotFoundString;

		typedef std::tuple<boost::filesystem::path, HapticConfig> package;
		
		HapticEnumerator(const std::string& path);
		~HapticEnumerator();

		PackageNode GeneratePackageTree(const std::vector<HapticFileNameList>& enumLists) const;

		std::vector<package> EnumeratePackages() const;

		std::vector<HapticFileNameList> GetAllFiles(const std::vector<package>& configs) const;
		std::vector<std::string> GetFileNames(boost::filesystem::path path) const;
		HapticFileNameList GetFilesInPackage(package config) const;

		static std::vector<boost::filesystem::path> GetDirectories(const std::string& path);


	private:
		std::vector<std::string> _validExtensions;
		std::string _basePath;
		void insert(PackageNode& node, HapticFileNameList& list) const;
		bool isValidFileExtension(const std::string& fileExtension) const;
		static const std::string PATTERN_DIR;
		static const std::string SEQUENCE_DIR;
		static const std::string EXPERIENCE_DIR;
		static const std::string MISSING_OR_INVALID_KEY;
	};
	std::unordered_map<boost::filesystem::path, HapticConfig> GetPackageMap(const std::vector<HapticEnumerator::package>& packages);
	PackageNode GetAllPackages(const HapticEnumerator& enumerator);

}

