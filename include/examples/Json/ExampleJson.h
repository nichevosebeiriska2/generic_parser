#pragma once

#include "common.h"

#include <vector>
#include <map>
#include <string>
#include <variant>

#include "ParseRuleDeclaration.h"
#include "ParserWithAction.h"
#include "Operators.h"

struct SJsonValue;


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
ParseRule<class tag1, SJsonValue>	value;
ParseRule<class tag2, SJsonValue>	arr;
ParseRule<class tag3, SJsonValue>	object;

using namespace Parsers;


auto parser_json_null = parser_with_action(_string_lit{ std::string_view{"NULL"} }, []() {return SJsonValue{ std::monostate{} }; });
// parser for bool values (true/false)
auto parser_json_bool = parser_with_action( parser_with_action(_string_lit{ std::string_view{"True"} }, [](){return SJsonValue{true};})
									 | parser_with_action(_string_lit{ std::string_view{"False"} }, []() {return SJsonValue{ false }; })
	, [](auto&& json_bool_variant_value) {return std::visit([](auto&& arg) {return SJsonValue{ arg }; }, json_bool_variant_value); }); // there is one of the weakest things about my parser 
// - you should really be aware about whar you are doing with types. 'alternative' parser returns std::variant and you should properly handle its result 
// dk what to do about it, but the same problem boost spirit has too. std::variant is not the easiest type to handle its template deduction errors. 
// but we have what we have

// nothing special
auto parser_json_int		= parser_with_action(_int{}, [](int&& i) {return SJsonValue{ i }; });
auto parser_json_float	= parser_with_action(_float{}, [](float&& i) {return SJsonValue{ i }; });
auto parser_json_string	= parser_with_action(_string{}, [](std::string&& i) {return SJsonValue{ i }; });

// array
auto parser_comma = _string_lit{ std::basic_string_view{","} };
auto parser_list_of_values = (value % parser_comma);
auto parser_json_array = parser_with_action("[" >> parser_list_of_values >> "]" , [](auto&& tuple) {
	auto* pArray = new SJsonArray;

	for (auto& value : std::get<0>(tuple))
		pArray->m_vecValues.emplace_back(std::move(value));

	return SJsonValue{ pArray };
	});

// object
auto parser_values_map = *(_string{} >> "=" >> value >> ";"); // zero or more fields
auto parser_json_object = parser_with_action("{" >> parser_values_map >> "}"
	, [](auto&& tuple_pairs_name_value)
	{
		auto* pObject = new SJsonObject;

		for (auto& [name, value] : std::get<0>(tuple_pairs_name_value))
			pObject->m_mapValues[name] = value;

		return SJsonValue{ pObject };
	}
);

auto parser_json_value = parser_with_action(parser_json_null | parser_json_bool  | parser_json_float | parser_json_int | parser_json_string | parser_json_array | parser_json_object
	, [](auto&& arg)
	{
		return std::visit([](auto&& jsonValue) {return jsonValue; }, arg);
	}
);

// implement parsing rules
ImplementParsingRule(value, parser_json_value);
ImplementParsingRule(arr, parser_json_array);
ImplementParsingRule(object, parser_json_object);
