#pragma once

#include "parser_aliases.h"

namespace Parsers
{
	// uses internal parser as a scan-only parser with no return attribute
	template<ConceptParser TParser>
	class omited
	{
		static_assert(!is_basic_non_literal_parser_v<TParser>, "template type parser is literal parser so it is omited by default!");

	public:
		using internal_char_type = char;
		using parsing_attribute = tag_attribute_unused;

	protected:
		TParser m_parser;

	public:
		constexpr omited(TParser &&parser) noexcept
			: m_parser(std::forward<TParser>(parser))
		{
		}

		template<ConceptCharType CharType>
		constexpr omited(CharType symbol) noexcept
			: m_parser(symbol)
		{}

		template<ConceptCharType CharType>
		constexpr omited(const CharType* symbol) noexcept
			: m_parser(symbol)
		{
		}

		constexpr omited(const TParser &parser) noexcept
			: m_parser(parser)
		{
		}

		constexpr omited(const omited<TParser> &&other) noexcept
			: m_parser(std::move(other.m_parser))
		{
		}

		constexpr omited(const omited<TParser> &other) noexcept
			: m_parser(other.m_parser)
		{
		}

		template<ConceptCharType CharType, ConceptParser SkipperParserType>
		auto Parse(const CharType*& ptr_string, const CharType*& ptr_string_end, SkipperParserType&& skipper) 
		{
			// its actually a scan function of internal parser. We`ll call it 'Parse()' for consistency of classes usage contract
			return m_parser.Scan(ptr_string, ptr_string_end, skipper);
		}

		template<ConceptCharType CharType>
		auto Parse(const CharType*& ptr_string, const CharType*& ptr_string_end)
		{
			return m_parser.Scan(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType, ConceptParser SkipperParserType>
		auto Scan(const CharType*& ptr_string, const CharType*& ptr_string_end, SkipperParserType&& skipper)
		{
			// its actually a scan function of internal parser. We`ll call it 'Parse()' for consistency of classes usage contract
			return m_parser.Scan(ptr_string, ptr_string_end, skipper);
		}

		template<ConceptCharType CharType>
		auto Scan(const CharType*& ptr_string, const CharType*& ptr_string_end)
		{
			return m_parser.Scan(ptr_string, ptr_string_end);
		}

		bool constexpr IsOmited()
		{
			return true;
		}

		auto operator ()(auto action)
		{
			return ParserWrapperWithAction(*this, action);
		}
	};

	// metafunctions 
	template<typename T>
	struct is_omited : std::false_type {};

	template<typename T>
	struct is_omited <omited<T>> : std::true_type {};

	template<typename T>
	inline constexpr bool is_omited_v = is_omited<T>::value;
	// \metafunctions 

	// deduction guides

	template<typename TParser>
	omited(omited<TParser>) -> omited<TParser>;

	template<typename TParser>
	omited(const omited<TParser>&) -> omited<TParser>;

	template<typename TParser>
	omited(omited<TParser>&&) -> omited<TParser>;

	// \deduction guides
}