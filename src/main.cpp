#include "ParserBase.h"
#include "ParserOmited.h"
#include "ParserRepeate.h"
#include "ParserList.h"
#include "ParserWithAction.h"
#include "ParserSequence.h"
#include "ParserAlternative.h"
#include "ParseRuleDeclaration.h"

#include "Operators.h"

#include "parser_aliases.h"
#include "ExampleJson.h"

#include <optional>
#include <map>

template<ConceptCharType CharType, ConceptParser ParserType, ConceptParser SkipperType>
auto ParseLexeme(const std::basic_string<CharType>& strInput, ParserType & parser, SkipperType & skipper)
{
	using namespace Parsers;
	// simple version of parsing function. This function will then have to handle errors.
	const CharType* strBegin = strInput.data();
	const CharType* strEnd = strInput.data() + strInput.length();

	
	if constexpr (is_unused_type_v<typename ParserType::parsing_attribute>)
		return parser.Scan(strBegin, strEnd, skipper);
	else
		return  parser.Parse(strBegin, strEnd, skipper) ? std::make_optional(parser.GetValueAndReset()) : std::nullopt;
}

using namespace Parsers;


#include <fstream>

std::string read_file(std::string strPathToFile)
{
	std::ifstream file(strPathToFile);

	if (file.is_open())
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{} };

	return "";
}

template<ConceptParser TParser>
bool f(TParser&& parser)
{
	return false;
}


template<ConceptScanner TScanner>
void f_scanner(TScanner scanner)
{

}


template<ConceptCharType CharType, ConceptParser ParserType, typename TResult>
auto Parse(const std::basic_string<CharType> &strInput, ParserType &parser, TResult &result)
{
	//CPa
	//using namespace Parsers;
	//// simple version of parsing function. This function will then have to handle errors.
	//const CharType *strBegin = strInput.data();
	//const CharType *strEnd = strInput.data() + strInput.length();


	//if constexpr(is_unused_type_v<typename ParserType::parsing_attribute>)
	//	return parser.Scan(strBegin, strEnd, skipper);
	//else
	//	return  parser.Parse(strBegin, strEnd, skipper) ? std::make_optional(parser.GetValueAndReset()) : std::nullopt;
}

namespace traits::parsers
{
	template<typename TParser, ConceptCharType CharType, typename TContext>
	struct attribute
	{
		using type = decltype(TParser::template GetReturnType<CharType, TContext>());
	};

	template<typename TParser, ConceptCharType CharType, typename TContext>
	using attribute_t = attribute<TParser, CharType, TContext>::type;

};

template<typename TParser>
class ParserRepeateNew
{
	TParser m_parser;

public:

	ParserRepeateNew(TParser && parser)
		: m_parser{parser}
	{}

	ParserRepeateNew(const TParser &parser)
		: m_parser{parser}
	{}


	template<ConceptCharType CharType, typename TContext>
	consteval static auto GetReturnType()
	{
		return std::vector<decltype(TParser::template GetReturnType<CharType, TContext>())>{};
	}

	template<ConceptCharType CharType, typename TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
								, constCharPtrRef<CharType> ptr_string_end
								, TContext &&context
								, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &attribute)
	{
		bool parsed = true;
		context.UseSkipper(ptr_string, ptr_string_end);// pre-skip

		while(m_parser.ParseNew(ptr_string, ptr_string_end, context, attribute.emplace_back()))// what about non default/copy constructible? looks like we have a problems is case TParser has parsing action
			context.UseSkipper(ptr_string, ptr_string_end);

		attribute.pop_back();

		return !attribute.empty();
	}
};


template<typename TParser, typename TParserDelimiter>
class ParserListNew
{
	TParser m_parser;
	TParserDelimiter m_parser_delimiter;

public:

	ParserListNew(TParser &&parser, TParserDelimiter&& delimiter)
		: m_parser{parser}
	{}

	ParserListNew(const TParser &parser, const TParserDelimiter &delimiter)
		: m_parser{parser}
		, m_parser_delimiter{delimiter}
	{}


	template<ConceptCharType CharType, typename TContext>
	consteval static auto GetReturnType()
	{
		return std::vector<decltype(TParser::template GetReturnType<CharType, TContext>())>{};
	}

