#pragma once

#include <vector>

#include "ParserBase.h"

// parsing of repetitive patterns

namespace Parsers
{

template<typename TParser>
class repeate
{
public:
	using parsing_attribute = std::vector<typename TParser::parsing_attribute>;

protected:
	TParser m_parser;
	parsing_attribute m_vec_result;

	int number_of_repeats = CONST_NUMBER_OF_CHARS_AT_LEAST_ONE;

public:

	constexpr repeate(TParser&& parser, INT N = CONST_NUMBER_OF_CHARS_AT_LEAST_ONE) noexcept
		: m_parser{std::forward<decltype(parser)>(parser)}
		, number_of_repeats{N}
	{
	}

	constexpr repeate(const TParser &parser, INT N = CONST_NUMBER_OF_CHARS_AT_LEAST_ONE) noexcept
		: m_parser{parser}
		, number_of_repeats{ N }
	{
	}

	constexpr repeate(const repeate& other) noexcept
		: m_parser{ other.m_parser }
		, number_of_repeats{ other.number_of_repeats }
	{
	}

	constexpr repeate(repeate&& other) noexcept
		: m_parser{ std::move(other.m_parser)}
		, number_of_repeats{ other.number_of_repeats }
	{
	}


	template<ConceptCharType CharType, typename TParserSkipper>
	bool Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TParserSkipper& skipper)
	{
		UseSkipper(ptr_string, ptr_string_end, skipper);
		UINT count = 0;
		while (ptr_string != ptr_string_end && m_parser.Parse(ptr_string, ptr_string_end, skipper))
		{
			m_vec_result.emplace_back(m_parser.GetValueAndReset());
			UseSkipper(ptr_string, ptr_string_end, skipper);
			count++;
		}

		if(number_of_repeats == CONST_NUMBER_OF_CHARS_AT_LEAST_ONE)
			return !m_vec_result.empty();
		else if(number_of_repeats == CONST_NUMBER_OF_CHARS_ZERO_OR_MORE)
			return true;
		else
			return number_of_repeats == count;
	}

	template<ConceptCharType CharType>
	bool Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
	{
		UINT count = 0;
		while (m_parser.Parse(ptr_string, ptr_string_end))
		{
			count++;
			m_vec_result.emplace_back(m_parser.GetValueAndReset());
		}

		if (number_of_repeats == CONST_NUMBER_OF_CHARS_AT_LEAST_ONE)
			return !m_vec_result.empty();
		else if (number_of_repeats == CONST_NUMBER_OF_CHARS_ZERO_OR_MORE)
			return true;
		else
			return number_of_repeats == count;
	}

	template<ConceptCharType CharType, typename ParserType>
	bool Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, ParserType& skipper)
	{
		UseSkipper(ptr_string, ptr_string_end, skipper);
		UINT count = 0;
		while (m_parser.Scan(ptr_string, ptr_string_end, skipper))
		{
			UseSkipper(ptr_string, ptr_string_end, skipper);
			count++;
		}

		if (number_of_repeats == CONST_NUMBER_OF_CHARS_AT_LEAST_ONE)
			return !m_vec_result.empty();
		else if (number_of_repeats == CONST_NUMBER_OF_CHARS_ZERO_OR_MORE)
			return true;
		else
			return number_of_repeats == count;
	}

	template<ConceptCharType CharType>
	bool Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
	{
		UINT count = 0;
		while (m_parser.Scan(ptr_string, ptr_string_end))
			count++;

		if (number_of_repeats == CONST_NUMBER_OF_CHARS_AT_LEAST_ONE)
			return !m_vec_result.empty();
		else if (number_of_repeats == CONST_NUMBER_OF_CHARS_ZERO_OR_MORE)
			return true;
		else
			return number_of_repeats == count;
	}

	static constexpr bool IsOmited()
	{
		return decltype(m_parser)::IsOmited();
	}

	auto GetValueAndReset()
	{
		m_parser.Reset();
		return std::exchange(m_vec_result, {});
	}
	void Reset()
	{
		m_parser.Reset();
		m_vec_result = {};
	}

	auto operator [](auto action)
	{
		return ParserWrapperWithAction(*this, action);
	}

};

}