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

#include "Json/ExampleJson.h"

void main()
{
	using namespace Parsers;

	std::string strBool = 
	R"({ 
		NAME = 123;
		object = {
			name = [a,			b ,                     c];
			anotherobject = { somevalue = 123;};
			emptyone = {};
		};
	};)";
	//	R"({ 
	//	NAME = 123;
	//		emptyone = {};
	//};)";
	auto space_newline = Skippers::space | Skippers::newline | Skippers::tab;
	auto res = ParseLexeme(strBool, value, space_newline);
	std::string str = "{ a = 1 }";

}