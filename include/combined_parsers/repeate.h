#pragma once

#include "base_parser.h"

template<typename TParser>
class ParserRepeateNew
{
	TParser m_parser;
	size_t number_of_repeats;

public:

	ParserRepeateNew(const ParserRepeateNew<TParser>& other)
		: m_parser{ other.m_parser }
		, number_of_repeats{ other.number_of_repeats }
	{}

	ParserRepeateNew(ParserRepeateNew<TParser>&& other)
		: m_parser{ std::move(other.m_parser) }
		, number_of_repeats{ other.number_of_repeats }
	{}

	ParserRepeateNew(ParserRepeateNew<TParser>& other, size_t N)
		: m_parser{ other.m_parser }
		, number_of_repeats{ N }
	{}

	ParserRepeateNew(ParserRepeateNew<TParser>&& other, size_t N)
		: m_parser{ std::move(other.m_parser) }
		, number_of_repeats{ N }
	{}

	ParserRepeateNew(TParser&& parser, size_t N)
		: m_parser{ std::forward<TParser>(parser)}
		, number_of_repeats{ N }
	{}

	ParserRepeateNew(const TParser& parser, size_t N)
		: m_parser{ parser }
		, number_of_repeats{ N }
	{}


	template<ConceptCharType CharType, ConceptContext TContext>
	constexpr static auto GetReturnType()
	{
		return std::vector<decltype(TParser::template GetReturnType<CharType, TContext>())>{};
	}

	template<ConceptCharType CharType, ConceptContext TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
		, constCharPtrRef<CharType> ptr_string_end
		, TContext&& context
		, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())>& attribute) const
	{
		UINT count = 0;
		context.UseSkipper(ptr_string, ptr_string_end);// pre-skip
		
		while (m_parser.ParseNew(ptr_string, ptr_string_end, context, attribute.emplace_back()) && count++ < number_of_repeats)// what about non default/copy constructible? looks like we have a problems is case TParser has parsing action
		{
		}


		if (number_of_repeats == CONST_NUMBER_OF_CHARS_AT_LEAST_ONE)
			return !attribute.empty();
		else if (number_of_repeats == CONST_NUMBER_OF_CHARS_ZERO_OR_MORE)
			return true;

		return attribute.size() == number_of_repeats;
	}

	auto operator[](auto&& callable)
	{
		return ParserWithActionNew(*this, callable);
	}
};

template<ConceptNewParser TParser>
ParserRepeateNew(ParserRepeateNew<TParser>&& other) -> ParserRepeateNew <TParser>;

template<ConceptNewParser TParser>
auto operator+(TParser&& parser)
{
	return ParserRepeateNew(parser, CONST_NUMBER_OF_CHARS_AT_LEAST_ONE);
}

template<ConceptNewParser TParser>
auto operator*(TParser&& parser)
{
	return ParserRepeateNew(parser, CONST_NUMBER_OF_CHARS_ZERO_OR_MORE);
}

template<ConceptNewParser TParser>
auto operator *(UINT N, TParser&& parser)
{
	return ParserRepeateNew(parser, N);
}