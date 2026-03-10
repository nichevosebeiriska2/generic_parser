#pragma once

#include "base_parser.h"


template<typename TParser, typename TParserDelimiter>
class ParserListNew
{
	TParser m_parser;
	TParserDelimiter m_parser_delimiter;
	const size_t number_of_repeats = CONST_NUMBER_OF_CHARS_AT_LEAST_ONE;

public:

	ParserListNew(const ParserListNew<TParser, TParserDelimiter>& other)
		: m_parser{ other.m_parser }
		, m_parser_delimiter{ other.m_parser_delimiter }
		, number_of_repeats{ other.number_of_repeats }
	{}

	ParserListNew(ParserListNew<TParser, TParserDelimiter>&& other ) noexcept
		: m_parser{ std::move(other.m_parser)}
		, m_parser_delimiter{ std::move(other.m_parser_delimiter)}
		, number_of_repeats{ other.number_of_repeats }
	{}

	ParserListNew(const ParserListNew<TParser, TParserDelimiter>& other, size_t N)
		: m_parser{ other.m_parser }
		, m_parser_delimiter{ other.m_parser_delimiter }
		, number_of_repeats{ N }
	{}

	ParserListNew(ParserListNew<TParser, TParserDelimiter>&& other, size_t N)
		: m_parser{ other.m_parser }
		, m_parser_delimiter{ other.m_parser_delimiter }
		, number_of_repeats{ N }
	{}

	ParserListNew(TParser&& parser, TParserDelimiter&& delimiter, size_t N = CONST_NUMBER_OF_CHARS_AT_LEAST_ONE)
		: m_parser{ std::forward<TParser>(parser) }
		, m_parser_delimiter{ std::forward<TParserDelimiter>(delimiter) }
		, number_of_repeats{ N }
	{}

	ParserListNew(const TParser& parser, const TParserDelimiter& delimiter, size_t N = CONST_NUMBER_OF_CHARS_AT_LEAST_ONE)
		: m_parser{ parser }
		, m_parser_delimiter{ delimiter }
		, number_of_repeats{ N }
	{}

	


	template<ConceptCharType CharType, ConceptContext TContext>
	constexpr static auto GetReturnType()
	{
		return std::vector<decltype(TParser::template GetReturnType<CharType, TContext>())>{};
	}

	template<ConceptCharType CharType, ConceptContext TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
		, constCharPtrRef<CharType> ptr_string_end
		, TContext&& context
		, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())>& attribute) const
	{
		bool delimiter_parser = true;
		context.UseSkipper(ptr_string, ptr_string_end);// pre-skip

		while (m_parser.ParseNew(ptr_string, ptr_string_end, context, attribute.emplace_back()) && delimiter_parser)// what about non default/copy constructible? looks like we have a problems is case TParser has parsing action
		{
			context.UseSkipper(ptr_string, ptr_string_end);

			auto delimiter_attr = decltype(TParserDelimiter::template GetReturnType<CharType, TContext>()){};
			delimiter_parser = m_parser_delimiter.ParseNew(ptr_string, ptr_string_end, context, delimiter_attr);
		}

		attribute.pop_back();

		return !attribute.empty();
	}

	auto operator[](auto&& callable)
	{
		return ParserWithActionNew(*this, callable);
	}
};

template<typename TParser, typename TDelimiter>
ParserListNew(TParser parser, TDelimiter delimiter) -> ParserListNew<TParser, TDelimiter>;

template<typename TParser, typename TDelimiter>
ParserListNew(TParser parser, TDelimiter delimiter, int N) -> ParserListNew<TParser, TDelimiter>;

//template<typename TParser, typename TDelimiter>
//ParserListNew(const ParserListNew<TParser, TDelimiter> &other) -> ParserListNew<TParser, TDelimiter>;

template<typename TParser, typename TDelimiter>
ParserListNew(const ParserListNew<TParser, TDelimiter>& other, int N) -> ParserListNew<TParser, TDelimiter>;

//template<typename TParser, typename TDelimiter>
//ParserListNew(ParserListNew<TParser, TDelimiter> &&other) -> ParserListNew<TParser, TDelimiter>;

template<typename TParser, typename TDelimiter>
ParserListNew(ParserListNew<TParser, TDelimiter>&& other, int N) -> ParserListNew<TParser, TDelimiter>;

template<ConceptNewParser TParser, ConceptNewParser TParserDelimiter>
auto operator %(TParser&& parser, TParserDelimiter&& parser_delimiter)
{
	return ParserListNew(parser, parser_delimiter);
}

template<ConceptNewParser TParser, ConceptCharType CharType>
auto operator %(TParser&& parser, const CharType* pStrLiteral)
{
	return ParserListNew(parser, ParserLiteralWithContext{ pStrLiteral });
}

template<ConceptNewParser TParser, ConceptNewParser TParserDelimiter>
auto operator+(ParserListNew<TParser, TParserDelimiter>& parser)
{
	return ParserListNew(parser, CONST_NUMBER_OF_CHARS_AT_LEAST_ONE);
}

template<ConceptNewParser TParser, ConceptNewParser TParserDelimiter>
auto operator+(ParserListNew<TParser, TParserDelimiter>&& parser)
{
	return ParserListNew(parser, CONST_NUMBER_OF_CHARS_AT_LEAST_ONE);
}


template<ConceptNewParser TParser, ConceptNewParser TParserDelimiter>
auto operator*(const ParserListNew<TParser, TParserDelimiter>& parser)
{
	return ParserListNew(parser, CONST_NUMBER_OF_CHARS_ZERO_OR_MORE);
}

template<ConceptNewParser TParser, ConceptNewParser TParserDelimiter>
auto operator*(ParserListNew<TParser, TParserDelimiter>&& parser)
{
	return ParserListNew(parser, CONST_NUMBER_OF_CHARS_ZERO_OR_MORE);
}