	template<ConceptCharType CharType, typename TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
								, constCharPtrRef<CharType> ptr_string_end
								, TContext &&context
								, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &attribute)
	{
		bool delimiter_parser = true;
		context.UseSkipper(ptr_string, ptr_string_end);// pre-skip

		while(m_parser.ParseNew(ptr_string, ptr_string_end, context, attribute.emplace_back()) && delimiter_parser)// what about non default/copy constructible? looks like we have a problems is case TParser has parsing action
		{
			context.UseSkipper(ptr_string, ptr_string_end);

			auto delimiter_attr = decltype(TParserDelimiter::template GetReturnType<CharType, TContext>()){};
			delimiter_parser = m_parser_delimiter.ParseNew(ptr_string, ptr_string_end, context, delimiter_attr);
		}

		attribute.pop_back();

		return !attribute.empty();
	}
};

template<typename ... TParsers>
class ParserSeqNew
{
	std::tuple<TParsers...> m_tuple_parsers;

protected:

	template<ConceptCharType CharType, typename TContext, size_t ... Ts>
	bool ParseImpl(constCharPtrRef<CharType> ptr_string
								 , constCharPtrRef<CharType> ptr_string_end
								 , TContext &&context
								 , std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &attribute
								 , std::index_sequence<Ts...> seq)
	{
		return (std::get<Ts>(m_tuple_parsers).ParseNew(ptr_string, ptr_string_end, context, std::get<Ts>(attribute)) && ...);
	}

	template<ConceptCharType CharType, typename TContext, size_t...Ts>
	consteval static auto GetReturnTypeImpl(std::index_sequence<Ts...> seq)
	{
		return std::make_tuple(std::tuple_element_t<Ts, decltype(m_tuple_parsers)>::template GetReturnType<CharType, TContext>() ...);
	}

public:

	ParserSeqNew(TParsers ... parsers)
		: m_tuple_parsers{parsers ...}
	{
	}


	template<ConceptCharType CharType, typename TContext>
	consteval static auto GetReturnType()
	{
		return GetReturnTypeImpl<CharType, TContext>(std::make_index_sequence<std::tuple_size_v<decltype(m_tuple_parsers)>>{});
	}

	template<ConceptCharType CharType, typename TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
								, constCharPtrRef<CharType> ptr_string_end
								, TContext &&context
								, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &attribute)
	{
		return ParseImpl(ptr_string, ptr_string_end, context, attribute, std::make_index_sequence<sizeof...(TParsers)>{});
	}
};

template<typename ... TParsers>
class ParserAltNew
{
	std::tuple<TParsers...> m_tuple_parsers;

protected:

	template<ConceptCharType CharType, typename TContext, size_t ... Ts>
	bool ParseImpl(constCharPtrRef<CharType> ptr_string
								 , constCharPtrRef<CharType> ptr_string_end
								 , TContext &&context
								 , std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &attribute
								 , std::index_sequence<Ts...> seq)
	{
		return (std::get<Ts>(m_tuple_parsers).ParseNew(ptr_string, ptr_string_end, context, std::get<Ts>(attribute)) || ...);
	}

	template<ConceptCharType CharType, typename TContext, size_t...Ts>
	consteval static auto GetReturnTypeImpl(std::index_sequence<Ts...> seq)
	{
		return std::variant<decltype(std::tuple_element_t<Ts, decltype(m_tuple_parsers)>::template GetReturnType<CharType, TContext>()) ... >{};
	}

public:

	ParserAltNew(TParsers ... parsers)
		: m_tuple_parsers{parsers ...}
	{
	}


	template<ConceptCharType CharType, typename TContext>
	consteval static auto GetReturnType()
	{
		return GetReturnTypeImpl<CharType, TContext>(std::make_index_sequence<std::tuple_size_v<decltype(m_tuple_parsers)>>{});
	}

	template<ConceptCharType CharType, typename TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
								, constCharPtrRef<CharType> ptr_string_end
								, TContext &&context
								, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &attribute)
	{
		return ParseImpl(ptr_string, ptr_string_end, context, attribute, std::make_index_sequence<sizeof...(TParsers)>{});
	}
};

template<typename TParser, typename TAction>
class ParserWithActionNew
{
protected:
	TParser m_parser;
	TAction m_action;
public:

	template<ConceptCharType CharType, typename TContext>
	consteval static auto GetReturnType()
	{
		using tParserResultType = typename traits::parsers::attribute_t<TParser, CharType, std::remove_cvref_t<TContext>>;

		//CParsingResultContext ctx_result{attribute, parser_result};
		//return std::invoke_result_t<TAction, tParserResultType>{};
		return 1;
	}

	ParserWithActionNew(TParser&& parser, TAction&& action)
		: m_parser{parser}
		, m_action{action}
	{}

	ParserWithActionNew(const TParser &parser, TAction &&action)
		: m_parser{parser}
		, m_action{action}
	{}

