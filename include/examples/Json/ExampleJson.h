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

struct SJsonValue	{ std::variant<std::monostate, bool, int, float, std::string, SJsonObject*, SJsonArray*> m_value; };
struct SJsonObject	{ std::map<std::string, SJsonValue> m_mapValues; };
struct SJsonArray	{ std::vector<SJsonValue> m_vecValues; };

// declare rules for json parser
// tag1,tag2, tag3 is just a template parameter to differ ParseRule-s with same return type;
ParseRule<class tag1, SJsonValue>	value;
ParseRule<class tag2, SJsonValue>	arr;
ParseRule<class tag3, SJsonValue>	object;

using namespace Parsers;

// actions
auto return_null = []() { return std::monostate{}; };
auto return_bool_true = []() { return true; };
auto return_bool_false = []() { return true; };
auto return_bool = [](auto&& json_bool_variant_value) {return std::visit([](auto&& bool_value) {return bool_value; }, json_bool_variant_value); };
auto return_raw_string = [](auto&& variant_pair_pointers) {
		auto [ptr_begin, ptr_end] = std::get<0>(variant_pair_pointers);
		return std::string{ ptr_begin , ptr_end };
	};


auto return_json_obj = [](auto&& tuple_pairs_name_value)
	{
		auto* pObject = new SJsonObject;

		for (auto& [name, value] : std::get<0>(tuple_pairs_name_value))
			pObject->m_mapValues[name] = std::move(value);

		return SJsonValue{ pObject };
	};

auto return_json_array = [](auto&& tuple) { return SJsonValue{ new SJsonArray{ std::get<0>(tuple) } }; };
auto return_json_value = [](auto&& arg) { return std::visit([](auto&& jsonValue) {return SJsonValue{ jsonValue }; }, arg); };

// \ actions

auto parser_json_null = _string_lit{ "null" } [return_null];
auto parser_json_bool = (_string_lit{ "true" } [return_bool_true] | _string_lit{ "false" } [return_bool_false]) [return_bool];

// nothing special
auto parser_json_int		= _int{};
auto parser_json_float		= _float{};

auto parser_escape_symbol	= _string_lit{ "\n" } | "\t" | "\r";
auto parser_escaped_quote	= _string_lit{ "\\\"" };
auto parser_not_quotes		= !(_string_lit{ "\"" });
auto parser_string			= (*(parser_not_quotes >> (parser_escaped_quote | parser_escape_symbol | _char{}))) ; // if next symol is not " try to parse escaped quotes or escape symbol or any other char
auto parser_json_string		= raw((("\"" >> parser_string >> "\"") ))[return_raw_string];

// array
auto parser_json_array = ("[" >> (*(value % ",")) >> "]") [return_json_array];
// object
auto parser_json_object = ("{" >> *((parser_json_string >> ":" >> value) % ",") >> "}")[return_json_obj];
// value
auto parser_json_value = (parser_json_null | parser_json_bool  | parser_json_float | parser_json_int | parser_json_string | parser_json_array | parser_json_object)[return_json_value];


// implement parsing rules
ImplementParsingRule(value, parser_json_value);
ImplementParsingRule(arr, parser_json_array);
ImplementParsingRule(object, parser_json_object);