#include "stdafx.h"
#include "MetaFile.h"
#include "rapidjson\document.h"
#include <iostream>
MetaFile::MetaFile(HapticFileInfo info):
	m_rootEffect(info)
{
}



MetaFile::~MetaFile()
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
	
}
//nsvr::detail::encoding::HapticDescriptionFile MetaFile::ToHDF()
//{
//	nsvr::detail::encoding::HapticDescriptionFile h;
//	serializeProtoT(*h.mutable_sequence_definitions(), m_sequences);
//	serializeProtoT(*h.mutable_pattern_definitions(), m_patterns);
//	serializeProtoT(*h.mutable_experience_definitions(), m_experiences);
//	
//	h.mutable_effect()->set_name(m_rootEffect.FullId);
//	h.mutable_effect()->set_type(GetFileType(m_rootEffect.FileType));
//	return h;
//}
void MetaFile::AddSequenceDefinition(std::string name, SequenceData data)
{
	m_sequences.push_back(DataTuple<SequenceData>(name, data));
}

void MetaFile::AddPatternDefinition(std::string name, PatternData data)
{
	m_patterns.push_back(DataTuple<PatternData>(name, data));
}

void MetaFile::AddExperienceDefinition(std::string name, ExperienceData data)
{
	m_experiences.push_back(DataTuple<ExperienceData>(name, data));

}

//nsvr::detail::encoding::HapticFile MetaFile::ToBinary()
//{
//	nsvr::detail::encoding::HapticFile f;
//
//	if (m_rootEffect.FileType == HapticFileType::Sequence) {
//		auto sequence = GetData<nsvr::detail::encoding::Sequence, SequenceData>(m_rootEffect.FullId, m_sequences);
//		//defaults
//		sequence.set_strength(1.0);
//		sequence.set_area(0);
//		f.set_type(nsvr::detail::encoding::HapticFile_Type_SEQUENCE);
//		f.mutable_sequence()->CopyFrom(sequence);
//		return f;
//	}
//	else if (m_rootEffect.FileType == HapticFileType::Pattern) {
//		nsvr::detail::encoding::HapticFile f;
//		auto actualData = Find<PatternData>(m_rootEffect.FullId, m_patterns);
//		auto pattern = GetData<nsvr::detail::encoding::Pattern, PatternData>(m_rootEffect.FullId, m_patterns);
//		pattern.set_strength(1.0);
//
//		for (const auto& p : actualData) {
//			
//			SerializeFromKey<nsvr::detail::encoding::Sequence, SequenceData>(p.Sequence(), pattern.add_sequences(), m_sequences);
//
//		}
//		std::cout << pattern.DebugString() << '\n';
//		f.mutable_pattern()->CopyFrom(pattern);
//
//		f.set_type(nsvr::detail::encoding::HapticFile_Type_PATTERN);
//		return f;
//
//	}
//	
//	return nsvr::detail::encoding::HapticFile();
//}
//




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
