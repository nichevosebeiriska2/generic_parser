#include "ParserBase.h"
#include "ParserOmited.h"
#include "ParserRepeate.h"
#include "ParserList.h"
#include "ParserWithAction.h"
#include "ParserSequence.h"
#include "parser_aliases.h"

#include <optional>

template<ConceptCharType CharType, ConceptParser ParserType, ConceptParser SkipperType>
auto ParseLexeme(const std::basic_string<CharType>& strInput, ParserType & parser, SkipperType & skipper)
{
	using namespace Parsers;
	// simple version of parsing function. This function will then have to handle errors.
	const CharType* strBegin = strInput.data();
	const CharType* strEnd = strInput.data() + strInput.length();

	return  parser.Parse(strBegin, strEnd, skipper) ? std::make_optional(parser.GetValueAndReset()) : std::nullopt;
}

template<ConceptCharType CharType, ConceptParser ParserType, ConceptParser SkipperType>
auto ParseLexeme(const std::basic_string<CharType>& strInput, ParserType& parser, SkipperType& skipper, auto&& action)
{
	using namespace Parsers;
	// simple version of parsing function. This function will then have to handle errors.
	const CharType* strBegin = strInput.data();
	const CharType* strEnd = strInput.data() + strInput.length();

	bool parsed = false;
	if (parsed = parser.Parse(strBegin, strEnd, skipper); parsed)
		action(parser.GetValueAndReset());
	
	return  parsed ? std::make_optional(parser.GetValueAndReset()) : std::nullopt;
}

struct SJsonArray
{
	std::vector<int> m_vec;
};

void main()
{
	using namespace Parsers;

	_string_lit parser(std::string_view{ "1" });
	_int parser_int{};
	repeate parser_repeate(_int{});
	list parser_list(_int{}, _string_lit{ std::string_view{","} }, 3);
	_string_lit parser_comma(std::string_view{ "," });
	_string_lit parser_br(std::string_view{ "[" });
	_string_lit parser_br_rev(std::string_view{ "]" });
	parser_with_action wrapper(parser_list, [](auto&& arg) {return SJsonArray{ arg }; });
	sequential seq(parser_br, wrapper, parser_br_rev);

	auto bbb2 = is_parser_v<int>;
	std::string str = "  [ 12 , 1 , 3]    ::[]::";
	auto r = ParseLexeme(str, seq, Skippers::space);

	//ParseLexeme(str, parser_list, Skippers::space, [](auto&& result) { return SJsonArray{ result }; });
		//auto r = ParseLexeme(str, parser_repeate, Skippers::space);
	//auto r = parser_rep.Parse(b, e, Skippers::space);

	int a = 1;
}