#pragma once

#include <limits>

#include "common.h"
#include "Constants.h"
#include "ScanningFunctions.h"

namespace Scanners
{

template<ConceptCharType CharType>
class InterfaceScanner
{
public:
	using internal_char_type = CharType;
	static constexpr int CONST_MAX_CHARS_SEQUENCE = std::numeric_limits<int>::max();

protected:
	int number_of_scanned_chars = 0;

public:
	constexpr InterfaceScanner() = default;

	int GetNumberOfScannedChars()
	{
		return number_of_scanned_chars;
	}
};

template<ConceptCharType CharType>
class ScannerInteger : public InterfaceScanner<CharType>
{
public:
	using TInterface = InterfaceScanner<CharType>;
	using internal_char_type = TInterface::internal_char_type;

public:
	constexpr ScannerInteger() = default;
	bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
	{
		return (TInterface::number_of_scanned_chars = ScanStringForInteger(ptr_to_string, ptr_to_string_end)) > 0;
	}
};

template<ConceptCharType CharType>
class ScannerFloat : public InterfaceScanner<CharType>
{
public:
	using TInterface = InterfaceScanner<CharType>;
	using internal_char_type = TInterface::internal_char_type;

public:
	constexpr ScannerFloat() = default;
	bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
	{
		return (TInterface::number_of_scanned_chars = ScanStringForFloat(ptr_to_string, ptr_to_string_end)) > 0;
	}
};

template<ConceptCharType CharType>
class ScannerStringLiteral : public InterfaceScanner<CharType>
{
public:
	using TInterface = InterfaceScanner<CharType>;
	using internal_char_type = TInterface::internal_char_type;

protected:
	std::basic_string_view<CharType> literal;

public:
	constexpr ScannerStringLiteral(std::basic_string_view<CharType> strview)
		: literal(strview)
	{
	}

	bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
	{
		bool parsed = ScanStringForLiteralString(ptr_to_string, ptr_to_string_end, literal.data());
		TInterface::number_of_scanned_chars = parsed ? literal.length() : 0;

		return parsed;
	}

	constexpr std::basic_string_view<CharType> GetLiteral() noexcept
	{
		return literal;
	}
};

template<ConceptCharType CharType>
class ScannerSymbolLiteral : public InterfaceScanner<CharType>
{
public:
	using TInterface = InterfaceScanner<CharType>;
	using internal_char_type = TInterface::internal_char_type;

protected:
	CharType symbol;

public:
	constexpr ScannerSymbolLiteral(CharType sym)
		: symbol(sym)
	{
	}

	bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
	{
		return (TInterface::number_of_scanned_chars = ScanStringForSymbol(ptr_to_string, ptr_to_string_end, symbol)) > 0;
	}
};


template<ConceptCharType CharType>
class ScannerString : public InterfaceScanner<CharType>
{
public:
	using TInterface = InterfaceScanner<CharType>;
	using internal_char_type = TInterface::internal_char_type;

public:
	constexpr ScannerString() = default;

	bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
	{
		return (TInterface::number_of_scanned_chars = ScanStringForString(ptr_to_string, ptr_to_string_end)) > 0;
	}
};

template<ConceptCharType CharType>
class ScannerChar : public InterfaceScanner<CharType>
{
public:
	using TInterface = InterfaceScanner<CharType>;
	using internal_char_type = TInterface::internal_char_type;

public:
	constexpr ScannerChar() = default;

	bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
	{
		return (TInterface::number_of_scanned_chars = ScanStringForString(ptr_to_string, ptr_to_string_end)) > 0;
	}
};

template<ConceptCharType CharType>
class ScannerStringAlnum : public InterfaceScanner<CharType>
{
public:
	using TInterface = InterfaceScanner<CharType>;
	using internal_char_type = TInterface::internal_char_type;

public:
	constexpr ScannerStringAlnum() = default;

	bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
	{
		return (TInterface::number_of_scanned_chars = ScanStringForStringAlnum(ptr_to_string, ptr_to_string_end)) > 0;
	}
};


template<ConceptCharType CharType>
class ScannerSingleSymbol : public InterfaceScanner<CharType>
{
public:
	using TInterface = InterfaceScanner<CharType>;
	using internal_char_type = TInterface::internal_char_type;
	CharType symbol_to_find;

public:
	constexpr ScannerSingleSymbol(CharType sym)
		: symbol_to_find{sym}
	{
	};

	bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
	{
		TInterface::number_of_scanned_chars = ScanStringForSymbol(ptr_to_string, ptr_to_string_end, symbol_to_find);
		return TInterface::number_of_scanned_chars > 0;
	}
};

}