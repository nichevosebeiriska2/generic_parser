#pragma once

#include<functional>

// TRuleTag - just a tag to differ ParseRule<> - s with same TResult type
template<class TRuleTag, typename TResult>
class ParseRule
{
public:
	using parsing_attribute = TResult;

protected:
	parsing_attribute m_last_result{};

public:
	ParseRule() = default;


	template<ConceptCharType CharType, typename TParserSkipper>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper);

	template<ConceptCharType CharType>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end);

	template<ConceptCharType CharType, typename TParserSkipper>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper);

	template<ConceptCharType CharType>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end);
	
	static constexpr bool IsOmited() noexcept { return false; };
	parsing_attribute GetValueAndReset ();
	void Reset() noexcept {}

	auto Copy()
	{
		return *this;
	}
};
