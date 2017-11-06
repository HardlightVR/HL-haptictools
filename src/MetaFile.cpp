#include "stdafx.h"
#include "MetaFile.h"
#include "rapidjson\document.h"
#include <iostream>
MetaFile::MetaFile(HapticFileInfo info):
	m_rootEffect(info)
{
}


void MetaFile::Serialize(rapidjson::Document & doc) const
{
	using namespace rapidjson;
	assert(doc.IsObject());
	auto& alloc = doc.GetAllocator();

	Value seqDict(kObjectType);
	serialize(seqDict, m_sequences, doc);
	doc.AddMember("sequence_definitions", seqDict, alloc);

	Value patDict(kObjectType);
	serialize(patDict, m_patterns, doc);
	doc.AddMember("pattern_definitions", patDict, alloc);

	Value expDict(kObjectType);
	serialize(expDict, m_experiences, doc);
	doc.AddMember("experience_definitions", expDict, alloc);

	Value effect(kObjectType);
	effect.AddMember("name", StringRef(m_rootEffect.FullId), alloc);
	effect.AddMember("type", FileTypeString(m_rootEffect.FileType), alloc);

	doc.AddMember("root_effect", effect, alloc);
	doc.AddMember("version", 2, alloc);
	
}

void MetaFile::AddSequenceDefinition(std::string name, SequenceData data)
{
	m_sequences.emplace_back(name, data);
}

void MetaFile::AddPatternDefinition(std::string name, PatternData data)
{
	m_patterns.emplace_back(name, data);
}

void MetaFile::AddExperienceDefinition(std::string name, ExperienceData data)
{
	m_experiences.emplace_back(name, data);

}

nsvr::detail::encoding::RootEffect_Type GetFileType(HapticFileType type)
{
	switch (type) {
	case HapticFileType::Experience:
		return nsvr::detail::encoding::RootEffect_Type_EXPERIENCE;
	case HapticFileType::Pattern:
		return nsvr::detail::encoding::RootEffect_Type_PATTERN;
	case HapticFileType::Sequence:
		return nsvr::detail::encoding::RootEffect_Type_SEQUENCE;
	default:
		return nsvr::detail::encoding::RootEffect_Type_UNKNOWN;
	}
}
