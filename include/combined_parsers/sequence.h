#pragma once/**/

#include "utils_tuple.h"
#include "base_parser.h"
#include "parser_with_action.h"

template<typename ... TParsers>
class ParserSeqNew
{
public:
	std::tuple<TParsers...> m_tuple_parsers;


	template<ConceptCharType CharType, ConceptContext TContext, size_t ... Ts>
	bool ParseImpl(constCharPtrRef<CharType> ptr_string
		, constCharPtrRef<CharType> ptr_string_end
		, TContext&& context
		, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())>& attribute
		, std::index_sequence<Ts...> seq) const
	{
		return (std::get<Ts>(m_tuple_parsers).ParseNew(ptr_string, ptr_string_end, context, std::get<Ts>(attribute)) && ...);
	}

	template<ConceptCharType CharType, ConceptContext TContext, size_t...Ts>
	consteval static auto GetReturnTypeImpl(std::index_sequence<Ts...> seq)
	{
		return std::make_tuple(std::tuple_element_t<Ts, decltype(m_tuple_parsers)>::template GetReturnType<CharType, TContext>() ...);
	}

public:

	ParserSeqNew(TParsers&& ... parsers)
		: m_tuple_parsers{ parsers ...}
	{
	}

	ParserSeqNew(const TParsers& ... parsers)
		: m_tuple_parsers{ parsers... }
	{
	}

	ParserSeqNew(const ParserSeqNew<TParsers...>& other)
		: m_tuple_parsers{ other.m_tuple_parsers }
	{
	}

	ParserSeqNew(ParserSeqNew<TParsers...>&& other) noexcept
		: m_tuple_parsers{ std::move(other.m_tuple_parsers) }
	{
	}

	template<typename... T, typename TRight>
	ParserSeqNew(const ParserSeqNew<T...>& seq, TRight&& right)
		: m_tuple_parsers{ std::tuple_cat(tuple_utils::as_tuple(seq.m_tuple_parsers), tuple_utils::as_tuple(right)) }
	{
	}

	template<typename TLeft, typename... T>
	ParserSeqNew(TLeft&& left, const ParserSeqNew<T...>& seq)
		: m_tuple_parsers{ std::tuple_cat(tuple_utils::as_tuple(left), tuple_utils::as_tuple(seq.m_tuple_parsers)) }
	{
	}

	template<typename... TLeft, typename... TRight>
	ParserSeqNew(const ParserSeqNew<TLeft...>& seq_left, const ParserSeqNew<TRight...>& seq_right)
		: m_tuple_parsers{ std::tuple_cat(tuple_utils::as_tuple(seq_left.m_tuple_parsers), tuple_utils::as_tuple(seq_right.m_tuple_parsers)) }
	{
	}

	template<ConceptCharType CharType, ConceptContext TContext>
	constexpr static auto GetReturnType()
	{
		return GetReturnTypeImpl<CharType, TContext>(std::make_index_sequence<std::tuple_size_v<decltype(m_tuple_parsers)>>{});
	}

	template<ConceptCharType CharType, ConceptContext TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
		, constCharPtrRef<CharType> ptr_string_end
		, TContext&& context
		, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())>& attribute) const
	{
		return ParseImpl(ptr_string, ptr_string_end, context, attribute, std::make_index_sequence<sizeof...(TParsers)>{});
	}

	auto operator[](auto callable)
	{
		return ParserWithActionNew(*this, callable);
	}
};


template<typename TLeft, typename TRight>
ParserSeqNew(const TLeft& left, const TRight& right) -> ParserSeqNew<TLeft, TRight>;

template<typename... SeqTypes, typename TRight>
ParserSeqNew(ParserSeqNew<SeqTypes...> seq, TRight right) -> ParserSeqNew<SeqTypes..., TRight>;

template<typename TLeft, typename... SeqTypes>
ParserSeqNew(TLeft left, ParserSeqNew<SeqTypes...> seq) -> ParserSeqNew<TLeft, SeqTypes...>;

template<typename... SeqTypesLeft, typename... SeqTypesRight>
ParserSeqNew(ParserSeqNew<SeqTypesLeft...> seq_left, ParserSeqNew<SeqTypesRight...> seq_right) -> ParserSeqNew<SeqTypesLeft..., SeqTypesRight...>;


template<ConceptNewParser TLeft, ConceptNewParser TRight>
auto operator>>(const TLeft &left, const TRight &right)
{
	return ParserSeqNew(left, right);
}

template<ConceptNewParser TLeft, ConceptNewParser TRight>
auto operator>>(TLeft&& left, TRight&& right)
{
	return ParserSeqNew(std::forward<TLeft>(left), std::forward<TRight>(right));
}

template<ConceptCharType CharType, ConceptNewParser TRight>
auto operator>>(CharType symbol, TRight&& right)
{
	return ParserLiteralWithContext{symbol} >> std::forward<TRight>(right);
}

template<ConceptNewParser TLeft, ConceptCharType CharType>
auto operator>>(TLeft&& left, CharType symbol)
{
	return  std::forward<TLeft>(left) >> ParserLiteralWithContext(symbol);
}

template<ConceptNewParser TLeft, ConceptCharType CharType>
auto operator>>(TLeft&& left, const CharType* literal)
{
	return std::forward<TLeft>(left) >> ParserLiteralWithContext(literal);
}

template<ConceptNewParser TLeft, ConceptCharType CharType>
auto operator>>(const TLeft &left, const CharType *literal)
{
	return left >> ParserLiteralWithContext(literal);
}

template<ConceptCharType CharType, ConceptNewParser TRight>
auto operator>>(const CharType* literal, TRight&& right)
{
	return ParserLiteralWithContext(literal) >> std::forward<TRight>(right);
}

template<ConceptCharType CharType, ConceptNewParser TRight>
auto operator>>(const CharType* literal, TRight& right)
{
	return ParserLiteralWithContext(literal) >> std::forward<TRight>(right);
}