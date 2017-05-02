#pragma once
#include "rapidjson\fwd.h"
class IJsonSerializable
{
public:
	virtual ~IJsonSerializable(void) {};
	virtual void Serialize(const rapidjson::Value& root) = 0;
	virtual void Deserialize(const  rapidjson::Value& root) = 0;
};