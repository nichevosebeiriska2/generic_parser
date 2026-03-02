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


#include <fstream>

std::string read_file(std::string strPathToFile)
{
	std::ifstream file(strPathToFile);

	if (file.is_open())
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{} };

	return "";
}

template<ConceptParser TParser>
bool f(TParser&& parser)
{
	return false;
}


template<ConceptScanner TScanner>
void f_scanner(TScanner scanner)
{

}
void main()
{
	using namespace Parsers;

	using namespace Scanners;

	f(_string_lit{"1"});
	f(_int{});

	f(_int{} >> _int{}>> ")");
	f(*_int{});
	f(_int{} % ",");
	f(omit(_int{}));
	f(_int{} | _float{});

	f_scanner(Scanners::_isalpha_str{});
	Scanners::_isalpha_str aa;
	const char *str_alpha = "abc1ds";
	const wchar_t *wstr_alpha = L"ab1cds";
	auto b1 =aa.Scan(str_alpha, str_alpha + 5);
	auto b2 = aa.Scan(wstr_alpha, wstr_alpha + 5);
	std::string strJson = read_file("C:\\Users\\dmileykin\\source\\repos\\generic_parser\\include\\examples\\Json\\medium.txt");
	auto res = ParseLexeme(strJson, value, Skippers::space);

	int a = 1;
}