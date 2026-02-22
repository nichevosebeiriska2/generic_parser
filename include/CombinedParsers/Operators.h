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

	// alternative
	template<ConceptParser TLeft, ConceptParser TRight>
	auto operator|(TLeft&& left, TRight&& right)
	{
		return alternative(left, right);
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

	template<ConceptParser TParser>
	auto operator*(UINT count, TParser&& parser)
	{
		return repeate(parser, count);
	}

	template<typename TParser>
	auto omit(TParser && parser)
	{
		return omited(std::forward<TParser>(parser));
	}
}
