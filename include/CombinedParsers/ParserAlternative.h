#pragma once

#include <variant>

#include "common.h"
#include "utils_tuple.h"

namespace Parsers
{
	template<typename ... Parsers >
	class alternative
	{
		static_assert((ConceptParser<Parsers> && ...), "alternative::alternative(Parsers...parsers):: each parser should meet ConceptParser requirements");

	protected:
		template<ConceptCharType CharType, ConceptParser SkipperType, size_t index>
		auto UseParser(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType &skipper)
		{
			bool parsed = std::get<index>(tuple_parsers).Parse(ptr_string, ptr_string_end, skipper);
		
			if (parsed)
				m_result.emplace<index>(std::get<index>(tuple_parsers).GetValueAndReset());
			else
				std::get<index>(tuple_parsers).Reset();

			return parsed;
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t index>
		auto UseParser(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			bool parsed = std::get<index>(tuple_parsers).Parse(ptr_string, ptr_string_end);

			if (parsed)
				m_result.emplace<index>(std::get<index>(tuple_parsers).GetValueAndReset());
			else
				std::get<index>(tuple_parsers).Reset();

			return parsed;
		}
		
		template<ConceptCharType CharType, ConceptParser SkipperType, size_t index>
			auto UseParserAsScanner(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			return std::get<index>(tuple_parsers).Scan(ptr_string, ptr_string_end, skipper);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t index>
		auto UseParserAsScanner(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return std::get<index>(tuple_parsers).Scan(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t ... Ts>
		auto ParseImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType & skipper, std::index_sequence<Ts...> seq)
		{
			// this parameter pack extension stops on first successfull Parse() call
			return (UseParser<CharType, SkipperType, Ts>(ptr_string, ptr_string_end, skipper) || ...);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t ... Ts>
		auto ParseImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, std::index_sequence<Ts...> seq)
		{
			// this parameter pack extension stops on first successfull Parse() call
			return (UseParser<CharType, SkipperType, Ts>(ptr_string, ptr_string_end) || ...);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t ... Ts>
		auto ScanImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper, std::index_sequence<Ts...> seq)
		{
			// this parameter pack extension stops on first successfull Parse() call
			return (UseParserAsScanner<CharType, SkipperType, Ts>(ptr_string, ptr_string_end, skipper) || ...);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t ... Ts>
		auto ScanImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, std::index_sequence<Ts...> seq)
		{
			// this parameter pack extension stops on first successfull Parse() call
			return (UseParserAsScanner<CharType, SkipperType, Ts>(ptr_string, ptr_string_end) || ...);
		}


	public:
		std::tuple<Parsers...> tuple_parsers;

		using parsing_attribute = std::variant<typename Parsers::parsing_attribute ...>;
		parsing_attribute m_result{};

	public:
		alternative(Parsers...parsers)
			: tuple_parsers{parsers...}
		{
		}

		template<typename... T>
		alternative(alternative<T...> seq)
			: tuple_parsers{ seq.tuple_parsers }
		{
		}

		template<typename... T, typename TRight>
		alternative(alternative<T...> seq, TRight && right)
			: tuple_parsers{ tuple_utils::concat_as_tuple(seq.tuple_parsers, right) }
		{
		}

		template<typename TLeft, typename... T>
		alternative(TLeft&& left, alternative<T...> seq)
			: tuple_parsers{ tuple_utils::concat_as_tuple(left, seq.tuple_parsers) }
		{
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		bool Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType & skipper)
		{
			return ParseImpl(ptr_string, ptr_string_end, skipper, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		bool Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return ParseImpl(ptr_string, ptr_string_end, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		bool Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			return ScanImpl(ptr_string, ptr_string_end, skipper, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		bool Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return ScanImpl(ptr_string, ptr_string_end, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		auto GetValueAndReset()
		{
			return std::exchange(m_result, {});
		}
	};

	template<typename... AltTypes, typename TRight>
	alternative(alternative<AltTypes...> seq, TRight right) -> alternative<AltTypes..., TRight>;

	template<typename TLeft, typename... AltTypes>
	alternative(TLeft left, alternative<AltTypes...> seq) -> alternative<TLeft, AltTypes...>;
}