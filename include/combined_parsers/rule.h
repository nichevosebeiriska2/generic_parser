#pragma once

#include "base_parser.h"

template<typename TRuleTag, typename TReturnType>
class RuleNew
{
public:
	using return_type = TReturnType;
	using tag = TRuleTag;

	RuleNew() = default;

	template<ConceptCharType CharType, ConceptContext TContext>
	constexpr static auto GetReturnType() { return return_type{}; };

	template<ConceptCharType CharType, ConceptContext TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext&& context, std::type_identity_t<return_type>& attribute) const;
};

#define IMPLEMENT_RULE_NEW(decl, impl)\
template<>\
template<ConceptCharType CharType, ConceptContext TContext>\
bool typename decltype(decl)::ParseNew(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end\
	, TContext&& context\
	, std::type_identity_t<decltype(decl)::return_type>& attribute) const\
{\
	using tRuleReturnType = traits::parsers::attribute<decltype(impl), CharType, std::remove_cvref_t<TContext>>::type;\
	static_assert(is_parser_with_action_v<decltype(impl)>, "RuleNew::ParseNew() - declaration should be \"parser_with_action\" type");\
	static_assert(std::is_same_v<tRuleReturnType , return_type> , "RuleNew::ParseNew() - \"parser_with_action\" has to return same type as rule");\
	return impl.ParseNew(ptr_string, ptr_string_end, context, attribute);\
};