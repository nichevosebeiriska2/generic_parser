#pragma once

#include <limits>

#include "common.h"
#include "Constants.h"
#include "Context.h"

#include <cctype>
#include <cwctype>


namespace Scanners
{
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isalpha,	std::isalpha,		std::iswalpha);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isalnum,	std::isalnum,		std::iswalnum);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isdigit,	std::isdigit,		std::iswdigit);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isblank,	std::isblank,		std::iswblank);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(iscntrl,	std::iscntrl,		std::iswcntrl);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isgraph,	std::isgraph,		std::iswgraph);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isprint,	std::isprint,		std::iswprint);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(ispunct,	std::ispunct,		std::iswpunct);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isspace,	std::isspace,		std::iswspace);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isxdigit, std::isxdigit,	std::iswxdigit);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(islower,	std::islower,		std::iswlower);
	DEFINE_CHAR_PARSER_WITH_STD_FUNCTIONS(isupper,	std::isupper,		std::iswupper);

	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isalpha_str, std::isalpha, std::iswalpha);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isalnum_str, std::isalnum, std::iswalnum);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isdigit_str, std::isdigit, std::iswdigit);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isblank_str, std::isblank, std::iswblank);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(iscntrl_str, std::iscntrl, std::iswcntrl);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isgraph_str, std::isgraph, std::iswgraph);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isprint_str, std::isprint, std::iswprint);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(ispunct_str, std::ispunct, std::iswpunct);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isspace_str, std::isspace, std::iswspace);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isxdigit_str, std::isxdigit, std::iswxdigit);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(islower_str, std::islower, std::iswlower);
	DEFINE_STRING_PARSER_WITH_STD_FUNCTIONS(isupper_str, std::isupper, std::iswupper);

	DEFINE_FLOAT_PARSER_WITH_STD_FUNCTIONS(scanner_float_ctx, std::strtof, std::wcstof);
	DEFINE_FLOAT_PARSER_WITH_STD_FUNCTIONS(scanner_double_ctx, std::strtod, std::wcstod);
	DEFINE_FLOAT_PARSER_WITH_STD_FUNCTIONS(scanner_long_double_ctx, std::strtold, std::wcstold);

	DEFINE_INTEGER_PARSER_WITH_STD_FUNCTIONS(scanner_int_cts, std::strtol, std::wcstol, 10);
	DEFINE_INTEGER_PARSER_WITH_STD_FUNCTIONS(scanner_uint_cts, std::strtoul, std::wcstoul, 10);
	DEFINE_INTEGER_PARSER_WITH_STD_FUNCTIONS(scanner_long_int_cts, std::strtoll, std::wcstoll, 10);
	DEFINE_INTEGER_PARSER_WITH_STD_FUNCTIONS(scanner_long_uint_cts, std::strtoull, std::wcstoull, 10);



	template<ConceptCharType OwnCharType>
	class CScannerStringRaw
	{
		std::basic_string_view<OwnCharType> m_literal;

	public:
		constexpr CScannerStringRaw(std::basic_string_view<OwnCharType> literal) noexcept
			: m_literal{literal}
		{};

		template<ConceptCharType CharType, typename TContext>
		constexpr static auto GetReturnType()
		{
			static_assert(std::is_same_v<CharType, OwnCharType>, "CScannerStringRaw::GetReturnType() : own char type != char type from ParseFunction");
			return std::basic_string<OwnCharType>{};
		}

		template<ConceptCharType CharType, typename TContext>
		bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val) const
		{
			if(ptr_string_end - ptr_string < m_literal.length())
				return false;

			if constexpr(std::is_same_v<CharType, char>)
			{
				const bool equal = std::strncmp(ptr_string, m_literal.data(), m_literal.length()) == 0;

				if(equal)
				{
					ptr_string += m_literal.length();
					if constexpr(!std::remove_cvref_t<TContext>::IsOmitedStatic())
						_val = m_literal;
				}
				
				return equal;
			}
			else if constexpr(std::is_same_v<CharType, wchar_t>)
			{
				const bool equal = std::wcsncmp(ptr_string, m_literal.data(), m_literal.length()) == 0;

				if(equal)
				{
					ptr_string += m_literal.length();
					if constexpr(!std::remove_cvref_t<TContext>::IsOmitedStatic())
						_val = m_literal;
				}
				
				return equal;
			}
			else
				static_assert(false, "CScannerStringRaw::Parse() : forbidden char type");
		}

	};


	template<ConceptCharType OwnCharType>
	class CScannerCharRaw
	{
		OwnCharType m_symbol;

	public:
		constexpr CScannerCharRaw(OwnCharType symbol) noexcept
			: m_symbol{symbol}
		{
		};

		template<ConceptCharType CharType, typename TContext>
		constexpr static auto GetReturnType()
		{
			static_assert(std::is_same_v<CharType, OwnCharType>, "CScannerStringRaw::GetReturnType() : own char type != char type from ParseFunction");
			return OwnCharType{};
		}

		template<ConceptCharType CharType, typename TContext>
		bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val) const
		{
			if(ptr_string >= ptr_string_end)
				return false;

			if constexpr(std::is_same_v<CharType, char> || std::is_same_v<CharType, wchar_t>)
			{
				const bool equal = *ptr_string == m_symbol;

				if(equal)
				{
					if constexpr(!std::remove_cvref_t<TContext>::IsOmitedStatic())
						_val = ptr_string;

					ptr_string++;
				}


				return equal;
			}
			else
				static_assert(false, "CScannerStringRaw::Parse() : forbidden char type");
		}

	};
};


class CScannerСharAny
{
public:
	constexpr CScannerСharAny() noexcept = default;

	template<ConceptCharType CharType, typename TContext>
	constexpr static auto GetReturnType()
	{
		return CharType{};
	}

	template<ConceptCharType CharType, typename TContext>
	bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &_val) const
	{
		if constexpr(!(std::is_same_v<CharType, char> || std::is_same_v<CharType, wchar_t>))
			static_assert(false, "CScannerStringRaw::Parse() : forbidden char type");

		if(ptr_string != ptr_string_end)
		{
			if constexpr(!std::remove_cvref_t<TContext>::IsOmitedStatic())
				_val = *ptr_string++;

			return true;
		}

		return false;
	}
};


namespace traits
{
		// this monstrosity should be hidden inside BaseParser class
		template<typename TParsingClass, ConceptCharType CharType, typename TContext>
		struct attribute
		{
			using type = decltype(TParsingClass::template GetReturnType<CharType, TContext>());
		};


		template<typename TParsingClass, ConceptCharType CharType, typename TContext>
		using attribute_t = attribute<std::remove_cvref_t<TParsingClass>, CharType, std::remove_cvref_t<TContext>>::type;
};