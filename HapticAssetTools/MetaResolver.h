#pragma once
#include "JsonParsers.h"
#include "Node.h"
#include "MetaFile.h"

#include "HapticFileInfo.h"
class MetaResolver
{
public:
	MetaResolver(std::unordered_map<std::string, boost::filesystem::path>& packagePaths);
	
	~MetaResolver();

	MetaFile Resolve(const SequenceFileInfo& info);
	MetaFile Resolve(const PatternFileInfo& info);
	MetaFile Resolve(const ExperienceFileInfo& info);

	MetaFile Resolve(std::string path);


private:
	Parser<SequenceNode> m_seqParser;
	Parser<PatternNode> m_patParser;
	Parser<ExperienceNode> m_expParser;


	//std::unordered_map<std::string, boost::filesystem::path>& m_packagePaths;

};

