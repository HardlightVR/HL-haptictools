#pragma once
#include <vector>
#include "rapidjson\fwd.h"
#include "HapticEffect.pb.h"
class INode {
	virtual void Serialize(rapidjson::Value& val, rapidjson::Document& doc) const = 0;
	virtual void Deserialize(rapidjson::Value& doc) = 0;
};

class SequenceNode : public INode{
public:
	SequenceNode();
	~SequenceNode();
	void Serialize(rapidjson::Value& val, rapidjson::Document& doc) const;
	void Deserialize(rapidjson::Value& doc);
	void Serialize(nsvr::detail::encoding::SequenceDefinitions& e) const;
	//void Serialize(nsvr::detail::encoding::Sequence& s) const;
private:
	float m_time;
	std::string m_effect;
	float m_strength;
	float m_duration;
};

class PatternNode: public INode {
public:
	PatternNode();
	~PatternNode();
	void Serialize(rapidjson::Value& val, rapidjson::Document& doc) const;
//	void Serialize(nsvr::detail::encoding::Pattern& p) const;
	void Serialize(nsvr::detail::encoding::PatternDefinitions& p) const;
	void Deserialize(rapidjson::Value& doc);
	std::string Sequence() const { return m_sequence; }
private:
	float m_time;
	std::string m_sequence;
	std::string m_area;
	float m_strength;
};

class ExperienceNode : public INode {
public:
	ExperienceNode();
	~ExperienceNode();
	void Serialize(nsvr::detail::encoding::ExperienceDefinitions& d) const;
	//void Serialize(nsvr::detail::encoding::Experience& e) const;
	void Serialize(rapidjson::Value& val, rapidjson::Document& doc) const;
	void Deserialize(rapidjson::Value& doc);
	std::string Pattern() const { return m_pattern; }
private:
	float m_time;
	//todo: make sure strength is implemented
	float m_strength;
	std::string m_pattern;
	
};

typedef std::vector<SequenceNode> SequenceData;
typedef std::vector<PatternNode> PatternData;
typedef std::vector<ExperienceNode> ExperienceData;
