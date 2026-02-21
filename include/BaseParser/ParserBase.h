#pragma once

#include <string>
#include "scanner_aliases.h"

namespace Parsers
{

/*
	Each parser should have Parse() and Scan() functions for both common and no-result parsing (in case parser tagged as omitted)
*/

template<ConceptCharType CharType, typename TParserSkipper>
void UseSkipper(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper)
{
	while(skipper.Scan(ptr_string, ptr_string_end))
	{
	}
}

template<template<typename> typename  TScanner, typename TResult>
class Parser
{
public:
	template<ConceptCharType CharType>
	using scanner_type = TScanner<CharType>;
	using internal_char_type = TScanner<char>::internal_char_type;
	using parsing_attribute = TResult;

protected:
	parsing_attribute m_result;

protected:
		auto ParseFromPointers(const auto *ptr_string, const auto *ptr_string_end)
		{
			if constexpr((std::is_integral_v<parsing_attribute> || std::is_floating_point_v<parsing_attribute>) && !ConceptCharType<parsing_attribute>)
			{
				parsing_attribute result = 0;
				const auto [ptr_end, error_code] = std::from_chars(ptr_string, ptr_string_end, result);
				return std::make_pair(ptr_end == ptr_string_end && ptr_end != ptr_string, result);
			}
			else if constexpr(ConceptCharType<parsing_attribute>) // parsing attibute is char/wchar_t
			{
				return std::make_pair(true, *ptr_string);
			}
			else // string
				return std::make_pair(true, std::basic_string(ptr_string, ptr_string_end));

		}

	public:
		constexpr Parser() noexcept {}; /* '= default' makes Parser aggregate type -> Parser always needs aggregate initialization for named variables ( Parser parser{} )  */
		constexpr Parser(const Parser &other) noexcept = default;
		constexpr Parser(Parser &&other) noexcept = default;

		template<ConceptCharType CharType, typename TParserSkipper>
		bool Parse(const CharType *&ptr_string, const CharType *&ptr_string_end, TParserSkipper &skipper)
		{
			UseSkipper(ptr_string, ptr_string_end, skipper);

			return Parse(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType>
		bool Parse(const CharType *&ptr_string, const CharType *&ptr_string_end)
		{
			auto scanner = TScanner<CharType>{};
			
			if(!scanner.Scan(ptr_string, ptr_string_end))
				return false;

			auto [parsed, result] = ParseFromPointers(ptr_string, ptr_string + scanner.GetNumberOfScannedChars());
			ptr_string += scanner.GetNumberOfScannedChars();
			m_result = result;

			return parsed;
		}

		template<ConceptCharType CharType, typename TParserSkipper>
		bool Scan(const CharType *&ptr_string, const CharType *&ptr_string_end, TParserSkipper &skipper)
		{
			UseSkipper(ptr_string, ptr_string_end, skipper);

			return Scan(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType>
		bool Scan(const CharType *&ptr_string, const CharType *&ptr_string_end)
		{
			auto scanner = TScanner<CharType>{};
			const bool scanned = scanner.Scan(ptr_string, ptr_string_end);

			if(scanned)
				ptr_string += scanner.GetNumberOfScannedChars();

			return scanned;
		}

		constexpr auto GetValueAndReset()
		{
			return std::exchange(m_result, {});
		}

		void Reset()
		{
			m_result = {};
		}
	};


	// used specialy for string/single symbol literals. omited by default!
	template<ConceptCharType CharType>
	class ParserLiteral
	{
	public:
		using parsing_attribute = tag_attribute_unused;
		using scanner_type = Scanners::ScannerStringLiteral<CharType>;
		using internal_char_type = scanner_type::internal_char_type;

		using const_pointer = const internal_char_type *;
		using const_pointer_ref = const internal_char_type *&;

	protected:
		scanner_type m_scanner;

	public:
		constexpr ParserLiteral() = default;

		constexpr ParserLiteral(const ParserLiteral &other) noexcept
			: m_scanner{other.m_scanner}
		{
		}

		constexpr ParserLiteral(ParserLiteral &&other) noexcept
			: m_scanner{other.m_scanner}
		{
		}

		constexpr ParserLiteral(std::basic_string_view<CharType> view) noexcept
			: m_scanner{view}
		{
		};

		template<ConceptCharType CharType, typename TParserSkipper>
		bool Parse(const CharType *&ptr_string, const CharType *&ptr_string_end, TParserSkipper &skipper)
		{
			UseSkipper(ptr_string, ptr_string_end, skipper);
			return Scan(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType>
		bool Parse(const CharType *&ptr_string, const CharType *&ptr_string_end)
		{
			return Scan(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType, typename TParserSkipper>
		bool Scan(const CharType *&ptr_string, const CharType *&ptr_string_end, TParserSkipper &skipper)
		{
			UseSkipper(ptr_string, ptr_string_end, skipper);
			return scan(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType>
		bool Scan(const CharType *&ptr_string, const CharType *&ptr_string_end)
		{
			static_assert(std::is_same_v<CharType, internal_char_type>, "ParserLiteral::Scan(const CharType *&ptr_string, const auto *&ptr_string_end, const auto &skipper) - char type should be the same as internal_char_type");
			bool scanned = m_scanner.Scan(ptr_string, ptr_string_end);
			if (scanned)
				ptr_string += m_scanner.GetNumberOfScannedChars();

			return scanned;
		}
	};
}


// metafunctions and concepts


template<typename T>
struct is_basic_non_literal_parser : public std::false_type {};

template<template<typename> typename  TScanner, typename TResult>
struct is_basic_non_literal_parser<Parsers::Parser<TScanner, TResult>> : std::true_type {};

template<typename T>
constexpr bool is_basic_non_literal_parser_v = is_basic_non_literal_parser<T>::value;

template<typename T>
struct is_basic_literal_parser : public std::false_type {};

template<ConceptCharType CharType>
struct is_basic_literal_parser<Parsers::ParserLiteral<CharType>> : std::true_type {};

template<typename T>
constexpr bool is_basic_literal_parser_v = is_basic_literal_parser<T>::value;

template<typename T>
concept ConcepBaseParser = is_basic_literal_parser_v<T> || is_basic_non_literal_parser_v<T>;

// \metafunctions and concepts