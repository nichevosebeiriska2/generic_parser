#pragma once

#include <type_traits>

#include "common.h"

namespace traits::parsers
{
	template<typename TParser, ConceptCharType CharType, typename TContext>
	struct attribute
	{
		using type = decltype(TParser::template GetReturnType<CharType, TContext>());
	};

	template<typename TParser, ConceptCharType CharType, typename TContext>
	using attribute_t = attribute<std::remove_cvref_t<TParser>, CharType, std::remove_cvref_t<TContext>>::type;

};


template<typename TParser, typename TAction>
class ParserWithActionNew
{
protected:
	TParser m_parser;
	TAction m_action;

	template<ConceptCharType CharType, ConceptContext TContext>
	using TChildParserReturnType = traits::parsers::attribute_t<TParser, CharType, std::remove_cvref_t<TContext>>;

public:
	template<ConceptCharType CharType, ConceptContext TContext>
	auto GetChildReturnType() { return TChildParserReturnType<CharType, TContext>{}; }

	template<ConceptCharType CharType, ConceptContext TContext>
	constexpr static auto GetReturnType()
	{
		using tReturnType = std::invoke_result_t<TAction, CActionContext<CharType, traits::parsers::attribute_t<TParser, CharType, TContext>>&>;

		static_assert(!std::is_same_v<tReturnType, void>, "ParserWithActionNew::GetReturnType()  - \"parser_with_action\" have to return value throught action");
		return tReturnType{};
	}

	ParserWithActionNew(TParser&& parser, TAction action)
		: m_parser{ parser }
		, m_action{ action }
	{}

	ParserWithActionNew(TParser& parser, TAction action)
		: m_parser{ parser }
		, m_action{ action }
	{}

	template<ConceptCharType CharType, ConceptContext TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
		, constCharPtrRef<CharType> ptr_string_end
		, TContext&& context
		, std::type_identity_t<decltype(GetReturnType<CharType, std::remove_cvref_t<TContext>>())>& attribute)
	{
		using tParserResultType = typename traits::parsers::attribute_t<TParser, CharType, decltype(context)>;
		tParserResultType parser_result{};
		CActionContext<CharType, decltype(parser_result)> actx{ parser_result };

		actx.SetBegin(ptr_string);
		bool parsed = m_parser.ParseNew(ptr_string, ptr_string_end, context, parser_result);
		actx.SetEnd(ptr_string);

		if (parsed)
			attribute = m_action(actx);

		return parsed;
	}
};

