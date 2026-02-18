#pragma once

#include <cctype>
#include <cwctype>
#include <charconv>

#include "Concepts.h"


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
int ScanStringForFloat(const CharType *ptr_to_string, const CharType *ptr_to_string_end);

template<>
int ScanStringForFloat<char>(const char *const ptr_to_string, const char *const ptr_to_string_end)
{
	if(ptr_to_string >= ptr_to_string_end || *ptr_to_string == '.')
		return 0;

	auto ptr_temp = ptr_to_string;

	if(*ptr_temp == '-')
		ptr_temp++;

	int number_of_dots = 0;
	bool previous_symbols_is_dot = false;
	while(ptr_temp < ptr_to_string_end 
				&& (std::isdigit(*ptr_temp) || ((*ptr_temp == '.') && number_of_dots < 1))
				&& number_of_dots < 2)
	{
		previous_symbols_is_dot = (*(ptr_temp) == '.');
		number_of_dots += previous_symbols_is_dot;
		ptr_temp += number_of_dots < 2;
	}

	return (ptr_temp - ptr_to_string) - (int)previous_symbols_is_dot;
}

template<>
int ScanStringForFloat<wchar_t>(const wchar_t *const ptr_to_string, const wchar_t *const ptr_to_string_end)
{
	if(ptr_to_string >= ptr_to_string_end || *ptr_to_string == L'.')
		return 0;

	auto ptr_temp = ptr_to_string;
	int number_of_dots = 0;

	while(ptr_temp < ptr_to_string_end
				&& (std::iswdigit(*ptr_temp) || ((ptr_temp > ptr_to_string) && (*ptr_temp == L'.')))
				&& number_of_dots < 2)
	{
		number_of_dots += (*ptr_temp == '.');
		ptr_temp += number_of_dots < 2;
	}

	return (ptr_temp - ptr_to_string);
}

template<ConceptCharType CharType>
bool ScanStringForLiteralString(const CharType *ptr_to_string, const CharType *ptr_to_string_end, const CharType *ptr_string_to_find);


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
int ScanStringForString(const CharType *ptr_to_string, const CharType *ptr_to_string_end);

template<>
int ScanStringForString<char>(const char * ptr_to_string, const char *ptr_to_string_end)
{
	auto ptr_temp = ptr_to_string;
	while(ptr_temp < ptr_to_string_end
				&& std::isalpha(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<>
int ScanStringForString<wchar_t>(const wchar_t *ptr_to_string, const wchar_t *ptr_to_string_end)
{
	auto ptr_temp = ptr_to_string;
	while(ptr_temp < ptr_to_string_end
				&& std::iswalpha(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<ConceptCharType CharType>
int ScanStringForStringAlnum(const CharType *ptr_to_string, const CharType *ptr_to_string_end);

template<>
int ScanStringForStringAlnum<char>(const char *ptr_to_string, const char *ptr_to_string_end)
{
	auto ptr_temp = ptr_to_string;
	while(ptr_temp < ptr_to_string_end
				&& std::isalnum(*ptr_temp))
		ptr_temp++;

	return (ptr_temp - ptr_to_string);
}

template<>
int ScanStringForStringAlnum<wchar_t>(const wchar_t *ptr_to_string, const wchar_t *ptr_to_string_end)
{
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

