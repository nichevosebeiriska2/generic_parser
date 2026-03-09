//#include "ParserBase.h"
//#include "ParserOmited.h"
//#include "ParserRepeate.h"
//#include "ParserList.h"
//#include "ParserWithAction.h"
//#include "ParserSequence.h"
//#include "ParserAlternative.h"
//#include "ParseRuleDeclaration.h"
//
#include "Operators.h"
//
//#include "parser_aliases.h"
//#include "ExampleJson.h"
//
//#include <optional>
//#include <map>

#include "base_parser.h"
#include "alternative.h"
#include "sequence.h"
#include "list.h"
#include "repeate.h"
#include "parser_with_action.h"
#include "rule.h"


#include <fstream>

std::string read_file(std::string strPathToFile)
{
	std::ifstream file(strPathToFile);

	if (file.is_open())
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{} };

	return "";
}




Context ctx{ Parsers::Skippers::wchar::space };
ParserWithContext<Scanners::scanner_float_ctx> parser;
ParserWithActionNew action_for_rule(parser, [](auto& ctx) { return std::string{}; });
RuleNew<class rule_returns_float, std::string> rule;

IMPLEMENT_RULE_NEW(rule, action_for_rule);

template<ConceptContext TContext>
void fContext(TContext&& ctx)
{

}

#include <gtest/gtest.h>

void f_test_new_context()
{
	auto res = decltype(action_for_rule)::GetReturnType<char, decltype(ctx)>();
	Context ctx{ Parsers::Skippers::wchar::space};
	fContext(ctx);
	bool bcontext = is_context<decltype(ctx)>::value;

	Scanners::CScannerFloat s_f;

	std::string strInput = "12 123 11";

	const char *str = "11 123 11";
	const char *str_end = str + std::strlen(str);

	const wchar_t *wstr = L"11 123 11";
	const wchar_t *wstr_end = wstr + std::wcslen(wstr);
	wchar_t *end;
	char *endc;


	ParserLiteralWithContext ps(std::basic_string_view{"123"});

	ParserListNew listNew(parser, ps);
	//ParserRepeateNew rep(parser);
	ParserAltNew alt(parser, ps);
	ParserWithActionNew action(parser, [](auto& ctx) { return std::string{ ctx.Begin(), ctx.End() }; });

	auto seq3 = parser >> "2";

	bool bAction = is_parser_with_action_v<decltype(action)>;

	//ParseLexeme2(strInput, rule);
	//ParseLexeme2(strInput, action);
	//ParseLexeme2(strInput, alt);
	//ParseLexeme2(strInput, listNew);
	//ParseLexeme2(strInput, rep);
	auto seq2 = action >> ps >> parser ;
	auto lambda = [](auto& ctx) {return 1; };
	auto seq_with_action = seq2 [ lambda ];
	ParseLexeme2(strInput, seq2);

	parser >> ps >> parser;
	
	//auto r_l = ps.GetReturnType<wchar_t, decltype(ctx)>();
	//ps.ParseNew(wstr, wstr_end, ctx, r_l);
	//auto res = seq2.GetReturnType<wchar_t, decltype(ctx)>();
	//seq2.ParseNew(wstr, wstr_end, ctx, res);

}

//#include "g"
#include "BaseParserTest.h"
#include "RepeateParserTest.h"
#include "ListParserTest.h"
#include "AlternativeParserTest.h"


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}