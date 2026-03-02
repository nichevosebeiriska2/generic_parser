#pragma once

#include <variant>
#include <type_traits>

#include "common.h"
#include "ParserBase.h"

namespace Parsers {


template<ConceptParser TParser>
class ParserRaw
{
	template<ConceptCharType CharType>
	using PairStringPointers = std::pair< constCharPtr<CharType>, constCharPtr<CharType>>;

	std::variant<PairStringPointers<char>, PairStringPointers<wchar_t>> m_result;
	TParser m_parser;
public:
	using parsing_attribute = std::string;

public:
	ParserRaw(TParser&& p)
		: m_parser{ p }
	{
	}

	ParserRaw(const TParser& p)
		: m_parser{ p }
	{
	}


	template<ConceptCharType CharType, ConceptParser SkipperType>
	auto Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
	{
		return Scan(ptr_string, ptr_string_end, skipper);
	}

	template<ConceptCharType CharType>
	auto Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
	{
		return Scan(ptr_string, ptr_string_end);
	}

	template<ConceptCharType CharType, ConceptParser SkipperType>
	auto Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
	{
		constCharPtr<CharType>ptr_begin = ptr_string;

		const bool scanned = m_parser.Scan(ptr_string, ptr_string_end, skipper);

		if (scanned)
			m_result = std::make_pair(ptr_begin, ptr_string);

		return scanned;
	}

	template<ConceptCharType CharType>
	auto Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
	{
		constCharPtr<CharType>ptr_begin = ptr_string;

		const bool scanned = m_parser.Scan(ptr_string, ptr_string_end);

		if (scanned)
			m_result = std::make_pair(ptr_begin, ptr_string);

		return scanned;
	}

	constexpr auto GetValueAndReset()
	{
		return m_result;
	}
	void Reset()
	{
		m_parser.Reset();
		m_result = {};
	}

	static constexpr bool IsOmited()
	{
		return false;
	}

	auto operator [](auto action)
	{
		return ParserWrapperWithAction(*this, action);
	}
};

template<typename TLeft>
ParserRaw(TLeft&& left) -> ParserRaw<TLeft>;

template<typename TLeft>
ParserRaw(const TLeft& left) -> ParserRaw<TLeft>;

template<ConceptParser TParser>
using raw = ParserRaw<TParser>;

};