#pragma once
#include "rapidjson\fwd.h"
#include "Node.h"
#include "HapticFileInfo.h"
#include "HapticEffect.pb.h"
#include <google/protobuf/map.h>
 template<class T>
 class DataTuple {
 public:
	 DataTuple(std::string name, T data);
	~DataTuple();
	T Data;
	std::string Key;
 };

 nsvr::detail::encoding::RootEffect_Type GetFileType(HapticFileType type);
class MetaFile
{
public:
	MetaFile(HapticFileInfo rootEffect);
	~MetaFile();
	void Serialize(rapidjson::Document & doc) const;


	void AddSequenceDefinition(std::string name, SequenceData data);
	void AddPatternDefinition(std::string name, PatternData data);
	void AddExperienceDefinition(std::string name, ExperienceData data);
//	nsvr::detail::encoding::HapticFile ToBinary();
	nsvr::detail::encoding::HapticDescriptionFile ToHDF();

private:

	//void Traverse()
	//nsvr::detail::encoding::Pattern GetPattern(std::string name);
	HapticFileInfo m_rootEffect;
	std::vector<DataTuple<SequenceData>> m_sequences;
	std::vector<DataTuple<PatternData>> m_patterns;
	std::vector<DataTuple<ExperienceData>> m_experiences;

	void serializeProto(const google::protobuf::Map<std::string, nsvr::detail::encoding::SequenceDefinitions> definitions, std::vector<DataTuple<SequenceData>> data) const;
	

	template<class DefinitionType, class DataType>
	void serializeProtoT(google::protobuf::Map<std::string, DefinitionType>& definitions, std::vector<DataTuple<DataType>>& data) const;
	
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


inline void MetaFile::serializeProto(google::protobuf::Map<std::string, nsvr::detail::encoding::SequenceDefinitions> definitions, std::vector<DataTuple<SequenceData>> data) const
{
	
	//so, sequence_definitions is a map from string -> a sequence definition
	for (const auto& dataTuple : data) {
		//for each definition, we want to add a new map entry, which will be dataTuple.Data
		nsvr::detail::encoding::SequenceDefinitions sd;

		for (const auto& sequenceData : dataTuple.Data) {
			sequenceData.Serialize(sd);
		}
	//	definitions.insert(std::make_pair(dataTuple.Key, sd));
	}

}
template<class DefinitionType, class DataType>
inline void MetaFile::serializeProtoT(google::protobuf::Map<std::string, DefinitionType>& definitions, std::vector<DataTuple<DataType>>& data) const
{

	//so, sequence_definitions is a map from string -> a sequence definition
	for (const auto& dataTuple : data) {
		//for each definition, we want to add a new map entry, which will be dataTuple.Data
		DefinitionType sd;

		for (const auto& sequenceData : dataTuple.Data) {
			sequenceData.Serialize(sd);
		}
		definitions.insert(google::protobuf::MapPair<std::string, DefinitionType>(dataTuple.Key, sd));
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
