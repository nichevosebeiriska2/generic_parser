#pragma once

#include <tuple>
#include <vector>

#include "common.h"
#include "utils_tuple.h"
#include "ParserBase.h"
#include "declarations.h"


namespace Parsers
{
	template<typename ... Parsers >
	class sequential
	{
		//static_assert(sizeof...(Parsers) > 1 || (sizeof...(Parsers) == 1 && ), "sequential<Parsers...>:: <Parsers...> template packet size must be greater than '1'");
		static_assert((ConceptParser<Parsers> && ...), "sequential::sequential(Parsers...parsers):: each parser should meet ConceptParser requirements");
	
	public:
		std::tuple<Parsers...> tuple_parsers;

	protected:
		template<ConceptCharType CharType, ConceptParser SkipperType, size_t index>
		auto UseParser(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType & skipper)
		{
			return std::get<index>(tuple_parsers).Parse(ptr_string, ptr_string_end, skipper);
		}

		template<ConceptCharType CharType, size_t index>
		auto UseParser(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return std::get<index>(tuple_parsers).Parse(ptr_string, ptr_string_end);
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

		template<int index>
		constexpr static auto EvaluateTypeSingleParser() noexcept
		{
			using tParser = std::remove_cvref_t<decltype(std::get<index>(tuple_parsers))>;

			if constexpr (is_unused_type_v <typename tParser::parsing_attribute>)
				return tag_attribute_unused{};
			else
				return typename tParser::parsing_attribute{};
		}

		template<size_t ... Ts>
		constexpr auto EvaluateResultType(std::index_sequence<Ts...> seq)
		{
			return std::make_tuple(EvaluateTypeSingleParser<Ts>() ...);
		}

		template<size_t ... Ts>
		constexpr static auto EvaluateResultTypeImpl(std::index_sequence<Ts...> seq)
		{
			return tuple_utils::TupleDeleteParamsOfRequiredType<tag_attribute_unused>(std::make_tuple(typename std::decay_t<decltype(std::get<Ts>(tuple_parsers))>::parsing_attribute{} ... ));
		}

		constexpr static auto EvaluateResultType2()
		{
			return EvaluateResultTypeImpl(std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t index>
		bool ParseSingleParser(auto &tup, constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			using tParser = std::remove_cvref_t<decltype(std::get<index>(tuple_parsers))>;
			if constexpr(!is_unused_type_v<typename tParser::parsing_attribute>)
			{
				bool parsed = UseParser<CharType, SkipperType, index>(ptr_string, ptr_string_end, skipper);

				if(parsed)
					std::get<index>(tup) = std::get<index>(tuple_parsers).GetValueAndReset();

				return parsed;
			}
			else // omited parsers use Scan() inside Parse() function so no overhead expected
				return UseParser<CharType, SkipperType, index>(ptr_string, ptr_string_end, skipper);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t ... Ts>
		auto ParseImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType & skipper, const std::index_sequence<Ts...>& seq)
		{
			// there is some overhead due to copying tup_temp to m_result with delete of tag_unused attibutes. 
			// I do not see other way to properly associate parser index with output tuple index yet. 
			// It is deffinitely should be optimized furthere 
			decltype(EvaluateResultType(seq)) tup_temp{};
			
			// its neccessary to use smth like (&& ...) to guarantee left-to-right order of evaluation (look for 'https://en.cppreference.com/w/cpp/language/eval_order.html')
			auto parsed = (ParseSingleParser<CharType, SkipperType, Ts>(tup_temp, ptr_string, ptr_string_end, skipper) && ...);

			if (parsed)
				m_result = tuple_utils::TupleDeleteParamsOfRequiredType<tag_attribute_unused>(tup_temp);

			return parsed;
		}

		template<ConceptCharType CharType, size_t ... Ts>
		auto ParseImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, const std::index_sequence<Ts...>& seq)
		{
			decltype(EvaluateResultType(seq)) tup_temp{};

			auto parsed = (ParseSingleParser<CharType, Ts>(tup_temp, ptr_string, ptr_string_end) && ...);

			if (parsed)
				m_result = tuple_utils::TupleDeleteParamsOfRequiredType<tag_attribute_unused>(tup_temp);

			return parsed;
		}

		template<ConceptCharType CharType, ConceptParser SkipperType, size_t ... Ts>
		auto ScanImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper, const std::index_sequence<Ts...>& seq)
		{
			return (UseParserAsScanner<CharType, SkipperType, Ts>(ptr_string, ptr_string_end, skipper) && ...);
		}

		template<ConceptCharType CharType, size_t ... Ts>
		auto ScanImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, const std::index_sequence<Ts...>& seq)
		{
			return (UseParserAsScanner<CharType, Ts>(ptr_string, ptr_string_end) && ...);
		}

