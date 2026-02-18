#pragma once

#include <string>
#include "Scanners.h"
//#include "utils_string.h"
#include "declarations.h"
#include "scanner_aliases.h"

namespace Parsers
{

struct tag_attribute_unused{};

template<typename T>
struct is_unused_type : public std::false_type{};

template<>
struct is_unused_type<tag_attribute_unused> : public std::true_type{};

template<typename T>
constexpr bool is_unused_type_v = is_unused_type<T>::value;

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
		using parsing_attribute = TResult;
		using parsing_attribute_vec = std::vector<TResult>;

		template<ConceptCharType CharType>
		using scanner_type = TScanner<CharType>;
		using internal_char_type = TScanner<char>::internal_char_type;

protected:
	parsing_attribute m_result;

protected:

		template<ConceptCharType CharType>
		auto ParseOneToken(const CharType *& ptr_string, const CharType *&ptr_string_end)
		{
			auto scanner = TScanner<CharType>{};
			const bool scanned = scanner.Scan(ptr_string, ptr_string_end);

			auto result = ParseFromPointers(ptr_string, ptr_string + scanner.GetNumberOfScannedChars());
			ptr_string += std::get<2>(result);
			return result;
		}

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
		constexpr Parser()										noexcept {}; /* '= default' makes Parser aggregate type -> Parser always needs aggregate initialization for named variables ( Parser parser{} )  */
		constexpr Parser(const Parser &other) noexcept = default;
		constexpr Parser(Parser &&other)			noexcept = default;

		template<ConceptCharType CharType, typename TParserSkipper>
			requires(Parsers::ConceptSkipperParser<TParserSkipper>)
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
			requires(Parsers::ConceptSkipperParser<TParserSkipper>)
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
			return std::move(std::exchange(m_result, {}));
		}

		void Reset()
		{
			m_result = {};
		}
	};


	template<ConceptCharType CharType, template <typename> class TScanner, typename TResult>
	class ParserLiteral
	{
	public:
		using parsing_attribute = TResult;
		using scanner_type = TScanner<CharType>;
		using internal_char_type = scanner_type::internal_char_type;

		using const_pointer = const internal_char_type *;
		using const_pointer_ref = const internal_char_type *&;

	protected:
		scanner_type m_scanner;
		parsing_attribute m_result{};
		
	protected:

		template<ConceptCharType CharType>
		auto ParseOneToken(const CharType *&ptr_string, const CharType *&ptr_string_end/*, const auto &context*/)
		{
			auto scanner = m_scanner;

			const bool scanned = scanner.Scan(ptr_string, ptr_string_end);

			return ParseFromPointers(ptr_string, ptr_string + scanner.GetNumberOfScannedChars());
		}

		auto ParseFromPointers(const auto *ptr_string, const auto *ptr_string_end)
		{
			if constexpr((std::is_integral_v<parsing_attribute> || std::is_floating_point_v<parsing_attribute>) && !ConceptCharType<parsing_attribute>)
			{
				parsing_attribute result = 0;
				auto [ptr_end, error_code] = std::from_chars(ptr_string, ptr_string_end, result);
				return std::make_pair(ptr_end == ptr_string_end && ptr_end == ptr_string_end, result);
			}
			else if constexpr(ConceptCharType<parsing_attribute>) // parsing attibute is char/wchar_t
			{
				return std::make_pair(true, *ptr_string);
			}
			else // string
			{
				m_result = std::basic_string(ptr_string, ptr_string_end);
				return std::make_pair(true, std::basic_string(ptr_string, ptr_string_end));
			}
		}

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
			requires (std::is_same_v<scanner_type, Scanners::ScannerStringLiteral<internal_char_type>>)
			: m_scanner{view}
		{
		};

		constexpr ParserLiteral(CharType *view) noexcept
			requires (std::is_same_v<scanner_type, Scanners::ScannerStringLiteral<internal_char_type>>)
			: m_scanner{view}
		{
		};

		constexpr ParserLiteral(CharType symbol) noexcept
			requires (std::is_same_v<scanner_type, Scanners::scanner_symbol<internal_char_type>>)
			: m_scanner{symbol}
		{
		};



		template<ConceptCharType CharType, typename TParserSkipper>
		bool Parse(const CharType *&ptr_string, const CharType *&ptr_string_end, TParserSkipper &skipper)
			requires(Parsers::ConceptSkipperParser<TParserSkipper>)
		{
			UseSkipper(ptr_string, ptr_string_end, skipper);

			static_assert(std::is_same_v<CharType, internal_char_type>, "ParserLiteral::Scan(const CharType *&ptr_string, const auto *&ptr_string_end, const auto &skipper) - char type should be the same as internal_char_type");
			
			return Parse(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType>
		bool Parse(const CharType *&ptr_string, const CharType *&ptr_string_end)
		{
			static_assert(std::is_same_v<CharType, internal_char_type>, "ParserLiteral::Scan(const CharType *&ptr_string, const auto *&ptr_string_end, const auto &skipper) - char type should be the same as internal_char_type");

			if(!m_scanner.Scan(ptr_string, ptr_string_end))
				return false;

			auto [parsed, result] = ParseFromPointers(ptr_string, ptr_string + m_scanner.GetNumberOfScannedChars());
			ptr_string += m_scanner.GetNumberOfScannedChars();
			m_result = result;

			return parsed;
		}

		template<ConceptCharType CharType, typename TParserSkipper>
		bool Scan(const CharType *&ptr_string, const CharType *&ptr_string_end, TParserSkipper &skipper)
			requires(Parsers::ConceptSkipperParser<TParserSkipper>)
		{
			static_assert(std::is_same_v<CharType, internal_char_type>, "ParserLiteral::Scan(const CharType *&ptr_string, const CharType *&ptr_string_end) - char type should be the same as internal_char_type");

			UseSkipper(ptr_string, ptr_string_end, skipper);

			return Scan(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType>
		bool Scan(const CharType *&ptr_string, const CharType *&ptr_string_end)
		{
			const bool scanned = m_scanner.Scan(ptr_string, ptr_string_end);

			if(scanned)
				ptr_string += m_scanner.GetNumberOfScannedChars();

			return scanned;
		}

		constexpr auto GetValueAndReset()
		{
			return std::move(std::exchange(m_result, {}));
		}

		void Reset()
		{
			m_result = {};
		}
	};
}