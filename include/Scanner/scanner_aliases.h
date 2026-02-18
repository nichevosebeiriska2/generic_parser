#pragma once
#include "Scanners.h"

namespace Scanners
{
	template<ConceptCharType CharType>
	using scanner_int = ScannerInteger<CharType>;

	template<ConceptCharType CharType>
	using scanner_int_optional = ScannerInteger<CharType>;

	template<ConceptCharType CharType>
	using scanner_string_literal = ScannerStringLiteral<CharType>;

	template<ConceptCharType CharType>
	using scanner_string = ScannerString<CharType>;

	template<ConceptCharType CharType>
	using scanner_string_alnum = ScannerStringAlnum<CharType>;

	template<ConceptCharType CharType>
	using scanner_symbol = ScannerSingleSymbol<CharType>;

	template<ConceptCharType CharType>
	using scanner_symbol_optional = ScannerSingleSymbol<CharType>;

	template<ConceptCharType CharType>
	using scanner_float = ScannerFloat<CharType>;

	template<ConceptCharType CharType>
	using scanner_float_optional = ScannerFloat<CharType>;

}