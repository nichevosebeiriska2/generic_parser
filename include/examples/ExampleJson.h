#pragma once

#include "common.h"

#include <vector>
#include <map>
#include <string>
#include <variant>

struct SJsonValue;

struct SJsonBool
{
	enum EValue { TRUE, FALSE } m_value;
};

struct SJsonNull
{

};

struct SJsonString
{
	std::string m_str;
};

struct SJsonInt
{
	INT m_value{ 0 };
};

struct SJsonFloat
{
	float m_value{ 0 };
};

struct SJsonObject;
struct SJsonArray;

struct SJsonValue
{
	std::variant<SJsonNull, SJsonBool, SJsonFloat, SJsonInt, SJsonString, SJsonObject*, SJsonArray*> m_value;
};

struct SJsonObject
{
	std::map<std::string, SJsonValue> m_mapValues;
};

struct SJsonArray
{
	std::vector<SJsonValue> m_vecValues;
};

