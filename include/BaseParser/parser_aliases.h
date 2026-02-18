#pragma once

#include "ParserBase.h"
//#include "ParserOmited.h"

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
	using _char = ParserLiteral<CharType, Scanners::scanner_string_alnum, std::basic_string<CharType>>;

	//template<ConceptCharType CharType>
	//using _string_lit = omited<ParserLiteral<CharType, Scanners::scanner_string_literal, std::basic_string<CharType>>>;

	//template<ConceptCharType CharType>
	//using _char_lit = omited<ParserLiteral<CharType, Scanners::scanner_symbol, CharType>>;

	//template<ConceptCharType CharType>
	//omited(CharType symbol) -> omited<ParserLiteral<CharType, Scanners::scanner_symbol, CharType>>;

	//template<ConceptCharType CharType>
	//omited(const CharType *string_literal) -> omited<ParserLiteral<CharType, Scanners::scanner_string_literal, std::basic_string<CharType>>>;
};