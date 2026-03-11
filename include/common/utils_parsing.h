#pragma once 

#include "base_parser.h"

template<ConceptCharType CharType, typename TParser>
auto ParseLexeme2(std::basic_string<CharType> strInput, TParser &parser)
{

	const CharType *ptr_begin = strInput.data();
	const CharType *ptr_end = ptr_begin + strInput.length();

	if constexpr(std::is_same_v<CharType, char>)
	{
		Context ctx{Skipper{" \n\r\t"}};
		auto result = TParser::template GetReturnType<CharType, decltype(ctx)>();
		bool parsed = parser.ParseNew(ptr_begin, ptr_end, ctx, result);
		return std::make_pair(parsed, std::move(result));
	}
	else
	{
		Context ctx{Skipper{L" \n\t"}};
		auto result = TParser::template GetReturnType<CharType, decltype(ctx)>();
		bool parsed = parser.ParseNew(ptr_begin, ptr_end, ctx, result);
		return std::make_pair(parsed, std::move(result));
	}
}

template<ConceptCharType CharType, typename TParser>
auto ParseLexemeNoSkip(std::basic_string<CharType> strInput, TParser &parser)
{

	const CharType *ptr_begin = strInput.data();
	const CharType *ptr_end = ptr_begin + strInput.length();

	if constexpr(std::is_same_v<CharType, char>)
	{
		Context ctx;
		auto result = TParser::template GetReturnType<CharType, decltype(ctx)>();
		bool parsed = parser.ParseNew(ptr_begin, ptr_end, ctx, result);
		return std::make_pair(parsed, std::move(result));
	}
	else
	{
		Context ctx;
		auto result = TParser::template GetReturnType<CharType, decltype(ctx)>();
		bool parsed = parser.ParseNew(ptr_begin, ptr_end, ctx, result);
		return std::make_pair(parsed, std::move(result));
	}
}

template<ConceptCharType CharType, typename TParser>
auto ParseLexeme2(const CharType *strInput, TParser &&parser)
{
	return ParseLexeme2(std::basic_string<CharType>{strInput}, parser);
}

template<ConceptCharType CharType, typename TParser>
auto ParseLexemeNoSkip(const CharType *strInput, TParser &&parser)
{
	return ParseLexemeNoSkip(std::basic_string<CharType>{strInput}, parser);
}