	template<ConceptCharType CharType, typename TContext>
	bool ParseNew(constCharPtrRef<CharType> ptr_string
								, constCharPtrRef<CharType> ptr_string_end
								, TContext &&context
								, std::type_identity_t<decltype(GetReturnType<CharType, TContext>())> &attribute)
	{
		//return ParseImpl(ptr_string, ptr_string_end, context, attribute, std::make_index_sequence<sizeof...(TParsers)>{});
		using tParserResultType = typename traits::parsers::attribute_t<TParser, CharType, /*std::remove_cvref_t<TContext>*/decltype(context)>;
		tParserResultType parser_result{};

		bool parsed = m_parser.ParseNew(ptr_string, ptr_string_end, context, parser_result);

		if(parsed)
		{
			CParsingResultContext ctx_result{attribute, parser_result};
			m_action(ctx_result);
		}

		return parsed;
	}
};




template<ConceptCharType CharType>
bool ParseLexeme2(std::basic_string<CharType> & strInput, auto& parser)
{
	Context ctx{Skippers::space};

	const CharType *ptr_begin = strInput.data();
	const CharType *ptr_end = ptr_begin + strInput.length();

	auto result = traits::parsers::attribute_t<std::remove_cvref_t<decltype(parser)>, CharType, decltype(ctx)>{};

	return parser.ParseNew(ptr_begin, ptr_end, ctx, result);
}


void f_test_new_context()
{
	Context ctx{Skippers::wchar::space};

	Scanners::CScannerFloat s_f;

	std::string strInput = "12 123 11";

	const char *str = "11 123 11";
	const char *str_end = str + std::strlen(str);

	const wchar_t *wstr = L"11 123 11";
	const wchar_t *wstr_end = wstr + std::wcslen(wstr);
	wchar_t *end;
	char *endc;

	ParserWithContext<Scanners::scanner_float_ctx> parser;
	ParserLiteralWithContext ps(std::basic_string_view{"123"});

	ParserListNew listNew(parser, ps);
	ParserSeqNew seq2(parser, ps, parser);
	ParserRepeateNew rep(parser);
	ParserAltNew alt(parser, ps);
	ParserWithActionNew action(parser, [](auto& ctx){return 1;});

	ParseLexeme2(strInput, action);
	ParseLexeme2(strInput, alt);
	ParseLexeme2(strInput, listNew);
	ParseLexeme2(strInput, rep);
	ParseLexeme2(strInput, seq2);

	//auto r_l = ps.GetReturnType<wchar_t, decltype(ctx)>();
	//ps.ParseNew(wstr, wstr_end, ctx, r_l);
	//auto res = seq2.GetReturnType<wchar_t, decltype(ctx)>();
	//seq2.ParseNew(wstr, wstr_end, ctx, res);

}

//#include "g"

void main()
{
	using namespace Parsers;

	using namespace Scanners;

	
	using t = decltype(Context(Skippers::space));

	auto parser_int = _int{};

	f_test_new_context();

	bool b_tag = is_skipper_non_type_v<tag_skipper_non>;
	Context Ctx2(tag_skipper_non{});


	f(_string_lit{"1"});
	f(_int{});

	f(_int{} >> _int{}>> ")");
	f(*_int{});
	f(_int{} % ",");
	f(omit(_int{}));
	f(_int{} | _float{});

	f_scanner(Scanners::_isalpha_str{});
	Scanners::_isalpha_str aa;
	const char *str_alpha = "  abc1ds";
	const char *str_alpha_end = str_alpha + 5;
	const wchar_t *wstr_alpha = L"ab1cds";

	auto ctx_space = Context(Skippers::space);
	auto ctx_omited = OmitedFromThis(Ctx2);
	auto ctx_case_sensetive = CaseSensetiveFromThis(Ctx2);
	auto ctx_case_inse = CaseInsensetiveFromThis(Ctx2);
	int i = 0;
	//parser_int.ParseNew(str_alpha, str_alpha_end, ctx_space, i);

	std::tuple<int, int> tup;
	int ii;
	CParsingResultContext parse_ctx(tup, ii);

	auto b1 =aa.Scan(str_alpha, str_alpha + 5);
	auto b2 = aa.Scan(wstr_alpha, wstr_alpha + 5);
	std::string strJson = read_file("C:\\Users\\dmileykin\\source\\repos\\generic_parser\\include\\examples\\Json\\medium.txt");
	auto res = ParseLexeme(strJson, value, Skippers::space);

	int a = 1;
}