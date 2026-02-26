#pragma once

#include "ParserOmited.h"
#include "ParserSequence.h"
#include "ParserAlternative.h"
#include "ParserList.h"
#include "ParserRepeate.h"

namespace Parsers
{
	// sequence
	template<ConceptParser TLeft, ConceptParser TRight>
	auto operator>>(TLeft &&left, TRight &&right)
	{
		return sequential(std::forward<TLeft>(left), std::forward<TRight>(right));
	}

	template<ConceptCharType CharType, ConceptParser TRight>
	auto operator>>(CharType symbol, TRight &&right)
	{
		return _char_lit<CharType>(symbol) >> std::forward<TRight>(right);
	}

	template<ConceptParser TLeft, ConceptCharType CharType>
	auto operator>>(TLeft &&left, CharType symbol)
	{
		return  std::forward<TLeft>(left) >> _char_lit<CharType>(symbol);
	}

	template<ConceptParser TLeft, ConceptCharType CharType>
	auto operator>>(TLeft &&left, const CharType *literal)
	{
		return std::forward<TLeft>(left) >> _string_lit<CharType>(literal);
	}

	template<ConceptCharType CharType, ConceptParser TRight>
	auto operator>>(const CharType *literal, TRight &&right)
	{
		return _string_lit<CharType>(literal) >> std::forward<TRight>(right);
	}

	// list
	template<ConceptParser TParser, ConceptParser TParserDelimiter>
	auto operator %(TParser&& parser, TParserDelimiter&& parser_delimiter)
	{
		return list(parser, parser_delimiter);
	}

	template<ConceptParser TParser, ConceptCharType CharType>
	auto operator %(TParser &&parser, const CharType* pStrLiteral)
	{
		return list(parser, _string_lit{pStrLiteral});
	}

	// alternative
	template<ConceptParser TLeft, ConceptParser TRight>
	constexpr auto operator|(TLeft&& left, TRight&& right)
	{
		return alternative(left, right);
	}

	template<ConceptParser TLeft, ConceptCharType CharType>
	constexpr auto operator|(TLeft &&left, const CharType* pStrLiteral)
	{
		return alternative(left, _string_lit{pStrLiteral});
	}

	template<ConceptCharType CharType>
	constexpr auto operator|(_string_lit<CharType> lit, const CharType *pStrLiteralright)
	{
		return alternative(lit, _string_lit{pStrLiteralright});
	}

	//repeate
	template<ConceptParser TParser>
	auto operator+(TParser parser)
	{
		return repeate(parser, CONST_NUMBER_OF_CHARS_AT_LEAST_ONE);
	}

	template<ConceptParser TParser>
	auto operator*(TParser&& parser)
	{
		return repeate(parser, CONST_NUMBER_OF_CHARS_ZERO_OR_MORE);
	}

	template<ConceptParser TParser, ConceptParser TParserDelimiter>
	auto operator+(list<TParser, TParserDelimiter> parser)
	{
		return list(parser, CONST_NUMBER_OF_CHARS_AT_LEAST_ONE);
	}

	template<ConceptParser TParser, ConceptParser TParserDelimiter>
	auto operator*(list<TParser, TParserDelimiter> parser)
	{
		return list(parser, CONST_NUMBER_OF_CHARS_ZERO_OR_MORE);
	}

	template<ConceptParser TParser>
	auto operator*(UINT count, TParser&& parser)
	{
		return repeate(parser, count);
	}

	// omited

	template<typename TParser>
	auto omit(TParser && parser)
	{
		return omited(std::forward<TParser>(parser));
	}

	namespace Skippers
	{
		inline auto space = Parsers::_string_lit{" "} | "\t" | "\n" ;
	}
}
