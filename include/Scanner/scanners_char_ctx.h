#pragma once

#include <type_traits>

#include "common.h"

#define CHAR_SCANNER(name, std_func_char, std_func_wchar)\
class CScanner_##name##_Char																								\
{																																						\
public:																																			\
	constexpr CScanner_##name##_Char() noexcept = default;									\
																																						\
	template<ConceptCharType CharType, typename TContext>											\
	consteval static auto GetType()																						\
	{																																					\
		if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())						\
			return tag_attribute_unused{};																				\
		else if constexpr(std::is_same_v<CharType, char> || std::is_same_v<CharType, wchar_t>)\
{																																						\
			static_assert(\
					std::is_same_v<std::invoke_result_t<decltype(&std_func_char), int>, std::invoke_result_t<decltype(&std_func_wchar), int> >\
					, "NUMERICAL_SCANNER :: char and wchar functions has different types"\
			);																																					\
			return std::invoke_result_t<decltype(&std_func_char), int>{};	\
		}																																				\
		else																																		\
			static_assert(false, "CScannerFloat::Parse() : forbidden char type");	\
	}																																					\
																																						\
	template<ConceptCharType CharType, typename TContext>											\
	bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetType<CharType, TContext>())> &_val)\
	{																																					\
		CharType *ptr_end;																											\
																																						\
		if constexpr(std::is_same_v<CharType, char>)														\
		{																																				\
			if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())					\
			{																																			\
				return std_func_char(*ptr_string) != 0;															\
			}																																			\
			else																																	\
			{																																			\
				const bool equal = std_func_char(*ptr_string);											\
				if(equal)																														\
					_val = *ptr_string++;																							\
																																						\
				return equal;																												\
			}																																			\
		}																																				\
		else if constexpr(std::is_same_v<CharType, wchar_t>)										\
		{																																				\
			if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())					\
			{																																			\
				return std_func_wchar(*ptr_string) != 0;														\
			}																																			\
			else																																	\
			{																																			\
				const bool equal = std_func_wchar(*ptr_string);											\
				if(equal)																														\
					_val = *ptr_string++;																							\
																																						\
				return equal;																												\
			}																																			\
		}																																				\
		else																																		\
			static_assert(false, "CScannerFloat::Parse() : forbidden char type");	\
	}																																					\
};																																					\
using name = CScanner_##name##_Char;

CHAR_SCANNER(char_digit, std::isdigit, std::iswdigit);