	public:
		using parsing_attribute = decltype(EvaluateResultType2());

		parsing_attribute m_result;

	public:
		sequential(Parsers&& ... parsers)
			: tuple_parsers{parsers...}
		{
		}

		sequential(const Parsers& ... parsers)
			: tuple_parsers{parsers...}
		{
		}

		sequential(const sequential<Parsers...> &other)
			: tuple_parsers{other.tuple_parsers}
		{
		}

		sequential(sequential<Parsers...> &&other) noexcept
			: tuple_parsers{std::move(other.tuple_parsers)}
		{
		}
		
		template<typename... T, typename TRight>
		sequential(sequential<T...> seq, TRight && right)
			: tuple_parsers{std::tuple_cat(tuple_utils::as_tuple(seq.tuple_parsers), tuple_utils::as_tuple(right))}
		{
		}

		template<typename TLeft, typename... T>
		sequential(TLeft && left, sequential<T...> seq)
			: tuple_parsers{std::tuple_cat(tuple_utils::as_tuple(left), tuple_utils::as_tuple(seq.tuple_parsers))}
		{
		}

		template<typename... TLeft, typename... TRight>
		sequential(sequential<TLeft...> seq_left, sequential<TRight...> seq_right)
			: tuple_parsers{std::tuple_cat(tuple_utils::as_tuple(seq_left.tuple_parsers), tuple_utils::as_tuple(seq_right.tuple_parsers))}
		{
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		auto Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType & skipper)
		{
			return ParseImpl(ptr_string, ptr_string_end, skipper, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		auto Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return ParseImpl(ptr_string, ptr_string_end, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		auto Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			return ScanImpl(ptr_string, ptr_string_end, skipper, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		template<ConceptCharType CharType>
		auto Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			return ScanImpl(ptr_string, ptr_string_end, std::make_index_sequence<std::tuple_size_v<decltype(tuple_parsers)>>{});
		}

		auto GetValueAndReset()
		{
			return std::exchange(m_result, {});
		}

		void Reset()
		{
			m_result = {};
		}

		constexpr static INT get_num_of_parsers()
		{
			return std::tuple_size_v<decltype(tuple_parsers)>;
		}

		constexpr static bool IsOmited()
		{
			auto lambda_check_parser_omited = [&]<size_t index>()  constexpr { return std::tuple_element_t<index, decltype(tuple_parsers)>::IsOmited(); };
			auto lambda_check_each = [&]<size_t ... Ts>(std::index_sequence<Ts...> indices) constexpr { return (lambda_check_parser_omited.operator() < Ts > () && ...); };

			return lambda_check_each(std::make_index_sequence<get_num_of_parsers()>{});
		}
	};

	template<typename TLeft, typename TRight>
	sequential(TLeft left, TRight right) -> sequential<TLeft, TRight>;

	template<typename... SeqTypes, typename TRight>
	sequential(sequential<SeqTypes...> seq, TRight right) -> sequential<SeqTypes..., TRight>;

	template<typename TLeft, typename... SeqTypes>
	sequential(TLeft left, sequential<SeqTypes...> seq) -> sequential<TLeft, SeqTypes...>;

	template<typename... SeqTypesLeft, typename... SeqTypesRight>
	sequential(sequential<SeqTypesLeft...> seq_left, sequential<SeqTypesRight...> seq_right) -> sequential<SeqTypesLeft..., SeqTypesRight...>;

	template<typename T>
	struct is_sequential : std::false_type{};

	template<typename... Parsers>
	struct is_sequential<sequential<Parsers...>> : std::true_type{};

}