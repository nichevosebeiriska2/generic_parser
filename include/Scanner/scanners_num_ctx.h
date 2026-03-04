#pragma once 

#include <type_traits>

#include "common.h"

#define INTEGER_SCANNER(name, std_func_char, std_func_wchar, base)\
class CScanner_##name##_Number																							\
{																																						\
public:																																			\
	constexpr CScanner_##name##_Number() noexcept = default;									\
																																						\
	template<ConceptCharType CharType, typename TContext>											\
	consteval static auto GetType()																						\
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
	bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetType<CharType, TContext>())> &_val)\
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

#define FLOATING_POINT_SCANNER(name, std_func_char, std_func_wchar)\
class CScanner_##name##_Number																							\
{																																						\
public:																																			\
	constexpr CScanner_##name##_Number() noexcept = default;									\
																																						\
	template<ConceptCharType CharType, typename TContext>											\
	consteval static auto GetType()																						\
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
	bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetType<CharType, TContext>())> &_val)\
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

FLOATING_POINT_SCANNER(scanner_float_ctx, std::strtof, std::wcstof);
FLOATING_POINT_SCANNER(scanner_double_ctx, std::strtod, std::wcstod);
FLOATING_POINT_SCANNER(scanner_long_double_ctx, std::strtold, std::wcstold);

INTEGER_SCANNER(scanner_int_cts, std::strtol, std::wcstol, 10);
INTEGER_SCANNER(scanner_uint_cts, std::strtoul, std::wcstoul, 10);
INTEGER_SCANNER(scanner_long_int_cts, std::strtoll, std::wcstoll, 10);
INTEGER_SCANNER(scanner_long_uint_cts, std::strtoull, std::wcstoull, 10);

class CScannerFloat
{
public:
	constexpr CScannerFloat() noexcept = default;

	template<ConceptCharType CharType, typename TContext>
	consteval static auto GetType()
	{
		if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())
			return tag_attribute_unused{};
		else if constexpr(std::is_same_v<CharType, char> || std::is_same_v<CharType, wchar_t>)
			return std::invoke_result_t<decltype(&std::strtof), const char *, char **>{};
		else
			static_assert(false, "CScannerFloat::Parse() : forbidden char type");
	}

	template<ConceptCharType CharType, typename TContext>
	bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetType<CharType, TContext>())> &_val)
	{
		CharType *ptr_end;

		if constexpr(std::is_same_v<CharType, char>)
		{
			if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())
			{
				std::strtof(ptr_string, &ptr_end);
				return ptr_end - ptr_string;
			}
			else
			{
				_val = std::strtof(ptr_string, &ptr_end);
				return ptr_end - std::exchange(ptr_string, ptr_end);
			}
		}
		else if constexpr(std::is_same_v<CharType, wchar_t>)
		{
			if constexpr(std::remove_cvref_t<TContext>::IsOmitedStatic())
			{
				std::wcstof(ptr_string, &ptr_end);
				return ptr_end - std::exchange(ptr_string, ptr_end);
			}
			else
			{
				_val = std::wcstof(ptr_string, &ptr_end);
				return ptr_end - std::exchange(ptr_string, ptr_end);
			}
		}
		else
			static_assert(false, "CScannerFloat::Parse() : forbidden char type");
	}

};