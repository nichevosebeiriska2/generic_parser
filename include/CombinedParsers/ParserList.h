#pragma once

#include <tuple>
#include <vector>

#include "ParserBase.h"

namespace Parsers
{
	template<ConceptParser TParser, ConceptParser TParserDelimiter >
	class list
	{

	public:
		using parsing_attribute = std::vector<typename TParser::parsing_attribute>;

	

	protected:
		TParser m_parser;
		TParserDelimiter m_parser_delimiter;
		parsing_attribute m_vector_result;
		const INT m_num_of_elements_to_parse{ CONST_NUMBER_OF_CHARS_AT_LEAST_ONE };

	protected:
	protected:
		template<ConceptCharType CharType, ConceptParser SkipperType>
		auto ParseImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			constCharPtrRef<CharType> ptr_last_parsed_value_end = ptr_string;

			// what would happen if skipper and parser_delimitar has the same single symbol( space for example ) ?
			// seems like i have to implement some additional requirements for this case
			
			UINT count = m_num_of_elements_to_parse;
			bool delimiter_scanned = true;

			while (m_parser.Parse(ptr_string, ptr_string_end, skipper) && delimiter_scanned && count != 0)
			{
				m_vector_result.emplace_back(m_parser.GetValueAndReset());
				ptr_last_parsed_value_end = ptr_string;
				count--;

				delimiter_scanned = m_parser_delimiter.Scan(ptr_string, ptr_string_end, skipper);
			}

			ptr_string = ptr_last_parsed_value_end;

			bool parsed_required_num_of_arg = false;
			if (m_num_of_elements_to_parse < 0)
				parsed_required_num_of_arg = m_num_of_elements_to_parse == CONST_NUMBER_OF_CHARS_AT_LEAST_ONE ? !m_vector_result.empty() : true;
			else
				parsed_required_num_of_arg = m_vector_result.size() == m_num_of_elements_to_parse;

			if (!parsed_required_num_of_arg)
				Reset();


			return parsed_required_num_of_arg;
		}


		template<ConceptCharType CharType, ConceptParser SkipperType>
		auto ScanImpl(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			constCharPtrRef<CharType> ptr_last_parsed_value_end = ptr_string;

			// what would happen if skipper and parser_delimitar has the same single symbol( space for example ) ?
			// seems like i have to implement some additional requirements for this case
			INT count = m_num_of_elements_to_parse;

			while (m_parser.Scan(ptr_string, ptr_string_end, skipper) && m_parser_delimiter.Scan(ptr_string, ptr_string_end, skipper) && (count--) != 0)
				ptr_last_parsed_value_end = ptr_string;

			ptr_string = ptr_last_parsed_value_end;

			if (m_num_of_elements_to_parse < 0)
				return m_num_of_elements_to_parse == CONST_NUMBER_OF_CHARS_AT_LEAST_ONE ? !m_vector_result.empty() : true;

			return m_vector_result.size() == m_num_of_elements_to_parse;
		}
	public:
		template<typename P, typename D>
		list(P && p, D && pd, INT num_of_elements_to_parse = CONST_NUMBER_OF_CHARS_AT_LEAST_ONE)
			: m_parser{p}
			, m_parser_delimiter{pd}
			, m_num_of_elements_to_parse{num_of_elements_to_parse}	// for cases we want parse a non-fixed number of arguments use constants
																	// CONST_NUMBER_OF_CHARS_AT_LEAST_ONE/ CONST_NUMBER_OF_CHARS_ZERO_OR_MORE
		{
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		auto Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			return ParseImpl(ptr_string, ptr_string_end, skipper);
		}

		template<ConceptCharType CharType, ConceptParser SkipperType>
		auto Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, SkipperType& skipper)
		{
			return ScanImpl(ptr_string, ptr_string_end, skipper);
		}

		auto GetValueAndReset()
		{
			m_parser.Reset();
			return std::exchange(m_vector_result, {});
		}
		void Reset()
		{
			m_parser.Reset();
			m_vector_result = {};
		}

		static constexpr bool IsOmited()
		{
			return decltype(m_parser)::IsOmited();
		}
	};

	template<typename TParser, typename TDelimiter>
	list(TParser parser, TDelimiter delimiter) -> list<TParser, TDelimiter>;

	template<typename TParser, typename TDelimiter>
	list(TParser parser, TDelimiter delimiter, int N) -> list<TParser, TDelimiter>;
}