#pragma once

#include <locale>

#include "common.h"

// constexpr algorithms

namespace ConstexprFunctions
{
	template<ConceptCharType CharType>
	constexpr size_t RawStringLength(constCharPtr<CharType> ptr_string) // for null-terminated strings only!
	{
		if constexpr (std::is_same_v<CharType, char>)
		{
			if (ptr_string)
			{
				constCharPtr<CharType> ptr_string_temp = ptr_string;
				while (*ptr_string_temp)
					ptr_string_temp++;

				return ptr_string_temp - ptr_string;
			}
			else
				return false;
		}
		else
			static_assert("RawStringLength : forbiden char type");
	}

	template<ConceptCharType CharType>
	constexpr bool RawStringIsSubstrPrefix(constCharPtr<CharType> ptr_string, constCharPtr<CharType> ptr_substr)
	{
		const size_t substr_len = RawStringLength(ptr_substr);

		if (RawStringLength(ptr_string) < substr_len)
			return false;

		for (int i = 0; i < substr_len; i++)
			if (*(ptr_substr + i) != *(ptr_string + i))
				return false;

		return true;

	}

	template<ConceptCharType CharType>
	constexpr bool IsAlpha(constCharPtr<CharType> ptr_symbol) // ASCII only!
	{
		if constexpr (std::is_same_v<CharType, char>)
		{
			CharType symbol = *ptr_symbol;
			return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
		}
		else if constexpr (std::is_same_v<CharType, wchar_t>)
		{
			CharType symbol = *ptr_symbol;
			return (symbol >= L'a' && symbol <= L'z') || (symbol >= L'A' && symbol <= L'Z');
		}
		else
			static_assert(false, "IsAlpha : forbidden char type");
	}

	template<ConceptCharType CharType>
	constexpr bool IsAlnum(constCharPtr<CharType> ptr_symbol) // ASCII only!
	{
		CharType symbol = *ptr_symbol;

		if constexpr (std::is_same_v<CharType, char>)
			return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z') || (symbol >= '0' && symbol <= '9');
		
		else if constexpr (std::is_same_v<CharType, wchar_t>)
			return (symbol >= L'a' && symbol <= L'z') || (symbol >= L'A' && symbol <= L'Z') || (symbol >= L'0' && symbol <= L'9');
		
		else
			static_assert(false, "IsAlpha : forbidden char type");
	}
}

// \constexpr algorithms