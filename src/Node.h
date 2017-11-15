#pragma once
#include <vector>
#include "rapidjson\fwd.h"



class SequenceNode {
public:
	void Serialize(rapidjson::Value& val, rapidjson::Document& doc) const;
	void Deserialize(rapidjson::Value& doc) ;
private:
	float m_time;
	std::string m_effect;
	float m_strength;
	float m_duration;
};

class PatternNode {
public:

	void Serialize(rapidjson::Value& val, rapidjson::Document& doc) const;
	void Deserialize(rapidjson::Value& doc);
	std::string Sequence() const { return m_sequence; }
private:
	float m_time;
	std::string m_sequence;
	std::string m_area;
	float m_strength;
};

class ExperienceNode {
public:
	
	
	void Serialize(rapidjson::Value& val, rapidjson::Document& doc) const;
	void Deserialize(rapidjson::Value& doc);
	std::string Pattern() const { return m_pattern; }
private:
	float m_time;
	float m_strength;
	std::string m_pattern;
	
};

using SequenceData = std::vector<SequenceNode>;
using PatternData = std::vector<PatternNode>;
using ExperienceData = std::vector<ExperienceNode>;


