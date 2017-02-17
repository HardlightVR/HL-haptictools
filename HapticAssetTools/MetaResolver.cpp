#include "stdafx.h"
#include "MetaResolver.h"

#include <unordered_map>


MetaResolver::MetaResolver(std::unordered_map<std::string, boost::filesystem::path>& packagePaths):
	m_seqParser(packagePaths, "sequence"),
	m_patParser(packagePaths, "pattern"),
	m_expParser(packagePaths, "experience")
{
}

MetaResolver::~MetaResolver()
{
}

MetaFile MetaResolver::Resolve(const SequenceFileInfo& info)
{
	auto data = m_seqParser.Get(info);

	MetaFile m(info);
	m.AddSequenceDefinition(info.FullId, data);
	
	return m;
	//m.AddSequenceDefinition("ns.click", res);
}

MetaFile MetaResolver::Resolve(const PatternFileInfo & info)
{
	auto patData = m_patParser.Get(info);

	MetaFile m(info);

	m.AddPatternDefinition(info.FullId, patData);

	std::vector<std::string> dependencies;

	for (const auto& patNode : patData) {
		if (std::find(dependencies.begin(), dependencies.end(), patNode.Sequence()) == dependencies.end()) {
			dependencies.push_back(patNode.Sequence());
		}
	}

	for (const auto& seq : dependencies) {
		m.AddSequenceDefinition(seq, m_seqParser.Get(SequenceFileInfo(seq)));
	}

	return m;

}


MetaFile MetaResolver::Resolve(std::string p)
{
	//first, determine what type we are dealing with
	boost::filesystem::path path(p);
	if (!path.has_extension()) {
		throw HapticsLoadingException("Path does not have an extension");
	}

	auto extension = path.extension();
	if (extension == ".sequence") {
		
	}
}

