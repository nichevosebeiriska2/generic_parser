#pragma once

#include <limits>

#include "common.h"
#include "Constants.h"
#include "Context.h"
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
		template<ConceptCharType CharType>
		bool Scan(constCharPtr<CharType> ptr_to_string, constCharPtr<CharType> ptr_to_string_end)
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

		template<ConceptCharType CharType>
		bool Scan(constCharPtr<CharType> ptr_to_string, constCharPtr<CharType> ptr_to_string_end)
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

		constexpr ScannerStringLiteral(const ScannerStringLiteral<CharType> &other)
			: literal(other.literal)
		{
		}

		constexpr ScannerStringLiteral(ScannerStringLiteral<CharType> &&other)
			: literal(other.literal)
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
	class ScannerStringLiteralRaw : public InterfaceScanner<CharType>
	{
	public:
		using TInterface = InterfaceScanner<CharType>;
		using internal_char_type = TInterface::internal_char_type;

	protected:
		std::basic_string_view<CharType> m_str_prefix;
		std::basic_string_view<CharType> m_str_postfix;

	public:
		constexpr ScannerStringLiteralRaw(std::basic_string_view<CharType> str_prefix, std::basic_string_view<CharType> str_postfix)
			: m_str_prefix{str_prefix}
			, m_str_postfix{str_postfix}
		{
		}

		bool Scan(const TInterface::internal_char_type *ptr_to_string, const TInterface::internal_char_type *ptr_to_string_end)
		{
			if(m_str_prefix.length() + m_str_postfix.length() > (ptr_to_string_end - ptr_to_string))
				return false;

			if constexpr(std::is_same_v<CharType, char>)
			{
				const CharType *ptr = nullptr;
				if((strncmp(ptr_to_string, m_str_prefix.data(), m_str_prefix.length()) == 0))
				{
					if(ptr = std::strstr(ptr_to_string + m_str_prefix.length(), m_str_postfix.data()); ptr)
					{
						TInterface::number_of_scanned_chars = (ptr - ptr_to_string) + m_str_postfix.length();
						return true;
					}
				}

				TInterface::number_of_scanned_chars = 0;
				return false;
			}
			else if constexpr(std::is_same_v<CharType, wchar_t>)
			{
				const CharType *ptr = nullptr;
				if((wcsncmp(ptr_to_string, m_str_prefix.data(), m_str_prefix.length()) == 0))
				{
					if(ptr = std::wcsstr(ptr_to_string + m_str_prefix.length(), m_str_postfix.data()))
					{
						TInterface::number_of_scanned_chars = (ptr - ptr_to_string) + m_str_postfix.length();
						return true;
					}
				}

				TInterface::number_of_scanned_chars = 0;
				return false;
			}
			else
				static_assert(false, "ScannerStringLiteralRaw::Scan() - forbiden char type");

			return false;
		}
	};


	template<ConceptCharType CharType>
	class ScannerCharAny : public InterfaceScanner<CharType>
	{
	public:
		using TInterface = InterfaceScanner<CharType>;
		using internal_char_type = TInterface::internal_char_type;

	public:
		constexpr ScannerCharAny()
		{
		}

		template<ConceptCharType CharType>
		bool Scan(constCharPtr<CharType> ptr_to_string, constCharPtr<CharType> ptr_to_string_end)
		{
			return (TInterface::number_of_scanned_chars = ptr_to_string < ptr_to_string_end) > 0;
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

	DEFINE_CHAR_FROM_STR_FUNCTION(isalpha,	std::isalpha,		std::iswalpha);
	DEFINE_CHAR_FROM_STR_FUNCTION(isalnum,	std::isalnum,		std::iswalnum);
	DEFINE_CHAR_FROM_STR_FUNCTION(isdigit,	std::isdigit,		std::iswdigit);
	DEFINE_CHAR_FROM_STR_FUNCTION(isblank,	std::isblank,		std::iswblank);
	DEFINE_CHAR_FROM_STR_FUNCTION(iscntrl,	std::iscntrl,		std::iswcntrl);
	DEFINE_CHAR_FROM_STR_FUNCTION(isgraph,	std::isgraph,		std::iswgraph);
	DEFINE_CHAR_FROM_STR_FUNCTION(isprint,	std::isprint,		std::iswprint);
	DEFINE_CHAR_FROM_STR_FUNCTION(ispunct,	std::ispunct,		std::iswpunct);
	DEFINE_CHAR_FROM_STR_FUNCTION(isspace,	std::isspace,		std::iswspace);
	DEFINE_CHAR_FROM_STR_FUNCTION(isxdigit, std::isxdigit,	std::iswxdigit);
	DEFINE_CHAR_FROM_STR_FUNCTION(islower,	std::islower,		std::iswlower);
	DEFINE_CHAR_FROM_STR_FUNCTION(isupper,	std::isupper,		std::iswupper);

	DEFINE_STRING_FROM_STR_FUNCTION(isalpha_str, std::isalpha, std::iswalpha);
	DEFINE_STRING_FROM_STR_FUNCTION(isalnum_str, std::isalnum, std::iswalnum);
	DEFINE_STRING_FROM_STR_FUNCTION(isdigit_str, std::isdigit, std::iswdigit);
	DEFINE_STRING_FROM_STR_FUNCTION(isblank_str, std::isblank, std::iswblank);
	DEFINE_STRING_FROM_STR_FUNCTION(iscntrl_str, std::iscntrl, std::iswcntrl);
	DEFINE_STRING_FROM_STR_FUNCTION(isgraph_str, std::isgraph, std::iswgraph);
	DEFINE_STRING_FROM_STR_FUNCTION(isprint_str, std::isprint, std::iswprint);
	DEFINE_STRING_FROM_STR_FUNCTION(ispunct_str, std::ispunct, std::iswpunct);
	DEFINE_STRING_FROM_STR_FUNCTION(isspace_str, std::isspace, std::iswspace);
	DEFINE_STRING_FROM_STR_FUNCTION(isxdigit_str, std::isxdigit, std::iswxdigit);
	DEFINE_STRING_FROM_STR_FUNCTION(islower_str, std::islower, std::iswlower);
	DEFINE_STRING_FROM_STR_FUNCTION(isupper_str, std::isupper, std::iswupper);


#include "scanners_num_ctx.h"
#include "scanners_char_ctx.h"

	template<ConceptCharType OwnCharType>
	class CScannerStringRaw
	{
		std::basic_string_view<OwnCharType> m_literal;

	public:
		constexpr CScannerStringRaw(std::basic_string_view<OwnCharType> literal) noexcept
			: m_literal{literal}
		{};

		template<ConceptCharType CharType, typename TContext>
		consteval static auto GetType()
		{
			static_assert(std::is_same_v<CharType, OwnCharType>, "CScannerStringRaw::GetType() : own char type != char type from ParseFunction");
			return std::basic_string<OwnCharType>{};
		}

		template<ConceptCharType CharType, typename TContext>
		bool ParseFunction(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end, TContext &&context, std::type_identity_t<decltype(GetType<CharType, TContext>())> &_val)
		{
			CharType *ptr_end;

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
				static_assert(false, "CScannerFloat::Parse() : forbidden char type");
		}

	};

};

namespace traits
{
	namespace scanners
	{
		// this monstrosity should be hidden inside BaseParser class
		template<typename TScanner, ConceptCharType CharType, typename TContext>
		struct attribute
		{
			using type = decltype(TScanner::template GetType<CharType, TContext>());
		};


		template<typename TScanner, ConceptCharType CharType, typename TContext>
		using attribute_t = attribute<TScanner, CharType, TContext>::type;
	};

};