#pragma once

#include "base_parser.h"

// predicate parsed for next symbol/token check. Does not mofify input string pointer!
template<typename TParser>
class ParserPredicateNon
{
	TParser m_parser;
public:

	ParserPredicateNon(const TParser &parser)
		: m_parser{parser}
	{}

	ParserPredicateNon(TParser&& parser)
		: m_parser{parser}
	{}

	template<ConceptCharType CharType, ConceptContext TContext>
	constexpr static auto GetReturnType()
	{
		return tag_attribute_unused{};
	}

	template<ConceptCharType CharType, ConceptContext TContext>
	bool ParseNew(constCharPtr<CharType> ptr_string
								, constCharPtr<CharType> ptr_string_end
								, TContext &&context
								, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &attribute) const
	{
		context.UseSkipper(ptr_string, ptr_string_end);// pre-skip
		auto attr = traits::attribute_t<TParser, CharType, TContext>{};

		return !m_parser.ParseNew(ptr_string, ptr_string_end, context, attr);
	}

	auto operator[](auto &&callable)
	{
		return ParserWithActionNew(*this, callable);
	}
};

template<ConceptNewParser TParser>
ParserPredicateNon(TParser &&parser) -> ParserPredicateNon<TParser>;

template<ConceptNewParser TParser>
constexpr auto operator!(TParser&& parser)
{
	return ParserPredicateNon{std::forward<TParser>(parser)};
}