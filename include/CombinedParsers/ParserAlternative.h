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

	public:
		std::tuple<Parsers...> tuple_parsers;

		static constexpr bool IsOmited()
		{
			auto lambda_check_parser_omited = [&]<size_t index>()  constexpr { return std::tuple_element_t<index, decltype(tuple_parsers)>::IsOmited(); };
			auto lambda_check_each = [&]<size_t ... Ts>(std::index_sequence<Ts...> indices) constexpr { return (lambda_check_parser_omited.operator() < Ts > () || ...); };

			return lambda_check_each(std::make_index_sequence<get_num_of_parsers()>{});
		}

	protected:

		static consteval size_t get_num_of_parsers()
		{
			return std::tuple_size_v<decltype(tuple_parsers)>;
		}

		constexpr auto make_indices()
		{
			return std::make_index_sequence<get_num_of_parsers()>{};
		}

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

		template<ConceptCharType CharType, size_t index>
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

		template<ConceptCharType CharType, size_t index>
		auto UseParserAsScanner(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return std::get<index>(tuple_parsers).Scan(ptr_string, ptr_string_end);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t ... Ts>
		auto ParseImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType & skipper, std::index_sequence<Ts...> seq)
		{
			// this parameter pack extension stops on first successfull Parse() call
			auto lambda = [&]<size_t index>() constexpr
				{
					using t = std::tuple_element_t<index, decltype(tuple_parsers)>;
					if constexpr (!std::remove_cvref_t<t>::IsOmited())
					{
						bool parsed =  std::get< index>(tuple_parsers).Parse(ptr_string, ptr_string_end, skipper);
					
						if (parsed)
							m_result.emplace<index>(std::get< index>(tuple_parsers).GetValueAndReset());// = std::get< index>(tuple_parsers).GetValueAndReset();

						return parsed;
					}
					else
						return std::get< index>(tuple_parsers).Scan(ptr_string, ptr_string_end, skipper);

				};

			return (lambda.operator()<Ts>() || ...);
			//return (UseParser<CharType, SkipperType, Ts>(ptr_string, ptr_string_end, skipper) || ...);
		}

		template<ConceptCharType CharType, size_t ... Ts>
		auto ParseImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, std::index_sequence<Ts...> seq)
		{
			// this parameter pack extension stops on first successfull Parse() call
			//return (UseParser<CharType, Ts>(ptr_string, ptr_string_end) || ...);
			auto lambda = [&]<size_t index>() constexpr
			{
				if constexpr (std::get< index>(tuple_parsers).IsOmited())
				{
					bool parsed = std::get< index>(tuple_parsers).Parse(ptr_string, ptr_string_end);

					if (parsed)
						m_result = std::get< index>(tuple_parsers).GetValueAndReset();

					return parsed;
				}
				else
					return std::get< index>(tuple_parsers).Scan(ptr_string, ptr_string_end);

			};

			return (lambda.operator() < Ts > () || ...);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t ... Ts>
		auto ScanImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper, std::index_sequence<Ts...> seq)
		{
			// this parameter pack extension stops on first successfull Parse() call
			return (UseParserAsScanner<CharType, SkipperType, Ts>(ptr_string, ptr_string_end, skipper) || ...);
		}

		template<ConceptCharType CharType, size_t ... Ts>
		auto ScanImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, std::index_sequence<Ts...> seq)
		{
			// this parameter pack extension stops on first successfull Parse() call
			return (UseParserAsScanner<CharType, Ts>(ptr_string, ptr_string_end) || ...);
		}


	public:

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

		template<typename ... TAltLeft, typename... TAltRight>
		alternative(alternative<TAltLeft...> left, alternative<TAltRight...> right)
			: tuple_parsers{ tuple_utils::concat_as_tuple(left.tuple_parsers, right.tuple_parsers) }
		{
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		bool Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType & skipper)
		{
			return ParseImpl(ptr_string, ptr_string_end, skipper, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType>
		bool Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return ParseImpl(ptr_string, ptr_string_end, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		bool Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			return ScanImpl(ptr_string, ptr_string_end, skipper, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType>
		bool Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return ScanImpl(ptr_string, ptr_string_end, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		auto GetValueAndReset()
		{
			return std::exchange(m_result, {});
		}


	};

	template<typename TLeft, typename TRight>
	alternative(TLeft&& left, TRight&& right) -> alternative<std::remove_cvref_t<TLeft>, std::remove_cvref_t<TRight>>;

	template<typename... AltTypes>
	alternative(alternative< AltTypes...>&& alt) -> alternative<AltTypes...>;

	template<typename... AltTypes, typename TRight>
	alternative(alternative<AltTypes...>&& seq, TRight&& right) -> alternative<AltTypes..., std::remove_cvref_t<TRight>>;

	template<typename... AltTypes, typename TRight>
	alternative(alternative<AltTypes...>& seq, TRight&& right) -> alternative<AltTypes..., std::remove_cvref_t<TRight>>;

	template<typename TLeft, typename... AltTypes>
	alternative(TLeft&& left, alternative<AltTypes...>&& seq) -> alternative< std::remove_cvref_t<TLeft>, AltTypes...>;

	template<typename TLeft, typename... AltTypes>
	alternative(TLeft&& left, alternative<AltTypes...>& seq) -> alternative< std::remove_cvref_t<TLeft>, AltTypes...>;

	template<typename... AltTypesLeft, typename... AltTypesRight>
	alternative(alternative<AltTypesLeft...>&& altLeft, alternative<AltTypesRight...>&& altRight) -> alternative<AltTypesLeft..., AltTypesRight...>;

	template<typename... AltTypesLeft, typename... AltTypesRight>
	alternative(alternative<AltTypesLeft...>& altLeft, alternative<AltTypesRight...>&& altRight) -> alternative<AltTypesLeft..., AltTypesRight...>;
	
	template<typename... AltTypesLeft, typename... AltTypesRight>
	alternative(alternative<AltTypesLeft...>&& altLeft, alternative<AltTypesRight...>& altRight) -> alternative<AltTypesLeft..., AltTypesRight...>;

	template<typename... AltTypesLeft, typename... AltTypesRight>
	alternative(alternative<AltTypesLeft...>& altLeft, alternative<AltTypesRight...>& altRight) -> alternative<AltTypesLeft..., AltTypesRight...>;
}