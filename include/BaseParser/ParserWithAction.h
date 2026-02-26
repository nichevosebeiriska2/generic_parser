#pragma once

#include "common.h"

namespace Parsers{

template<ConceptParser ParserType, typename ActionType>
class ParserWrapperWithAction
{
	consteval static auto DeduceNewParsingAttribute()
	{

		if constexpr (ParserType::IsOmited())
		{
			static_assert(std::is_invocable_v<ActionType>
				, "ParserWrapperWithAction::DeduceNewParsingAttribute() : action cant be invoked with arguments");

			return std::invoke_result_t < ActionType>{};
		}
		else
		{
			static_assert(std::is_invocable_v<ActionType, decltype(std::declval<ParserType>().GetValueAndReset())>
				, "ParserWrapperWithAction::DeduceNewParsingAttribute() : action cant be invoked with ParserType return type");
			return std::invoke_result_t<ActionType, decltype(std::declval<ParserType>().GetValueAndReset())>{};
		}

	}

protected:
	ParserType m_parser;
	ActionType m_action;

public:
	using parsing_attribute = decltype(DeduceNewParsingAttribute());

public:
	ParserWrapperWithAction(auto&& parser, auto&& action)
		: m_parser{ parser }
		, m_action{ action }
	{}

	template<ConceptCharType CharType, typename TParserSkipper>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper)
	{
		UseSkipper(ptr_string, ptr_string_end, skipper);
		return m_parser.Parse(ptr_string, ptr_string_end, skipper);
	}

	template<ConceptCharType CharType>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end)
	{
		return m_parser.Parse(ptr_string, ptr_string_end);
	}

	template<ConceptCharType CharType, typename TParserSkipper>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper)
	{
		UseSkipper(ptr_string, ptr_string_end, skipper);
		return m_parser.Scan(ptr_string, ptr_string_end);
	}

	template<ConceptCharType CharType>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end)
	{
		return m_parser.Scan(ptr_string, ptr_string_end);
	}

	constexpr auto GetValueAndReset()
	{
		if constexpr (ParserType::IsOmited())
			return m_action();
		else
			return m_action(m_parser.GetValueAndReset());

	}

	void Reset()
	{
		m_parser.Reset();
	}

	constexpr static bool IsOmited()
	{
		return false;
	}

	auto Copy()
	{
		return *this;
	}
};

template<typename P, typename A>
ParserWrapperWithAction(P&&, A&&) -> ParserWrapperWithAction<std::remove_cvref_t<P>, std::remove_cvref_t<A>>;


template<typename P, typename A>
using parser_with_action = ParserWrapperWithAction<P, A>;

};