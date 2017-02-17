#include "stdafx.h"
#include "Node.h"

#include "rapidjson\document.h"
#include <iostream>
#include "HapticDirectoryTools.h"
using namespace rapidjson;

SequenceNode::SequenceNode(){}
SequenceNode::~SequenceNode(){}
void SequenceNode::Serialize(rapidjson::Value & val, rapidjson::Document& doc) const
{
	assert(val.IsObject());
	auto& alloc = doc.GetAllocator();

	val.AddMember("effect", Value().SetString(m_effect, alloc).Move(), alloc);
	val.AddMember("duration", Value(m_duration), alloc);
	val.AddMember("strength", Value(m_strength), alloc);
	val.AddMember("time", Value(m_time), alloc);

}

void SequenceNode::Deserialize(rapidjson::Value& doc)
{
	assert(doc.IsObject());
	
	//Required keys
	m_effect = parseKeyOrThrow<std::string>(doc, "effect");
	m_time = parseKeyOrThrow<float>(doc, "time");

	//Here, we can supply defaults
	m_duration = parseKeyOrDefault<float>(doc, "duration", 0.0f);
	m_strength = parseKeyOrDefault<float>(doc, "strength", 1.0f);
}

void SequenceNode::Serialize(NullSpaceHaptics::SequenceDefinitions & s) const
{
	auto newEffect = s.mutable_effects()->Add();
	newEffect->set_strength(m_strength);
	newEffect->set_time(m_time);
	newEffect->set_duration(m_duration);
	newEffect->set_effect(m_effect);
}

void SequenceNode::Serialize(NullSpaceHaptics::Sequence& s) const
{
	auto newEffect = s.mutable_effects()->Add();
	newEffect->set_strength(m_strength);
	newEffect->set_time(m_time);
	newEffect->set_duration(m_duration);
	newEffect->set_effect(m_effect);
}

PatternNode::PatternNode(){}
PatternNode::~PatternNode(){}

void PatternNode::Serialize(rapidjson::Value & val, rapidjson::Document & doc) const
{
	assert(val.IsObject());
	auto& alloc = doc.GetAllocator();

	val.AddMember("sequence",Value().SetString(m_sequence, alloc).Move(), alloc);
	val.AddMember("area", Value().SetString(m_area, alloc).Move(), alloc);

	val.AddMember("strength", Value(m_strength), alloc);
	val.AddMember("time", Value(m_time), alloc);
}

void PatternNode::Serialize(NullSpaceHaptics::Pattern & p) const
{
	auto newSeq = p.mutable_sequences()->Add();
	//TODO: placeholder area for now
	newSeq->set_area(0);
	newSeq->set_strength(m_strength);
	newSeq->set_time(m_time);
	
}


void PatternNode::Serialize(NullSpaceHaptics::PatternDefinitions& p)
{
	auto newSeq = p.mutable_pattern_nodes()->Add();
	//todo: temporary before we convert string of area to actual number
	newSeq->set_area(0);
	newSeq->set_sequence(m_sequence);
	newSeq->set_strength(m_strength);
	newSeq->set_time(m_time);
	

	

}

void PatternNode::Deserialize(rapidjson::Value & doc)
{
	assert(doc.IsObject());

	//Required keys
	m_area = parseKeyOrThrow<std::string>(doc, "area");
	m_sequence = parseKeyOrThrow<std::string>(doc, "sequence");
	m_time = parseKeyOrThrow<float>(doc, "time");

	//Here, we can supply defaults
	m_strength = parseKeyOrDefault<float>(doc, "strength", 1.0f);
}

ExperienceNode::ExperienceNode(){}
ExperienceNode::~ExperienceNode(){}
void ExperienceNode::Serialize(NullSpaceHaptics::Experience & e) const
{
	auto newPat = e.mutable_patterns()->Add();
	newPat->set_strength(m_strength);
	newPat->set_time(m_time);
}
void ExperienceNode::Serialize(rapidjson::Value & val, rapidjson::Document & doc) const
{
	assert(val.IsObject());
	auto& alloc = doc.GetAllocator();
	val.AddMember("pattern", Value().SetString(m_pattern, alloc).Move(), alloc);
	val.AddMember("time", Value(m_time), alloc);

}

void ExperienceNode::Deserialize(rapidjson::Value & doc)
{
	assert(doc.IsObject());
	m_time = parseKeyOrThrow<float>(doc, "time");
	m_strength = parseKeyOrDefault(doc, "strength", 1.0);
	m_pattern = parseKeyOrThrow<std::string>(doc, "pattern");
}
