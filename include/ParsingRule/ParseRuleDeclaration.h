#pragma once

#include<functional>


template<typename TRuleDecl, ConceptParser TParserImplementation>
class ParsingRule
{
public:
	using parsing_attribute = TRuleDecl::parsing_attribute;
	
protected:
	TParserImplementation m_parser;

public:

	ParsingRule(TRuleDecl decl, TParserImplementation impl)
		: m_parser{std::forward<TParserImplementation>(impl)}
	{
	}

	template<ConceptCharType CharType, typename TParserSkipper>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper)
	{
		return m_parser.Parse(ptr_string, ptr_string_end, skipper);
	}

	template<ConceptCharType CharType>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end)
	{
		return m_parser.Parse(ptr_string, ptr_string_end);
	}

	template<ConceptCharType CharType, typename TParserSkipper>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper)
	{
		return m_parser.Scan(ptr_string, ptr_string_end, skipper);
	}

	template<ConceptCharType CharType>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end)
	{
		return m_parser.Scan(ptr_string, ptr_string_end);
	}

	auto GetValueAndReset()
	{
		return m_parser.GetValueAndReset();
	}

	void Reset()
	{
		m_parser.Reset();
	}
};


template<typename TResult>
class ParseRuleDeclaration
{
	std::function<bool(const char*&, const char*&)> m_parsing_function;
	

public:
	using parsing_attribute = TResult;

	ParseRuleDeclaration() = default;


	template<ConceptCharType CharType, typename TParserSkipper>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper);

	template<ConceptCharType CharType>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end);

	template<ConceptCharType CharType, typename TParserSkipper>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper);

	template<ConceptCharType CharType>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end);
	/*template<ConceptCharType CharType, typename TParserSkipper>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper)
	{
		UseSkipper(ptr_string, ptr_string_end, skipper);
		return m_parsing_function(ptr_string, ptr_string_end);
	}

	template<ConceptCharType CharType>
	bool Parse(const CharType*& ptr_string, const CharType*& ptr_string_end)
	{
		return m_parsing_function(ptr_string, ptr_string_end);
	}

	template<ConceptCharType CharType, typename TParserSkipper>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end, TParserSkipper& skipper)
	{
		UseSkipper(ptr_string, ptr_string_end, skipper);
		return m_parsing_function(ptr_string, ptr_string_end);
	}

	template<ConceptCharType CharType>
	bool Scan(const CharType*& ptr_string, const CharType*& ptr_string_end)
	{
		return Scan(ptr_string, ptr_string_end);
	}*/


	static constexpr bool IsOmited()
	{
		return false;
	}

	parsing_attribute GetValueAndReset();

	void Reset()
	{

	}

};


// metafunctions

//template<typename T> 
//struct is_rule : std::false_type {};
//
//template<typename T>
//struct is_rule<ParseRule<T>> : std::true_type {};
//
//template<typename T>
//constexpr bool is_rule_v = is_rule<T>::value;
//
//// \metafunctions
//
//// concepts
//
//template<typename T>
//concept ConceptParsingRule = is_rule_v<T>;

// \ concepts
