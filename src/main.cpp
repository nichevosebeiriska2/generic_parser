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

using namespace Parsers;

//#include "json_example_input.h"

void main()
{
	using namespace Parsers;


	std::string str_json2 = std::string(R"(" 123\" " 123213)");
	std::wstring str_float = L" 1.234e3f";
	float f = 0;
	wchar_t* b = str_float.data();
	 wchar_t* e = b + str_float.length();

	auto r = std::wcstof(b, &e);
	//auto res = ParseLexeme(str_json_small, value, Skippers::space);

	int a = 1;
}