#pragma once
#include "rapidjson\fwd.h"
#include "Node.h"
#include "HapticFileInfo.h"
#include "HapticEffect.pb.h"
 template<class T>
 class DataTuple {
 public:
	 DataTuple(std::string name, T data);
	~DataTuple();
	T Data;
	std::string Key;
 };
class MetaFile
{
public:
	MetaFile(HapticFileInfo rootEffect);
	~MetaFile();
	void Serialize(rapidjson::Document & doc) const;


	void AddSequenceDefinition(std::string name, SequenceData data);
	void AddPatternDefinition(std::string name, PatternData data);
	void AddExperienceDefinition(std::string name, ExperienceData data);
	NullSpaceHaptics::HapticFile ToBinary();
	NullSpaceHaptics::HapticDescriptionFile ToHDF();

private:
	//void Traverse()
	//NullSpaceHaptics::Pattern GetPattern(std::string name);
	HapticFileInfo m_rootEffect;
	std::vector<DataTuple<SequenceData>> m_sequences;
	std::vector<DataTuple<PatternData>> m_patterns;
	std::vector<DataTuple<ExperienceData>> m_experiences;

	template<class T>
	void serializeProto(NullSpaceHaptics::HapticDescriptionFile& hf, std::vector<DataTuple<T>>) const;
	template<class T>
	void serialize(rapidjson::Value& val, std::vector<DataTuple<T>>, rapidjson::Document& d) const;


	template<class HapticResult, class HapticType> 
	HapticResult GetData(std::string name, std::vector<DataTuple<HapticType>>& values) const;

	template<class HapticData>
	HapticData Find(std::string name, std::vector<DataTuple<HapticData>>& values) const;

	template<class HapticType, class HapticData>
	void SerializeFromKey(std::string name, HapticType* ptr, std::vector<DataTuple<HapticData>>& values);
};


template<class T>
struct effect_name_compare : public std::unary_function<DataTuple<T>, bool>
{
		explicit effect_name_compare(std::string key) : baseline(key) {}
	bool operator() (const DataTuple<T>& rhs)
	{
		return rhs.Key == baseline;
	}
		std::string baseline;
};

template<class T>
inline DataTuple<T>::DataTuple(std::string name, T data):Data(data), Key(name)
{
}

template<class T>
inline DataTuple<T>::~DataTuple()
{
}

template<class T>
inline void MetaFile::serializeProto(NullSpaceHaptics::HapticDescriptionFile & hf, std::vector<DataTuple<T>> data) const
{
	for (const auto& item : data) {
		for (const auto& atom : item.Data) {

		}
	}

}

template<class T>
inline void MetaFile::serialize(rapidjson::Value & val, std::vector<DataTuple<T>> data, rapidjson::Document& doc) const
{
	using namespace rapidjson;
	assert(val.IsObject());
	auto& alloc = doc.GetAllocator();

	for (const auto& item:data ) {
		Value seqArray(kArrayType);
		for (const auto& atom : item.Data) {
			Value seqAtom(kObjectType);
			atom.Serialize(seqAtom, doc);
			seqArray.PushBack(seqAtom, alloc);
		}
		val.AddMember(Value().SetString(item.Key, alloc).Move(), seqArray, alloc);
	}

}

template<class HapticResult, class HapticType>
inline HapticResult MetaFile::GetData(std::string name, std::vector<DataTuple<HapticType>>& values) const
{
	auto data = std::find_if(values.begin(), values.end(), effect_name_compare<HapticType>(name));
	HapticResult s;
	for (const auto& item : data->Data) {
		item.Serialize(s);
	}
	return s;
}

template<class HapticData>
HapticData MetaFile::Find(std::string name, std::vector<DataTuple<HapticData>>& values) const
{
	auto p = std::find_if(values.begin(), values.end(), effect_name_compare<HapticData>(name));
	return (*p).Data;
}

template<class HapticType, class HapticData>
inline void MetaFile::SerializeFromKey(std::string name, HapticType * ptr, std::vector<DataTuple<HapticData>>& values)
{
	auto data = std::find_if(values.begin(), values.end(), effect_name_compare<HapticData>(name));
	for (const auto& item : data->Data) {
		item.Serialize(*ptr);
	}
}
