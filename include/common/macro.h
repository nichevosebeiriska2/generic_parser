#pragma  once

#include <string>
// MACRO

// You have to copy 'impl' parser to separate its result from ones upward in recursive call stack!
// ImplementParsingRule links two objects : declaration and implementation (as a 'declaration' type not a specific object). 
// Its literally implement Parse()/Scan() functions of 'declaration' parser type
// With no copy in Parser() methods each call of 'declaration.Parse()' will be called from one 'impl' parser object.
// But 'logical' lifetime of separate parser should be ended once it have Parse()/Scan() method called.
// Otherwise 'impl' as a object (not a type!) will end up participating in several iterations
// Every parser used as a 'impl' type has to have Copy() method

#define ImplementParsingRule(decl, impl)\
template<>\
template<ConceptCharType CharType, typename TParserSkipper>\
bool typename decltype(decl)::Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TParserSkipper& skipper)	\
{\
	auto copy = impl.Copy();\
	bool parsed = copy.Parse(ptr_string, ptr_string_end, skipper);\
	if (parsed) m_last_result = copy.GetValueAndReset(); \
	return parsed;\
}; \
template<>\
template<ConceptCharType CharType>\
bool typename decltype(decl)::Parse(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)	\
{\
	auto copy = impl.Copy();\
	bool parsed = copy.Parse(ptr_string, ptr_string_end);\
	if (parsed) m_last_result = copy.GetValueAndReset(); \
	return parsed;\
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
	return std::exchange(m_last_result, {});\
}\
\


// Macro for single symbol char parser. name - alias for parser, str_std_function_name_str/str_std_function_name_wstr internal functions for symbol validation 
// (std:: functions only used, but each function with bool(int/wint_t) convertable signature can be used)
#define  DEFINE_CHAR_FROM_STR_FUNCTION(name, str_std_function_name_str, str_std_function_name_wstr) \
class CScanner##_##name##_##Char												\
{																										\
	protected:																				\
	bool scanned = false;															\
																										\
	public:																						\
template<ConceptCharType CharType, typename TContext>									\
bool ParseFunction(constCharPtr<CharType> ptr_to_string, constCharPtr<CharType> ptr_to_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val)\
{																										\
	if(ptr_to_string == ptr_to_string_end)						\
	{																									\
		return scanned = false;													\
	}																									\
																										\
	if constexpr(std::is_same_v<CharType, char>)			\
	{																									\
		if(str_std_function_name_str(*ptr_to_string))		\
		{																								\
			return scanned = true;												\
		}																								\
		return false;																		\
	}																									\
	else if constexpr(std::is_same_v<CharType, wchar_t>)\
	{																									\
		if(str_std_function_name_str(*ptr_to_string))		\
		{																								\
			return scanned = true;												\
		}																								\
		return false;																		\
	}																									\
	else																							\
		static_assert(false, "forbidden char type");		\
																										\
																										\
}																										\
																										\
template<ConceptCharType CharType, typename TContext>\
constexpr static auto GetReturnType()\
{\
	return CharType{};\
}\
																										\
};																									\
using _##name = CScanner##_##name##_##Char;								

// same as DEFINE_CHAR_FROM_STR_FUNCTION but for multiple symbols scan at one call

#define  DEFINE_STRING_FROM_STR_FUNCTION(name, str_std_function_name_str, str_std_function_name_wstr) \
class CScanner##_##name##_##String									\
{																										\
	protected:																				\
	UINT scanned_symbols{0};													\
																										\
	public:																						\
																										\
template<ConceptCharType CharType, typename TContext>									\
bool ParseFunction(constCharPtr<CharType> ptr_to_string, constCharPtr<CharType> ptr_to_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val)\
{\
constCharPtr<CharType> ptr_temp = ptr_to_string;		\
																										\
	if constexpr(std::is_same_v<CharType, char>)			\
	{																									\
		while(ptr_temp < ptr_to_string_end && str_std_function_name_str(*ptr_temp))\
				ptr_temp++;																	\
		size_t scanned_symbols = (ptr_temp - ptr_to_string);								\
		if(scanned_symbols)\
			_val = std::string{ ptr_to_string, ptr_temp };\
		return scanned_symbols > 0;\
	}																									\
	else if constexpr(std::is_same_v<CharType, wchar_t>)\
	{																									\
		while(ptr_temp < ptr_to_string_end && str_std_function_name_wstr(*ptr_temp))\
				ptr_temp++;																	\
		size_t scanned_symbols = (ptr_temp - ptr_to_string);								\
		if(scanned_symbols)\
			_val = std::wstring{ ptr_to_string, ptr_temp };\
		return scanned_symbols > 0;\
	}																									\
	else																							\
		static_assert(false, "forbidden char type");		\
																										\
																										\
};																										\
template<ConceptCharType CharType, typename TContext>\
constexpr static auto GetReturnType()\
{\
	return std::basic_string<CharType>{};\
};\
};																									\
using _##name = CScanner##_##name##_##String;								

// \ MACRO
