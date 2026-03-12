#pragma once

#include "common.h"

#include <vector>
#include <map>
#include <string>
#include <variant>

#include "rule.h"
#include "base_parser.h"
#include "parser_predicate_not.h"
#include "list.h"
#include "sequence.h"
#include "alternative.h"

struct SJsonObject;
struct SJsonArray;

struct SJsonValue
{
	std::variant<std::monostate, bool, int, float, std::string, SJsonArray *, SJsonObject *> m_value;
};

struct SJsonObject
{
	std::map<std::string, SJsonValue> m_values;
};
struct SJsonArray
{
	std::vector<SJsonValue> m_values;
};

RuleNew<class tag_value, SJsonValue> value;
RuleNew<class tag_object, SJsonValue> object;
RuleNew<class tag_array, SJsonValue> array;

auto action_on_null = [](auto &ctx)
{
	return std::monostate{};
};
auto action_on_bool = [](auto &ctx)
{
	return true;
};
auto action_on_str = [](auto &ctx)
{
	return std::string{ctx.Begin(), ctx.End()};
};

auto action_on_array = [](auto &ctx)
{
	auto pArray = new SJsonArray;
	for(auto &value : std::get<1>(ctx.GetValue()))
		pArray->m_values.emplace_back(std::move(value));

	return SJsonValue{pArray};
};

auto action_on_object = [](auto &ctx)
{
	auto pObject = new SJsonObject;
	for(auto [name, unused, value] : std::get<1>(ctx.GetValue()))
		pObject->m_values[std::move(name)] = SJsonValue{std::move(value)};

	return SJsonValue{pObject};
};

auto action_on_value = [](auto &ctx)
{
	return std::visit([](auto &&arg)
	{
		return SJsonValue{arg};
	}, ctx.GetValue());
};


auto parser_null = (ParserLiteralWithContext{"null"})[action_on_null];
auto parser_bool = (ParserLiteralWithContext{"true"} |  ParserLiteralWithContext{"false"})[action_on_bool];
auto parser_int = aliases::int_;
auto parser_float = aliases::float_;
auto parser_string = ("\"" >> (*(!ParserLiteralWithContext{"\""} >> ("\\\"" | aliases::char_any))) >> "\"")[action_on_str];

auto parser_object = "{" >> (*((parser_string >> ":" >> value) % ",")) >> "}";
auto parser_array = "[" >> (*(value % ",")) >> "]";
auto parser_value = object | array | parser_null | parser_bool  | parser_float | parser_int | parser_string;

IMPLEMENT_RULE(object, parser_object[action_on_object]);
IMPLEMENT_RULE(array, parser_array[action_on_array]);
IMPLEMENT_RULE(value, parser_value[action_on_value]);
