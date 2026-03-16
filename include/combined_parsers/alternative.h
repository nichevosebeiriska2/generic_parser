#pragma once

#include <variant>
#include "utils_tuple.h"
#include "base_parser.h"


template<typename ... TParsers>
class ParserAltNew
{
public:
	std::tuple<TParsers...> m_tuple_parsers;

protected:

	template<ConceptCharType CharType, ConceptContext TContext, size_t ... Ts>
	bool ParseImpl(constCharPtrRef<CharType> ptr_string
		, constCharPtrRef<CharType> ptr_string_end
		, TContext&& context
		, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())>& attribute
		, std::index_sequence<Ts...> seq) const
	{
		auto lambda_parse_func = [&]<size_t index>()
		{
			using tParserType = traits::attribute_t<decltype(std::get<index>(m_tuple_parsers)), CharType, TContext>;
			auto _val = tParserType{};
			const auto ptr_temp = ptr_string;

			auto parsed = std::get<index>(m_tuple_parsers).ParseNew(ptr_string, ptr_string_end, context, _val);

			if(parsed)
				attribute.emplace<index>(_val);
			else
				ptr_string = ptr_temp;

			return parsed;
		};

		return (lambda_parse_func.operator()<Ts>() || ... );
	}

	template<ConceptCharType CharType, ConceptContext TContext, size_t...Ts>
	constexpr static auto GetReturnTypeImpl(std::index_sequence<Ts...> seq)
	{
		return std::variant<decltype(std::tuple_element_t<Ts, decltype(m_tuple_parsers)>::template GetReturnType<CharType, TContext>()) ... >{};
	}

public:

	constexpr ParserAltNew(ParserAltNew<TParsers...> && other)
		: m_tuple_parsers{ std::move(other.m_tuple_parsers)}
	{
	}

	constexpr ParserAltNew(const ParserAltNew<TParsers...>& other)
		: m_tuple_parsers{ other.m_tuple_parsers }
	{
	}

	template<typename ... TOtherParsert, typename TRight>
	constexpr ParserAltNew(ParserAltNew<TOtherParsert...>&& other, TRight&& right)
		: m_tuple_parsers { tuple_utils::concat_as_tuple(other.m_tuple_parsers, right) }
	{

	}

	template<typename ... TOtherParsert, typename TRight>
	constexpr ParserAltNew(const ParserAltNew<TOtherParsert...>& other, TRight&& right)
		: m_tuple_parsers{ tuple_utils::concat_as_tuple(other.m_tuple_parsers, right) }
	{

	}

	template<typename TLeft, typename ... TOtherParsert>
	constexpr ParserAltNew(TLeft&& left, ParserAltNew<TOtherParsert...>&& other)
		: m_tuple_parsers{ tuple_utils::concat_as_tuple(left, other.m_tuple_parsers) }
	{

	}

	template<typename TLeft, typename ... TOtherParsert>
	constexpr ParserAltNew(TLeft& left, const ParserAltNew<TOtherParsert...>& other)
		: m_tuple_parsers{ tuple_utils::concat_as_tuple(left, other.m_tuple_parsers) }
	{

	}

	constexpr ParserAltNew(TParsers ... parsers)
		: m_tuple_parsers{ std::forward<TParsers>(parsers) ...}
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

	auto operator[](auto&& callable) const
	{
		return ParserWithActionNew(*this, callable);
	}
};

template<typename TLeft, typename TRight>
ParserAltNew(TLeft&& left, TRight&& right) -> ParserAltNew<std::remove_cvref_t<TLeft>, std::remove_cvref_t<TRight>>;

template<typename... AltTypes>
ParserAltNew(ParserAltNew< AltTypes...>&& alt) -> ParserAltNew<AltTypes...>;

template<typename... AltTypes>
ParserAltNew(ParserAltNew< AltTypes...>& alt) -> ParserAltNew<AltTypes...>;

template<typename... AltTypes>
ParserAltNew(const ParserAltNew< AltTypes...>& alt) -> ParserAltNew<AltTypes...>;

template<typename... AltTypes, typename TRight>
ParserAltNew(ParserAltNew<AltTypes...>&& seq, TRight&& right) -> ParserAltNew<AltTypes..., std::remove_cvref_t<TRight>>;

template<typename... AltTypes, typename TRight>
ParserAltNew(ParserAltNew<AltTypes...>& seq, TRight&& right) -> ParserAltNew<AltTypes..., std::remove_cvref_t<TRight>>;

template<typename TLeft, typename... AltTypes>
ParserAltNew(TLeft&& left, ParserAltNew<AltTypes...>&& seq) -> ParserAltNew< std::remove_cvref_t<TLeft>, AltTypes...>;

template<typename TLeft, typename... AltTypes>
ParserAltNew(TLeft&& left, ParserAltNew<AltTypes...>& seq) -> ParserAltNew< std::remove_cvref_t<TLeft>, AltTypes...>;

template<typename... AltTypesLeft, typename... AltTypesRight>
ParserAltNew(ParserAltNew<AltTypesLeft...>&& altLeft, ParserAltNew<AltTypesRight...>&& altRight) -> ParserAltNew<AltTypesLeft..., AltTypesRight...>;

template<typename... AltTypesLeft, typename... AltTypesRight>
ParserAltNew(ParserAltNew<AltTypesLeft...>& altLeft, ParserAltNew<AltTypesRight...>&& altRight) -> ParserAltNew<AltTypesLeft..., AltTypesRight...>;

template<typename... AltTypesLeft, typename... AltTypesRight>
ParserAltNew(ParserAltNew<AltTypesLeft...>&& altLeft, ParserAltNew<AltTypesRight...>& altRight) -> ParserAltNew<AltTypesLeft..., AltTypesRight...>;

template<typename... AltTypesLeft, typename... AltTypesRight>
ParserAltNew(ParserAltNew<AltTypesLeft...>& altLeft, ParserAltNew<AltTypesRight...>& altRight) -> ParserAltNew<AltTypesLeft..., AltTypesRight...>;

template<ConceptNewParser TLeft, ConceptNewParser TRight>
constexpr auto operator|(TLeft&& left, TRight&& right)
{
	return ParserAltNew(left, right);
}

template<ConceptCharType CharType, ConceptNewParser TRight>
constexpr auto operator|(const CharType* literal, TRight &&right)
{
	return ParserAltNew(ParserLiteralWithContext{literal}, right);
}

template<ConceptNewParser TLeft, ConceptCharType CharType>
constexpr auto operator|(TLeft &&left, const CharType *literal)
{
	return ParserAltNew(left, ParserLiteralWithContext{literal});
}

template<ConceptCharType CharType, ConceptNewParser TRight>
constexpr auto operator|(CharType symbol, TRight &&right)
{
	return ParserAltNew(ParserLiteralWithContext{symbol}, right);
}

template<ConceptNewParser TLeft, ConceptCharType CharType>
constexpr auto operator|(TLeft &&left, CharType symbol)
{
	return ParserAltNew(left, ParserLiteralWithContext{symbol});
}


namespace aliases
{
	constexpr auto space = space_ | newline_ | tab_;
}