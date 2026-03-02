#pragma once

#include <variant>
#include <type_traits>

#include "common.h"
#include "ParserBase.h"

namespace Parsers {

	template<ConceptParser TParser>
	class ParserPredicateNot
	{
		TParser m_parser;
	public:
		using parsing_attribute = tag_attribute_unused;

	protected:


	public:
		constexpr ParserPredicateNot(TParser&& p)
			: m_parser{ p }
		{
		}

		constexpr ParserPredicateNot(const TParser& p)
			: m_parser{ p }
		{
		}

		constexpr ParserPredicateNot(const ParserPredicateNot<TParser>& other)
			: m_parser{ other.m_parser }
		{
		}

		constexpr ParserPredicateNot(ParserPredicateNot<TParser>&& other)
			: m_parser{ other.m_parser }
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
			auto ptr_temp = ptr_string;
			const bool scanned = m_parser.Scan(ptr_string, ptr_string_end, skipper);

			ptr_string = ptr_temp;
			return !scanned;
		}

		template<ConceptCharType CharType>
		auto Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
		{
			constCharPtr<CharType>ptr_begin = ptr_string;
			auto ptr_temp = ptr_string;
			const bool scanned = m_parser.Scan(ptr_string, ptr_string_end);

			ptr_string = ptr_temp;

			return !scanned;
		}

		constexpr auto GetValueAndReset()
		{
			return tag_attribute_unused{};
		}
		void Reset()
		{
			if constexpr (!std::remove_cvref_t<decltype(m_parser)>::IsOmited())
				m_parser.Reset();
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

	template<ConceptParser TParser>
	using predicate_not = ParserPredicateNot<TParser>;
};