#pragma once

#include <type_traits>

#include "macro.h"

using UINT = uint32_t;
using INT  = int32_t;
// concepts
template<typename T>
concept ConceptCharType = std::is_same_v<T, char> || std::is_same_v<T, wchar_t>;

template<ConceptCharType CharType>
using constCharPtr = const CharType*; // use it to declare const ptr reference to underlying char type in template Parse()/Scan() functions. 

template<ConceptCharType CharType>
using constCharPtrRef = const CharType*&; // use it to declare const ptr reference to underlying char type in template Parse()/Scan() functions. 

template<typename T, typename CharType>
concept ConceptHasParseFunction = requires(T && t, constCharPtr<CharType> ptr)
{
	requires std::is_same_v<bool, decltype(t.Parse(ptr, ptr))>;
};

template<typename T, typename CharType>
concept ConceptHasScanFunction = requires(T && t, const CharType* ptr)
{
	requires std::is_same_v<bool, decltype(t.Scan(ptr, ptr))>;
};

// there is pretty much all requirements to parser as a type : it has to have Parse() and Scan() functions and alias for its parsing result 
// move/copy construclible is not really required tbh. it is more for the sake of optimization
template<typename T>
concept ConceptParser = requires(T t)
{
	// you need to drop qualifiers inside requires expression. otherwise you will get compiler error using ConceptParser as universal reference type
	typename std::remove_cvref_t<decltype(t)>::parsing_attribute;

	requires std::is_move_constructible_v<T>;
	requires std::is_copy_constructible_v<T>;

	{ std::remove_cvref_t<decltype(t)>::IsOmited()} -> std::same_as<bool>;

	requires ConceptHasParseFunction<T, char> /*&& ConceptHasParseFunction<T, wchar_t>*/;
	requires ConceptHasScanFunction<T, char> /*&& ConceptHasScanFunction<T, wchar_t>*/;
};


template<typename T> 
concept ConceptScanner = requires(T t)
{
	requires std::is_move_constructible_v<T>;
	requires std::is_copy_constructible_v<T>;

	{t.GetNumberOfScannedChars() } -> std::same_as<int>;
	requires ConceptHasScanFunction<T, char> && ConceptHasScanFunction<T, wchar_t>;
};

// \concepts

// metafunctions 
struct tag_attribute_unused {}; // compile time tag of no attribute parser

template<typename T>
struct is_unused_type : public std::false_type {};

template<>
struct is_unused_type<tag_attribute_unused> : public std::true_type {};

template<typename T>
constexpr bool is_unused_type_v = is_unused_type<T>::value;

struct tag_skipper_non
{
}; // compile time tag of no attribute parser

template<typename T>
struct is_skipper_non_type : public std::false_type
{
};

template<>
struct is_skipper_non_type<tag_skipper_non> : public std::true_type
{
};

template<typename T>
constexpr bool is_skipper_non_type_v = is_skipper_non_type<T>::value;

template<typename T>
struct is_parser : std::false_type {};

template<ConceptParser ParserType>
struct is_parser<ParserType> : std::true_type {};

template<typename T>
constexpr bool is_parser_v = is_parser<T>::value;

// \metafunctions 


template<template<typename> class TScanner, ConceptCharType CharType>
class ParserLiteralWithContext;

template<typename TScanner>
class ParserWithContext;

template<typename ... TParsers>
class ParserAltNew;

template<typename ... TParsers>
class ParserSeqNew;

template<typename TParser, typename TParserDelimiter>
class ParserListNew;

template<typename TParser>
class ParserRepeateNew;

template<typename TRuleTag, typename TReturnType>
class RuleNew;

template<typename TParser, typename TAction>
class ParserWithActionNew;

template<typename TParser>
class ParserPredicateNon;

// base
template<typename T>
struct is_base_parser : public std::false_type {};

template<template<typename> class TScanner, ConceptCharType CharType>
struct is_base_parser<ParserLiteralWithContext<TScanner, CharType>> : public std::true_type {};

template<typename TScanner>
struct is_base_parser<ParserWithContext<TScanner>> : public std::true_type {};

template<typename T>
constexpr bool is_base_parser_v = is_base_parser< std::remove_cvref_t<T>>::value;

//alt
template<typename T>
struct is_alternative_parser : public std::false_type {};

template<typename ... TParsers>
struct is_alternative_parser<ParserAltNew<TParsers...>> : public std::true_type {};

template<typename T>
constexpr bool is_alternative_parser_v = is_alternative_parser< std::remove_cvref_t<T>>::value;


// list
template<typename T>
struct is_list_parser : public std::false_type {};

template<typename TParser, typename TParserDelimiter>
struct is_list_parser<ParserListNew<TParser, TParserDelimiter>> : public std::true_type {};

template<typename T>
constexpr bool is_list_parser_v = is_list_parser< std::remove_cvref_t<T>>::value;

//repeate 
template<typename T>
struct is_repeate_parser : public std::false_type {};

template<typename TParser>
struct is_repeate_parser<ParserRepeateNew<TParser>> : public std::true_type {};

template<typename T>
constexpr bool is_repeate_parser_v = is_repeate_parser< std::remove_cvref_t<T>>::value;

//seq
template<typename T>
struct is_new_sequential : std::false_type {};

template<typename... Parsers>
struct is_new_sequential<ParserSeqNew<Parsers...>> : std::true_type {};

template<typename T>
constexpr bool is_new_sequential_v = is_new_sequential< std::remove_cvref_t<T>>::value;

//rule
template<typename T>
struct is_rule_new : std::false_type {};

template<typename T, typename U>
struct is_rule_new<RuleNew<T, U>> : std::true_type {};

template<typename T>
constexpr bool is_rule_new_v = is_rule_new<std::remove_cvref_t<T>>::value;

// with_action
template<typename T>
struct is_parser_with_action : std::false_type {};

template<typename TParser, typename TAction>
struct is_parser_with_action<ParserWithActionNew<TParser, TAction>> : std::true_type {};

template<typename T>
constexpr bool is_parser_with_action_v = is_parser_with_action<std::remove_cvref_t<T>>::value;

template<typename T>
struct is_parser_predicate_not : std::false_type{};

template<typename T>
struct is_parser_predicate_not<ParserPredicateNon<T>> : std::true_type{};

template<typename T>
constexpr bool is_parser_predicate_not_v = is_parser_predicate_not<std::remove_cvref_t<T>>::value;

// concept
template<typename T>
concept ConceptNewParser = (is_base_parser_v< std::remove_cvref_t<T>>
														|| is_alternative_parser_v< std::remove_cvref_t<T>>
														|| is_new_sequential_v< std::remove_cvref_t<T>>
														|| is_repeate_parser_v< std::remove_cvref_t<T>>
														|| is_list_parser_v< std::remove_cvref_t<T>>
														|| is_rule_new_v< std::remove_cvref_t<T>>
														|| is_parser_with_action_v<std::remove_cvref_t<T>>
														||is_parser_predicate_not_v<std::remove_cvref_t<T>>)
														&& std::move_constructible<T> 
														&& std::copy_constructible<T>;