#pragma once

#include "ParserBase.h"

namespace Parsers
{
	using parser_float = Parser<Scanners::scanner_float<char>, float>;
	using parser_int = Parser<Scanners::scanner_int<char>, int>;

	using _float = parser_float;
	using _int = parser_int;

	namespace type_char
	{
		using _char = Parser<Scanners::ScannerCharAny<char>, char>;
		using _char_alpha = Parser<Scanners::_isalpha, char>;
		using _char_alnum = Parser<Scanners::_isalnum, char>;
		using _char_digit = Parser<Scanners::_isdigit, char>;
		using _char_blank = Parser<Scanners::_isblank, char>;
		using _char_cntrl = Parser<Scanners::_iscntrl, char>;
		using _char_graph = Parser<Scanners::_isgraph, char>;
		using _char_print = Parser<Scanners::_isprint, char>;
		using _char_punct = Parser<Scanners::_ispunct, char>;
		using _char_space = Parser<Scanners::_isspace, char>;
		using _char_xdigit = Parser<Scanners::_isxdigit, char>;
		using _char_lower = Parser<Scanners::_islower, char>;
		using _char_upper = Parser<Scanners::_isupper, char>;
	};

	using _wchar = Parser<Scanners::ScannerCharAny<char>, wchar_t>;

	template<ConceptCharType CharType>
	using _string_lit = ParserLiteral<CharType, Scanners::ScannerStringLiteral>;

	template<ConceptCharType CharType>
	using _string_lit_raw = ParserLiteral<CharType, Scanners::ScannerStringLiteralRaw>;
};