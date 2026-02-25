#pragma once

#include "ParserBase.h"

namespace Parsers
{
	using parser_float = Parser<Scanners::scanner_float, float>;
	using parser_int = Parser<Scanners::scanner_int, int>;

	using _float = parser_float;
	using _int = parser_int;

	template<ConceptCharType CharType = char>
	using _symbol = Parser<Scanners::scanner_symbol, CharType>;

	template<ConceptCharType CharType = char>
	using _string = Parser<Scanners::scanner_string_alnum, std::basic_string<CharType>>;

	template<ConceptCharType CharType>
	using _char = Parser<Scanners::scanner_string_alnum, CharType>;

	template<ConceptCharType CharType>
	using _string_lit = ParserLiteral<CharType>;
};

namespace Skippers
{
	auto space = Parsers::_string_lit{ std::string_view{" "} };
	auto newline = Parsers::_string_lit{ std::string_view{"\n"} };
	auto tab = Parsers::_string_lit{ std::string_view{"\t"} };
	//constexpr auto space_newline = space | newline;
}