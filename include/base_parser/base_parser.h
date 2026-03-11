#pragma once

#include "Scanners.h"
#include "parser_with_action.h"

template<typename TScanner>
class ParserWithContext
{
public:
	constexpr ParserWithContext() noexcept {};
	constexpr ParserWithContext(const ParserWithContext& other) noexcept = default;
	constexpr ParserWithContext(ParserWithContext&& other) noexcept = default;

	template<ConceptCharType CharType, ConceptContext TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
		, constCharPtrRef<CharType> ptr_string_end
		, TContext&& context
		, traits::attribute<TScanner, CharType, std::remove_cvref_t<TContext>>::type& attribute) const
	{
		context.UseSkipper(ptr_string, ptr_string_end);
		return TScanner{}.ParseFunction(ptr_string, ptr_string_end, context, attribute);
	}

	template<ConceptCharType CharType, ConceptContext TContext>
	constexpr static auto GetReturnType()
	{
		return traits::attribute_t<TScanner, CharType, std::remove_cvref_t<TContext>>{};
	}

	auto operator[](auto callable) const
	{
		return ParserWithActionNew(*this, callable);
	}
};


template<template<typename> class TScanner, ConceptCharType CharType>
class ParserLiteralWithContext
{
	using scanner_type = TScanner<CharType>;

protected:
	scanner_type m_scanner;

public:
	constexpr ParserLiteralWithContext(const ParserLiteralWithContext<TScanner, CharType>& other) noexcept = default;
	constexpr ParserLiteralWithContext(ParserLiteralWithContext<TScanner, CharType>&& other) noexcept = default;

	constexpr ParserLiteralWithContext(std::basic_string_view<CharType> literal) noexcept
		: m_scanner{ literal }
	{}

	constexpr ParserLiteralWithContext(constCharPtr<CharType>* literal) noexcept
		: m_scanner{literal}
	{}

	constexpr ParserLiteralWithContext(CharType symbol) noexcept
		: m_scanner{symbol}
	{}

	template<ConceptCharType CharType, ConceptContext TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
		, constCharPtrRef<CharType> ptr_string_end
		, TContext&& context
		, traits::attribute<scanner_type, CharType, std::remove_cvref_t<TContext>>::type& attribute) const
	{
		context.UseSkipper(ptr_string, ptr_string_end);
		return m_scanner.ParseFunction(ptr_string, ptr_string_end, context, attribute);
	}

	template<ConceptCharType CharType, ConceptContext TContext>
	constexpr static auto GetReturnType()
	{
		return traits::attribute_t<TScanner<CharType>, CharType, TContext>{};
	}

	auto operator[](auto callable) const
	{
		return ParserWithActionNew(*this, callable);
	}
};

template <ConceptCharType CharType>
ParserLiteralWithContext(std::basic_string_view<CharType> literal) -> ParserLiteralWithContext<Scanners::CScannerStringRaw, CharType>;

template <ConceptCharType CharType>
ParserLiteralWithContext(const CharType* literal) -> ParserLiteralWithContext<Scanners::CScannerStringRaw, CharType>;

template <ConceptCharType CharType>
ParserLiteralWithContext(CharType symbol) -> ParserLiteralWithContext<Scanners::CScannerCharRaw, CharType>;

template<template<typename> class TScanner, ConceptCharType CharType>
ParserLiteralWithContext(const ParserLiteralWithContext<TScanner, CharType>& other) -> ParserLiteralWithContext<TScanner, CharType>;

template<template<typename> class TScanner, ConceptCharType CharType>
ParserLiteralWithContext(ParserLiteralWithContext<TScanner, CharType>&& other) -> ParserLiteralWithContext<TScanner, CharType>;


namespace aliases
{
	constexpr auto int_					= ParserWithContext<Scanners::scanner_int_cts>{};
	constexpr auto uint_				= ParserWithContext<Scanners::scanner_uint_cts>{};
	constexpr auto long_int_		= ParserWithContext<Scanners::scanner_long_int_cts>{};
	constexpr auto long_uint_		= ParserWithContext<Scanners::scanner_long_uint_cts>{};

	constexpr auto float_				= ParserWithContext<Scanners::scanner_float_ctx>{};
	constexpr auto double_			= ParserWithContext<Scanners::scanner_double_ctx>{};
	constexpr auto long_double_	= ParserWithContext<Scanners::scanner_long_double_ctx>{};

	constexpr auto str_alpha = ParserWithContext<Scanners::_isalpha_str>{};
	constexpr auto str_alnum = ParserWithContext<Scanners::_isalnum_str>{};
	constexpr auto str_digit = ParserWithContext<Scanners::_isdigit_str>{};
	constexpr auto str_blank = ParserWithContext<Scanners::_isblank_str>{};
	constexpr auto str_cntrl = ParserWithContext<Scanners::_iscntrl_str>{};
	constexpr auto str_graph = ParserWithContext<Scanners::_isgraph_str>{};
	constexpr auto str_print = ParserWithContext<Scanners::_isprint_str>{};
	constexpr auto str_punct = ParserWithContext<Scanners::_ispunct_str>{};
	constexpr auto str_space = ParserWithContext<Scanners::_isspace_str>{};
	constexpr auto str_xdigit = ParserWithContext<Scanners::_isxdigit_str>{};
	constexpr auto str_lower = ParserWithContext<Scanners::_islower_str>{};
	constexpr auto str_upper = ParserWithContext<Scanners::_isupper_str>{};

	constexpr auto char_alpha = ParserWithContext<Scanners::_isalpha>{};
	constexpr auto char_alnum = ParserWithContext<Scanners::_isalnum>{};
	constexpr auto char_digit = ParserWithContext<Scanners::_isdigit>{};
	constexpr auto char_blank = ParserWithContext<Scanners::_isblank>{};
	constexpr auto char_cntrl = ParserWithContext<Scanners::_iscntrl>{};
	constexpr auto char_graph = ParserWithContext<Scanners::_isgraph>{};
	constexpr auto char_print = ParserWithContext<Scanners::_isprint>{};
	constexpr auto char_punct = ParserWithContext<Scanners::_ispunct>{};
	constexpr auto char_space = ParserWithContext<Scanners::_isspace>{};
	constexpr auto char_xdigit = ParserWithContext<Scanners::_isxdigit>{};
	constexpr auto char_lower = ParserWithContext<Scanners::_islower>{};
	constexpr auto char_upper = ParserWithContext<Scanners::_isupper>{};
	constexpr auto char_any = ParserWithContext<CScannerСharAny>{};

	constexpr auto space_		= ParserLiteralWithContext{' '};
	constexpr auto newline_	= ParserLiteralWithContext{'\n'};
	constexpr auto tab_			= ParserLiteralWithContext{'\t'};
}