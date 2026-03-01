#pragma once

#include <cctype>
#include <cwctype>
#include <charconv>

#include "common.h"
#include "ConstexprFunctions.h"


template<ConceptCharType CharType>
int ScanStringForInteger(const CharType *ptr_to_string, const CharType *ptr_to_string_end);



template<>
int ScanStringForInteger<char>(const char *const ptr_to_string, const char *const ptr_to_string_end)
{
	auto ptr_temp = ptr_to_string;
	if(*ptr_temp == '-')
		ptr_temp++;

	while(ptr_temp < ptr_to_string_end 
				&& std::isdigit(*ptr_temp))
	{
		ptr_temp++;
	}

	return (ptr_temp - ptr_to_string);
}

template<>
int ScanStringForInteger<wchar_t>(const wchar_t *const ptr_to_string, const wchar_t *const ptr_to_string_end)
{
	if(*ptr_to_string == L'0')
		return 0;

	auto ptr_temp = ptr_to_string;

	if(*ptr_temp == L'-')
		ptr_temp++;

	while(ptr_temp < ptr_to_string_end 
				&& std::iswdigit(*ptr_temp))
	{
		ptr_temp++;
	}

	return (ptr_temp - ptr_to_string);
}


template<ConceptCharType CharType>
int ScanStringForFloat(const CharType* ptr_to_string, const CharType* ptr_to_string_end)
{
	if constexpr ( std::is_same_v<CharType, char>)
	{
		char* ptr_end = nullptr;
		std::strtof(ptr_to_string, &ptr_end);

		return ptr_end - ptr_to_string;
	}
	else
	{
		wchar_t* ptr_end = nullptr;
		std::wcstof(ptr_to_string, &ptr_end);

		return ptr_end - ptr_to_string;
	}
}


template<ConceptCharType CharType>
constexpr bool ScanStringForLiteralString(const CharType* ptr_to_string, const CharType* ptr_to_string_end, const CharType* ptr_string_to_find)
{
	if constexpr (std::is_same_v<CharType, char>)
	{
		return strncmp(ptr_to_string, ptr_string_to_find, strlen(ptr_string_to_find)) == 0;
	}
	else if constexpr (std::is_same_v<CharType, wchar_t>)
	{
		return wcsncmp(ptr_to_string, ptr_string_to_find, wcslen(ptr_string_to_find)) == 0;
	}
}


template<>
bool ScanStringForLiteralString<char>(const char *ptr_to_string, const char *ptr_to_string_end, const char *ptr_string_to_find)
{
	return strncmp(ptr_to_string, ptr_string_to_find, strlen(ptr_string_to_find)) == 0;
}

template<>
bool ScanStringForLiteralString<wchar_t>(const wchar_t *ptr_to_string, const wchar_t *ptr_to_string_end, const wchar_t *ptr_string_to_find)
{
	return wcsncmp(ptr_to_string, ptr_string_to_find, wcslen(ptr_string_to_find)) == 0;
}

template<ConceptCharType CharType>
constexpr int ScanStringForString(const CharType *ptr_to_string, const CharType *ptr_to_string_end);

template<>
constexpr int ScanStringForString<char>(const char * ptr_to_string, const char *ptr_to_string_end)
{
	if constexpr (std::is_constant_evaluated())
	{
		auto ptr_temp = ptr_to_string;
		while (ptr_temp < ptr_to_string_end
			&& ConstexprFunctions::IsAlpha(ptr_temp))
			ptr_temp++;

		return (ptr_temp - ptr_to_string);
	}

	auto ptr_temp = ptr_to_string;
	while(ptr_temp < ptr_to_string_end
				&& std::isalpha(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<>
constexpr int ScanStringForString<wchar_t>(const wchar_t *ptr_to_string, const wchar_t *ptr_to_string_end)
{
	if constexpr (std::is_constant_evaluated())
	{
		auto ptr_temp = ptr_to_string;
		while (ptr_temp < ptr_to_string_end
			&& ConstexprFunctions::IsAlpha(ptr_temp))
			ptr_temp++;

		return (ptr_temp - ptr_to_string);
	}

	auto ptr_temp = ptr_to_string;
	while(ptr_temp < ptr_to_string_end
				&& std::iswalpha(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<ConceptCharType CharType>
constexpr int ScanStringPrintableChars(const CharType* ptr_to_string, const CharType* ptr_to_string_end);

template<>
constexpr int ScanStringPrintableChars<char>(const char* ptr_to_string, const char* ptr_to_string_end)
{
	auto ptr_temp = ptr_to_string;
	while (ptr_temp < ptr_to_string_end
		&& std::isprint(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<>
constexpr int ScanStringPrintableChars<wchar_t>(const wchar_t* ptr_to_string, const wchar_t* ptr_to_string_end)
{
	auto ptr_temp = ptr_to_string;
	while (ptr_temp < ptr_to_string_end
		&& std::iswprint(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<ConceptCharType CharType>
constexpr int ScanStringPrintableCharSymbol(const CharType* ptr_to_string, const CharType* ptr_to_string_end);

template<>
constexpr int ScanStringPrintableCharSymbol<char>(const char* ptr_to_string, const char* ptr_to_string_end)
{
	return std::isprint(*ptr_to_string) ? 1 : 0;
}

template<>
constexpr int ScanStringPrintableCharSymbol<wchar_t>(const wchar_t* ptr_to_string, const wchar_t* ptr_to_string_end)
{
	return std::iswprint(*ptr_to_string) ? 1 : 0;
}

template<ConceptCharType CharType>
constexpr int ScanStringForStringAlnum(const CharType *ptr_to_string, const CharType *ptr_to_string_end);

template<>
constexpr int ScanStringForStringAlnum<char>(const char *ptr_to_string, const char *ptr_to_string_end)
{
	if constexpr (std::is_constant_evaluated())
	{
		auto ptr_temp = ptr_to_string;
		while (ptr_temp < ptr_to_string_end
			&& ConstexprFunctions::IsAlnum(ptr_temp))
			ptr_temp++;

		return (ptr_temp - ptr_to_string);
	}

	auto ptr_temp = ptr_to_string;
	while (ptr_temp < ptr_to_string_end
		&& std::isalnum(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<>
constexpr int ScanStringForStringAlnum<wchar_t>(const wchar_t *ptr_to_string, const wchar_t *ptr_to_string_end)
{
	if constexpr (std::is_constant_evaluated())
	{
		auto ptr_temp = ptr_to_string;
		while (ptr_temp < ptr_to_string_end
			&& ConstexprFunctions::IsAlnum(ptr_temp))
			ptr_temp++;

		return (ptr_temp - ptr_to_string);
	}

	auto ptr_temp = ptr_to_string;
	while(ptr_temp < ptr_to_string_end
				&& std::iswalnum(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<ConceptCharType CharType>
int ScanStringForSymbol(const CharType *ptr_to_string, const CharType *ptr_to_string_end, CharType symbol)
{
	return (ptr_to_string != ptr_to_string_end && *ptr_to_string == symbol) ? 1 : 0;
}
