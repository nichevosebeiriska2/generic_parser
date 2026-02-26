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

void main()
{
	using namespace Parsers;

	std::string strBool = 
	R"({ 
		"NAME" = 123;
		"NAME2" = "123";

		"array" = [1,2,3, 
								[1,2,3],
								["s1", "s2"],
								[true, true ,null]
							];
		"object" = {"almnlnlnlnljljlkjljljljlj" = { "b" = {};};};
	};)";
	
	auto res = ParseLexeme(strBool, value, Skippers::space);

	int a = 1;
}