#include "ParserBase.h"
#include "ParserOmited.h"
#include "ParserRepeate.h"
#include "ParserList.h"
#include "ParserWithAction.h"
#include "ParserSequence.h"
#include "ParserAlternative.h"
#include "ParseRuleDeclaration.h"

#include "Operators.h"

#include "parser_aliases.h"
#include "ExampleJson.h"

#include <optional>
#include <map>

template<ConceptCharType CharType, ConceptParser ParserType, ConceptParser SkipperType>
auto ParseLexeme(const std::basic_string<CharType>& strInput, ParserType & parser, SkipperType & skipper)
{
	using namespace Parsers;
	// simple version of parsing function. This function will then have to handle errors.
	const CharType* strBegin = strInput.data();
	const CharType* strEnd = strInput.data() + strInput.length();

	
	if constexpr (is_unused_type_v<typename ParserType::parsing_attribute>)
		return parser.Scan(strBegin, strEnd, skipper);
	else
		return  parser.Parse(strBegin, strEnd, skipper) ? std::make_optional(parser.GetValueAndReset()) : std::nullopt;
}

// declarations
//ParseRule<SJsonValue>	value;
//ParseRule<SJsonArray*>	arr;
//ParseRule<SJsonObject*>	object;

//using t = decltype(arr);
//template<>
//template<ConceptCharType CharType, typename TParserSkipper>
//bool ParseRuleDeclaration<SJsonArray*>::Parse(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper)
//{
//	return false;
//}

//template<>
//template<ConceptCharType CharType>
//bool ParseRuleDeclaration<SJsonArray*>::Parse(const CharType*& ptr_string, const CharType*& ptr_string_end)
//{
//	return false;
//}

using namespace Parsers;

//auto parser_json_null = ParserWrapperWithAction(_string_lit{ std::string_view{"Null"} }, []() {return SJsonNull(); });
//
//auto parser_json_bool_true = ParserWrapperWithAction(_string_lit{ std::string_view{"True"} }, []() {return SJsonBool{ SJsonBool::TRUE }; });
//auto parser_json_bool_false = ParserWrapperWithAction(_string_lit{ std::string_view{"False"} }, []() {return SJsonBool{ SJsonBool::FALSE }; });
//auto parser_json_bool = ParserWrapperWithAction(parser_json_bool_true /*| parser_json_bool_false*/, [](auto&& arg) {return SJsonBool{ arg.m_value }; });
//
//auto parser_json_value_int = ParserWrapperWithAction(_int{}, [](int arg) {return SJsonInt{ arg }; });
//auto parser_json_value_float = ParserWrapperWithAction(_float{}, [](float arg) {return SJsonFloat{ arg }; });
//auto parser_json_value_string = ParserWrapperWithAction(_string{}, [](std::string&& arg) {return SJsonString{ arg }; });
//
//auto parser_array = "[" >> (value % _string_lit{ std::string_view{","} }) >> "]";
//auto parser_object = "{" >> (value % _string_lit{ std::string_view{","} }) >> "}";
//
//auto parser_json_array = ParserWrapperWithAction(parser_array, [](auto&& arg)
//	{
//		auto arr = new SJsonArray;
//
//		for (auto& el : std::get<0>(arg))
//			arr->m_vecValues.emplace_back(el);
//
//		return arr;
//	});
//
//auto parser_json_object = ParserWrapperWithAction(parser_object, [](auto&& arg)
//	{
//		auto obj = new SJsonObject;
//
//		//for (auto& el : std::get<0>(arg))
//			//arr.m_vecValues.emplace_back(el);
//
//		return obj;
//	});
//
//auto parser_json_value = ParserWrapperWithAction(parser_json_null | parser_json_bool  | parser_json_value_float | parser_json_value_int | parser_json_value_string  | object | arr
//	, [](const auto& arg) {SJsonValue value; value.m_value = arg; return value; });
//
//ImplementParsingRule(arr, parser_json_array);
//ImplementParsingRule(value, parser_json_value);
//ImplementParsingRule(object, parser_json_object);

#include "Json/ExampleJson.h"

void main()
{
	using namespace Parsers;

	std::string strBool = 
	R"({ 
		NAME = 123;
		obj = {
			c = [1,somestring,3];
		};
		empty = {};
	};)";
	auto space_newline = Skippers::space | Skippers::newline | Skippers::tab;
	auto res = ParseLexeme(strBool, value, space_newline);
	std::string str = "{ a = 1 }";

}