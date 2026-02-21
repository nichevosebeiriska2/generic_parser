#include "ParserBase.h"
#include "ParserOmited.h"
#include "ParserRepeate.h"
#include "parser_aliases.h"

#include <optional>

template<ConceptCharType CharType, ConceptParser ParserType, ConceptParser SkipperType>
auto ParseLexeme(const std::basic_string<CharType>& strInput, ParserType & parser, SkipperType & skipper)
{
	using namespace Parsers;
	// simple version of parsing function. This function will then have to handle errors.
	const CharType* strBegin = strInput.data();
	const CharType* strEnd = strInput.data() + strInput.length();

	bool bscanned = ScanStringForLiteralString(strBegin, strEnd, " ");
	//repeate parser_repeate(_int{}); 
	//auto r = parser_repeate.Parse(strBegin, strEnd, skipper);
	return  parser.Parse(strBegin, strEnd, skipper) ? std::make_optional(parser.GetValueAndReset()) : std::nullopt;
}

void main()
{
	using namespace Parsers;

	_string_lit parser(std::string_view{ "1" });
	_int parser_int{};
	repeate parser_repeate(_int{});
	//is_specialization_of < _int > f;
	auto bbb2 = is_parser_v<int>;
	std::string str = "  12 1 3";
	auto r = ParseLexeme(str, parser_repeate, Skippers::space);
		//auto r = ParseLexeme(str, parser_repeate, Skippers::space);
	//auto r = parser_rep.Parse(b, e, Skippers::space);

	int a = 1;
}