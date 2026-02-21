#pragma once

#include "common.h"
#include "ParserBase.h"

// deductions guides for each parser type collected here
namespace Parsers
{

///////////////////////////////////////
//						base parser						 //
///////////////////////////////////////

//template<template<typename> class TScanner, typename TResult>
//class Parser;
//
////template<ConceptCharType CharType, template <typename> class TScanner, typename TResult>
////class ParserLiteral;
//
/////////////////////////////////////////
////							repeate
/////////////////////////////////////////
//
//template<typename TParser, int N = CONST_NUMBER_OF_CHARS_AT_LEAST_ONE>
//class repeate;
//template<typename TParser>
//repeate(repeate<TParser>) -> repeate<TParser>;
//
//template<typename TParser>
//repeate(const repeate<TParser> &) -> repeate<TParser>;
//
//template<typename TParser>
//repeate(repeate<TParser> &&) -> repeate<TParser>;
//
//template<typename T>
//struct is_repeate : std::false_type{};
//
//template<typename T, int N>
//struct is_repeate <repeate<T, N>> : std::true_type{};
//
//template<typename T>
//inline constexpr bool is_repeate_v = is_repeate<T>::value;
//
//template<typename TParser, int N>
//repeate(const repeate<TParser, N> & ) -> repeate<TParser, N>;
//
//template<typename TParser, int N>
//repeate(repeate<TParser, N> &&) -> repeate<TParser, N>;
//
//
/////////////////////////////////////////
////								omited
/////////////////////////////////////////
//
//
/////////////////////////////////////////
////						 sequential
/////////////////////////////////////////
//
//template<typename ... Parsers >
//class sequential;
//
//template<typename TLeft, typename TRight>
//sequential(TLeft left, TRight right) -> sequential<TLeft, TRight>;
//
//template<typename... SeqTypes, typename TRight>
//sequential(sequential<SeqTypes...> seq, TRight right) -> sequential<SeqTypes..., TRight>;
//
//template<typename TLeft, typename... SeqTypes>
//sequential(TLeft left, sequential<SeqTypes...> seq) -> sequential<TLeft, SeqTypes...>;
//
//template<typename... SeqTypesLeft, typename... SeqTypesRight>
//sequential(sequential<SeqTypesLeft...> seq_left, sequential<SeqTypesRight...> seq_right) -> sequential<SeqTypesLeft..., SeqTypesRight...>;
//
//template<typename T>
//struct is_sequential : std::false_type{};
//
//template<typename... Parsers>
//struct is_sequential<sequential<Parsers...>> : std::true_type{};
//
//template<typename T>
//inline constexpr bool is_sequential_v = is_sequential<T>::value;
//
/////////////////////////////////////////
////								list
/////////////////////////////////////////
//
//template<typename TParser, typename TParserDelimiter >
//class list;
//
//template<typename T>
//struct is_list : std::false_type{};
//
//template<typename TParsers, typename TParserDelimiter>
//struct is_list<list<TParsers, TParserDelimiter>> : std::true_type{};
//
//template<typename T>
//inline constexpr bool is_list_v = is_list<T>::value;
//
/////////////////////////////////////////
////						alternative
/////////////////////////////////////////
//
//template<typename ... Parsers >
//class alternative;
//
//template<typename T>
//struct is_alternative : std::false_type
//{
//};
//
//template<typename ... TParsers>
//struct is_alternative<alternative<TParsers...>> : std::true_type
//{
//};
//
//template<typename T>
//inline constexpr bool is_alternative_v = is_alternative<T>::value;
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//template <typename T>
//struct is_base_parser_wrapper : public std::false_type{};
//
//template <typename T>
//struct is_base_parser_literal_wrapper : public std::false_type{};
//
//template<template<typename> class TScanner, typename TResult>
//struct is_base_parser_wrapper <Parser<TScanner, TResult>> : public std::true_type{};
//
//template<ConceptCharType CharType, template<typename> class TScanner, typename TResult>
//struct is_base_parser_literal_wrapper <ParserLiteral<CharType, TScanner, TResult>> : public std::true_type{};
//
//template<ConceptCharType CharType, template<typename> class TScanner, typename TResult>
//struct is_base_parser_literal_wrapper <omited<ParserLiteral<CharType, TScanner, TResult>>> : public std::true_type{};
//
//template<typename T>
//inline constexpr bool is_base_parser_v = is_base_parser_wrapper<std::remove_cvref_t<T>>::value;
//
//template<typename T>
//inline constexpr bool is_base_parser_literal_v = is_base_parser_literal_wrapper<std::remove_cvref_t<T>>::value;
//
//
//// parser
//template<typename T>
//concept ConceptBaseParser = requires(T&&t)
//{
//	typename T::parsing_attribute;
//	requires std::is_copy_constructible_v<T>;
//	requires std::is_move_constructible_v<T>;
//	requires is_base_parser_v<T> || is_base_parser_literal_v<T>;
//};
//
//
//template<typename T, typename CharType>
//concept ConceptHasParseMethod = requires(T&&t, const CharType*&ptr_ref_to_str/*, _char_lit<char> parser_skip{'.'}*/)
//{
//	ConceptCharType<CharType>;
//	{t.Parse(ptr_ref_to_str, ptr_ref_to_str)};
//	{t.Scan(ptr_ref_to_str, ptr_ref_to_str)};
//	//{t.Parse(ptr_ref_to_str, ptr_ref_to_str, parser_skip)};
//};
//
//template<typename T>
//concept ConceptSkipperParser = requires(T&&t)
//{
//	requires ConceptBaseParser<T> || is_omited_v<T> || is_alternative_v<T>;
//	//std::is_same_v<typename T::parsing_attribute, tag_attribute_unused>;
//};
//
//template<typename T>
//concept ConceptCommonParser = requires(T&&t)
//{
//	requires ConceptBaseParser<T> || is_sequential_v<T> || is_repeate_v<T> || is_omited_v<T> || is_alternative_v<T> || is_list_v<T>;
//};
//
//template<typename T>
//struct is_common_parser : public std::false_type{};
//
//template<ConceptCommonParser T>
//struct is_common_parser<T> : public std::false_type{};
//
//template<typename T>
//constexpr bool is_common_parser_v = is_common_parser<T>::value;

}