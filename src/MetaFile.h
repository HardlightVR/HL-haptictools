#pragma once
#include "rapidjson\fwd.h"
#include "Node.h"
#include "HapticFileInfo.h"
#include "HapticEffect.pb.h"
#include <google/protobuf/map.h>
 template<class T>
 class KeyValue {
 public:
	 KeyValue(std::string key, T value);
	T Value;
	std::string Key;
 };

 nsvr::detail::encoding::RootEffect_Type GetFileType(HapticFileType type);
class MetaFile
{
public:
	MetaFile(HapticFileInfo rootEffect);
	void Serialize(rapidjson::Document & doc) const;


	void AddSequenceDefinition(std::string name, SequenceData data);
	void AddPatternDefinition(std::string name, PatternData data);
	void AddExperienceDefinition(std::string name, ExperienceData data);


private:

	HapticFileInfo m_rootEffect;
	std::vector<KeyValue<SequenceData>> m_sequences;
	std::vector<KeyValue<PatternData>> m_patterns;
	std::vector<KeyValue<ExperienceData>> m_experiences;
	

	template<class T>
	void serialize(rapidjson::Value& val, std::vector<KeyValue<T>>, rapidjson::Document& d) const;


	template<class HapticResult, class HapticType> 
	HapticResult GetData(std::string name, std::vector<KeyValue<HapticType>>& values) const;

	template<class HapticData>
	HapticData Find(std::string name, std::vector<KeyValue<HapticData>>& values) const;

	template<class HapticType, class HapticData>
	void SerializeFromKey(std::string name, HapticType* ptr, std::vector<KeyValue<HapticData>>& values);
};


template<class T>
struct effect_name_compare : public std::unary_function<KeyValue<T>, bool>
{
		explicit effect_name_compare(std::string key) : baseline(key) {}
	bool operator() (const KeyValue<T>& rhs)
	{
		return rhs.Key == baseline;
	}
		std::string baseline;
};

template<class T>
inline KeyValue<T>::KeyValue(std::string name, T data):Value(std::move(data)), Key(std::move(name))
{
}



template<class T>
inline void MetaFile::serialize(rapidjson::Value & val, std::vector<KeyValue<T>> data, rapidjson::Document& doc) const
{
	using namespace rapidjson;
	assert(val.IsObject());
	auto& alloc = doc.GetAllocator();

	for (const auto& item:data ) {
		Value seqArray(kArrayType);
		for (const auto& atom : item.Value) {
			Value seqAtom(kObjectType);
			atom.Serialize(seqAtom, doc);
			seqArray.PushBack(seqAtom, alloc);
		}
		val.AddMember(Value().SetString(item.Key, alloc).Move(), seqArray, alloc);
	}

}

template<class HapticResult, class HapticType>
inline HapticResult MetaFile::GetData(std::string name, std::vector<KeyValue<HapticType>>& values) const
{
	auto data = std::find_if(values.begin(), values.end(), effect_name_compare<HapticType>(name));
	HapticResult s;
	for (const auto& item : data->Value) {
		item.Serialize(s);
	}
	return s;
}

template<class HapticData>
HapticData MetaFile::Find(std::string name, std::vector<KeyValue<HapticData>>& values) const
{
	auto p = std::find_if(values.begin(), values.end(), effect_name_compare<HapticData>(name));
	return p.second->Value;
}

template<class HapticType, class HapticData>
inline void MetaFile::SerializeFromKey(std::string name, HapticType * ptr, std::vector<KeyValue<HapticData>>& values)
{
	auto data = std::find_if(values.begin(), values.end(), effect_name_compare<HapticData>(name));
	for (const auto& item : data->Value) {
		item.Serialize(*ptr);
	}
}
