#pragma once
#include <fstream>
#include <boost\filesystem.hpp>
#include <unordered_map>

#include "rapidjson\istreamwrapper.h"
#include "rapidjson\document.h"
#include "HapticDirectoryTools.h"
#include "Node.h"
#include "HapticFileInfo.h"

#include "rapidjson/error/en.h"
template<class T>
class Parser {
public:
	Parser(std::unordered_map<std::string, boost::filesystem::path>& pathMap, std::string magicKey);
	~Parser();
	std::vector<T> Get(const HapticFileInfo& info);
private:
	std::string m_key;
	std::unordered_map<std::string, boost::filesystem::path>& m_pathMap;
	std::unordered_map<std::string, std::vector<T>> m_cache;
	std::vector<T> parseFromPath(const boost::filesystem::path& path, const HapticFileInfo& info);
};

template<class T>
inline Parser<T>::Parser(std::unordered_map<std::string, boost::filesystem::path>& pathMap, std::string magicKey):
	m_pathMap(pathMap), m_key(magicKey)
{
	
}


template<class T>
inline Parser<T>::~Parser()
{
}

template<class T>
inline std::vector<T> Parser<T>::Get(const HapticFileInfo& info)
{
	using namespace rapidjson;

	if (m_cache.find(info.FullId) != m_cache.end()) {
		return m_cache.at(info.FullId);
	}

	if (m_pathMap.find(info.FullyQualifiedPackage) == m_pathMap.end()) {
		//throw
		throw HapticsLoadingException("While looking for " + info.FullId + ", could not find package " + info.FullyQualifiedPackage);
	}

	auto path = m_pathMap.at(info.FullyQualifiedPackage);
	path /= info.GetDirectory();
	 path /= info.GetValidFileNames()[0];
	if (!boost::filesystem::exists(path)) {
		throw HapticsLoadingException("Couldn't find " + info.FullId + " in package " + info.FullyQualifiedPackage);
	}
	
	return parseFromPath(path, info);


}


template<class T>
inline std::vector<T> Parser<T>::parseFromPath(const boost::filesystem::path & path, const HapticFileInfo& info)
{
	using namespace rapidjson;
	std::ifstream ifs(path.c_str());
	IStreamWrapper isw(ifs);
	Document d;
	if (d.ParseStream<kParseTrailingCommasFlag|kParseCommentsFlag>(isw).HasParseError()) {
		throw new HapticsLoadingException(GetParseError_En(d.GetParseError()));
	}

	//Make sure the filetype is actually what we are trying to parse
	findKeyOrThrow(d, m_key.c_str());

	auto& val = d[m_key];

	assert(val.IsArray());

	std::vector<T> result;
	for (auto& v : val.GetArray()) {
		T node;
		node.Deserialize(v);
		result.push_back(std::move(node));
	}
	m_cache[info.FullId] = result;
	return result;


}
