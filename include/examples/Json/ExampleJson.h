#pragma once

#include "common.h"

#include <vector>
#include <map>
#include <string>
#include <variant>

#include "ParseRuleDeclaration.h"
#include "ParserWithAction.h"
#include "Operators.h"

struct SJsonObject;
struct SJsonArray;

struct SJsonValue
{
	std::variant<std::monostate, bool, int, float, std::string, SJsonObject*, SJsonArray*> m_value;
};

struct SJsonObject
{
	std::map<std::string, SJsonValue> m_mapValues;
};

struct SJsonArray
{
	std::vector<SJsonValue> m_vecValues;
};

// declare rules for json parser
// tag1,tag2, tag3 is just a template parameter to differ ParseRule-s with same return type;
ParseRule<class tag1, SJsonValue>	value;
ParseRule<class tag2, SJsonValue>	arr;
ParseRule<class tag3, SJsonValue>	object;

using namespace Parsers;


auto parser_json_null = _string_lit{ "null" }([]() { return SJsonValue{ std::monostate{} }; });

// parser for bool values (true/false)
auto parser_json_bool = parser_with_action(
	_string_lit{ "true" } ( [](){return SJsonValue{true};}) 
	| _string_lit{ "false" }([]() {return SJsonValue{ false }; })
	, [](auto&& json_bool_variant_value) {return std::visit([](auto&& arg) {return SJsonValue{ arg }; }, json_bool_variant_value); }); // there is one of the weakest things about my parser 
// - you should really be aware about whar you are doing with types. 'alternative' parser returns std::variant and you should properly handle its result 
// dk what to do about it, but the same problem boost spirit has too. std::variant is not the easiest type to handle its template deduction errors. 
// but we have what we have

// nothing special
auto parser_json_int		= _int{}([](int i) { return SJsonValue{ i }; });
auto parser_json_float	= _float{}([](float f) { return SJsonValue{ f }; });
auto parser_quoted_string = "\"" >> _string{} >> "\"";
auto parser_json_string	= (parser_quoted_string)([](/*std::string &&s*/auto && tup) { return SJsonValue{std::move(std::get<0>(tup))}; });

// array
auto parser_json_array = ("[" >> (*(value % ",")) >> "]") ( [](auto&& tuple) {
	auto* pArray = new SJsonArray;

	for (auto& value : std::get<0>(tuple))
		pArray->m_vecValues.emplace_back(std::move(value));

	return SJsonValue{ pArray };
	});

// object
auto parser_values_map = *(parser_quoted_string >> "=" >> value >> ";"); // zero or more fields
auto parser_json_object = ("{" >> parser_values_map >> "}")(
	[](auto&& tuple_pairs_name_value)
	{
		auto* pObject = new SJsonObject;

		for (auto& [name, value] : std::get<0>(tuple_pairs_name_value))
			pObject->m_mapValues[name] = value;

		return SJsonValue{ pObject };
	}
);

auto parser_json_value = (parser_json_null | parser_json_bool  | parser_json_float | parser_json_int | parser_json_string | parser_json_array | parser_json_object)(
	 [](auto&& arg) { return std::visit([](auto&& jsonValue) {return SJsonValue{jsonValue}; }, arg); }
);

// implement parsing rules
ImplementParsingRule(value, parser_json_value);
ImplementParsingRule(arr, parser_json_array);
ImplementParsingRule(object, parser_json_object);
