#pragma  once

#include <string>
// MACRO

// Macro for single symbol char parser. name - alias for parser, str_std_function_name_str/str_std_function_name_wstr internal functions for symbol validation 
// (std:: functions only used, but each function with bool(int/wint_t) convertable signature can be used)
#define  DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(name, str_std_function_name_str, str_std_function_name_wstr) \
class CScanner##_##name##_##Char												\
{																										\
	protected:																				\
	bool scanned = false;															\
																										\
	public:																						\
template<ConceptCharType CharType, typename TContext>									\
bool ParseFunction(constCharPtrRef<CharType> ptr_to_string, constCharPtrRef<CharType> ptr_to_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val)\
{																										\
	if(ptr_to_string == ptr_to_string_end)						\
	{																									\
		return scanned = false;													\
	}																									\
																										\
	if constexpr(std::is_same_v<CharType, char>)			\
	{																									\
		if(str_std_function_name_str((unsigned char)(*ptr_to_string)))		\
		{																								\
			scanned = true;												\
			if(scanned){\
				_val = *ptr_to_string;\
				ptr_to_string ++;\
			}\
			return scanned;\
		}																								\
		return false;																		\
	}																									\
	else if constexpr(std::is_same_v<CharType, wchar_t>)\
	{																									\
		if(str_std_function_name_wstr(*ptr_to_string))		\
		{																								\
			scanned = true;												\
			if(scanned){\
				_val = *ptr_to_string;\
				ptr_to_string ++;\
			}\
			return scanned;\
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

#define  DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(name, str_std_function_name_str, str_std_function_name_wstr) \
class CScanner##_##name##_##String									\
{																										\
	protected:																				\
	UINT scanned_symbols{0};													\
																										\
	public:																						\
																										\
template<ConceptCharType CharType, typename TContext>									\
bool ParseFunction(constCharPtrRef<CharType> ptr_to_string, constCharPtrRef<CharType> ptr_to_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val)\
{\
constCharPtr<CharType> ptr_temp = ptr_to_string;		\
																										\
	if constexpr(std::is_same_v<CharType, char>)			\
	{																									\
		while(ptr_temp < ptr_to_string_end && str_std_function_name_str((unsigned char)(*ptr_temp)))\
				ptr_temp++;																	\
		size_t scanned_symbols = (ptr_temp - ptr_to_string);								\
		if(scanned_symbols){\
			_val = std::string{ ptr_to_string, ptr_temp };\
			ptr_to_string += scanned_symbols;\
		}\
		return scanned_symbols > 0;\
	}																									\
	else if constexpr(std::is_same_v<CharType, wchar_t>)\
	{																									\
		while(ptr_temp < ptr_to_string_end && str_std_function_name_wstr(*ptr_temp))\
				ptr_temp++;																	\
		size_t scanned_symbols = (ptr_temp - ptr_to_string);								\
		if(scanned_symbols){\
			_val = std::wstring{ ptr_to_string, ptr_temp };\
			ptr_to_string += scanned_symbols;\
		}\
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

#define DEFINE_INTEGER_PARSER_WITH_STD_FUNCTIONS(name, std_func_char, std_func_wchar, base)\
class CScanner_##name##_Number																							\
{																																						\
public:																																			\
	constexpr CScanner_##name##_Number() noexcept = default;									\
																																						\
	template<ConceptCharType CharType, typename TContext>											\
	constexpr static auto GetReturnType()																						\
	{																																					\
		if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())						\
			return tag_attribute_unused{};																				\
		else if constexpr(std::is_same_v<CharType, char> || std::is_same_v<CharType, wchar_t>)\
{																																						\
			static_assert(\
					std::is_same_v<std::invoke_result_t<decltype(&std_func_char), const char *, char **, int>, std::invoke_result_t<decltype(&std_func_wchar), const wchar_t *, wchar_t **, int> >\
					, "NUMERICAL_SCANNER :: char and wchar functions has different types"\
			);																																					\
			return std::invoke_result_t<decltype(&std_func_char), const char *, char **, int>{};	\
		}																																				\
		else																																		\
			static_assert(false, "CScannerFloat::Parse() : forbidden char type");	\
	}																																					\
																																						\
	template<ConceptCharType CharType, typename TContext>											\
	bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val) const\
	{																																					\
		CharType *ptr_end;																											\
																																						\
		if constexpr(std::is_same_v<CharType, char>)														\
		{																																				\
			if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())					\
			{																																			\
				std_func_char(ptr_string, &ptr_end, base);													\
				return ptr_end - ptr_string;																				\
			}																																			\
			else																																	\
			{																																			\
				_val = std_func_char(ptr_string, &ptr_end, base);										\
				return ptr_end - std::exchange(ptr_string, ptr_end);								\
			}																																			\
		}																																				\
		else if constexpr(std::is_same_v<CharType, wchar_t>)										\
		{																																				\
			if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())					\
			{																																			\
				std_func_wchar(ptr_string, &ptr_end, base);													\
				return ptr_end - std::exchange(ptr_string, ptr_end);								\
			}																																			\
			else																																	\
			{																																			\
				_val = std_func_wchar(ptr_string, &ptr_end, base);									\
				return ptr_end - std::exchange(ptr_string, ptr_end);								\
			}																																			\
		}																																				\
		else																																		\
			static_assert(false, "CScannerFloat::Parse() : forbidden char type");	\
	}																																					\
};																																					\
using name = CScanner_##name##_Number;

#define DEFINE_FLOAT_PARSER_WITH_STD_FUNCTIONS(name, std_func_char, std_func_wchar)\
class CScanner_##name##_Number																							\
{																																						\
public:																																			\
	constexpr CScanner_##name##_Number() noexcept = default;									\
																																						\
	template<ConceptCharType CharType, typename TContext>											\
	constexpr static auto GetReturnType()																						\
	{																																					\
		if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())						\
			return tag_attribute_unused{};																				\
		else if constexpr(std::is_same_v<CharType, char> || std::is_same_v<CharType, wchar_t>)\
{																																						\
			static_assert(\
					std::is_same_v<std::invoke_result_t<decltype(&std_func_char), const char *, char **>, std::invoke_result_t<decltype(&std_func_wchar), const wchar_t *, wchar_t **> >\
					, "NUMERICAL_SCANNER :: char and wchar functions has different types"\
			);																																					\
			return std::invoke_result_t<decltype(&std_func_char), const char *, char **>{};	\
		}																																				\
		else																																		\
			static_assert(false, "CScannerFloat::Parse() : forbidden char type");	\
	}																																					\
																																						\
	template<ConceptCharType CharType, typename TContext>											\
	bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val) const\
	{																																					\
		CharType *ptr_end;																											\
																																						\
		if constexpr(std::is_same_v<CharType, char>)														\
		{																																				\
			if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())					\
			{																																			\
				std_func_char(ptr_string, &ptr_end);																\
				return ptr_end - ptr_string;																				\
			}																																			\
			else																																	\
			{																																			\
				_val = std_func_char(ptr_string, &ptr_end);													\
				return ptr_end - std::exchange(ptr_string, ptr_end);								\
			}																																			\
		}																																				\
		else if constexpr(std::is_same_v<CharType, wchar_t>)										\
		{																																				\
			if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())					\
			{																																			\
				std_func_wchar(ptr_string, &ptr_end);																\
				return ptr_end - std::exchange(ptr_string, ptr_end);								\
			}																																			\
			else																																	\
			{																																			\
				_val = std_func_wchar(ptr_string, &ptr_end);												\
				return ptr_end - std::exchange(ptr_string, ptr_end);								\
			}																																			\
		}																																				\
		else																																		\
			static_assert(false, "CScannerFloat::Parse() : forbidden char type");	\
	}																																					\
};																																					\
using name = CScanner_##name##_Number;

// \ MACRO
