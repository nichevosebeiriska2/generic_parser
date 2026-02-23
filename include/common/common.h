#pragma once

#include <type_traits>

using UINT = uint32_t;
using INT  = int32_t;
// concepts
template<typename T>
concept ConceptCharType = std::is_same_v<T, char> || std::is_same_v<T, wchar_t>;

template<ConceptCharType CharType>
using constCharPtrRef = const CharType*&; // use it to declare const ptr reference to underlying char type in template Parse()/Scan() functions. 

template<typename T, typename CharType>
concept ConceptHasParseFunction = requires(T && t, const CharType* ptr)
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
	typename std::remove_cvref_t<T>::parsing_attribute; 

	requires std::is_move_constructible_v<T>;
	requires std::is_copy_constructible_v<T>;

	//{ t.IsOmited() } -> std::same_as<bool>;
	//requires ConceptHasParseFunction<T, char> || ConceptHasParseFunction<T, wchar_t>;
	//requires ConceptHasScanFunction<T, char> || ConceptHasScanFunction<T, wchar_t>;
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

template<typename T>
struct is_parser : std::false_type {};

template<ConceptParser ParserType>
struct is_parser<ParserType> : std::true_type {};

template<typename T>
constexpr bool is_parser_v = is_parser<T>::value;

// for ParserWithAction ActionType type 
template<typename T>
struct action_traits;

template<typename ClassType, typename ReturnType, typename... Args>
struct action_traits<ReturnType(ClassType::*)(Args...) const> {
	using return_type = ReturnType;

	static constexpr std::size_t num_of_arguments = sizeof...(Args);
	static constexpr bool has_arguments = num_of_arguments != 0;

	static_assert(num_of_arguments <= 1, "action_traits :: action must have 0 or 1 arguments");

	template<std::size_t N>
	struct argument {
		static_assert(N < num_of_arguments, "Invalid argument index");
		using type = std::tuple_element_t<N, std::tuple<Args...>>;
	};
};

template<typename Lambda>
using lambda_traits = action_traits<decltype(&Lambda::operator())>;

// \metafunctions 

// MACRO

#define ImplementParsingRule(decl, impl)\
template<>\
template<ConceptCharType CharType, typename TParserSkipper>\
bool typename decltype(decl)::Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TParserSkipper& skipper)	\
{\
	return impl.Parse(ptr_string, ptr_string_end, skipper);\
}; \
template<>\
template<ConceptCharType CharType>\
bool typename decltype(decl)::Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)	\
{\
	return impl.Parse(ptr_string, ptr_string_end);\
}; \
template<>\
template<ConceptCharType CharType, typename TParserSkipper>\
bool typename decltype(decl)::Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TParserSkipper& skipper)	\
{\
	return impl.Scan(ptr_string, ptr_string_end, skipper);\
}; \
template<>\
template<ConceptCharType CharType>\
bool typename decltype(decl)::Scan(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)	\
{\
	return impl.Scan(ptr_string, ptr_string_end);\
}; \
template<>\
decltype(decl)::parsing_attribute typename decltype(decl)::GetValueAndReset(){\
	return impl.GetValueAndReset();\
}\
\



// \ MACRO


