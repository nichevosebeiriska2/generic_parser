#pragma once

#include <gtest/gtest.h>

#include "BaseParserTest.h"
#include "parser_predicate_not.h"

TEST(TestPredicateNot, Tests)
{
	EXPECT_EQ(status_getter(ParseLexeme2("1", !char_alpha)), true);
	EXPECT_EQ(status_getter(ParseLexeme2("a", !char_alpha)), false);

	EXPECT_EQ(status_getter(ParseLexeme2("1", !char_digit)), false);
	EXPECT_EQ(status_getter(ParseLexeme2("a", !char_digit)), true);

	EXPECT_EQ(status_getter(ParseLexeme2("#", !char_print)), false);
	EXPECT_EQ(status_getter(ParseLexeme2("\t", !char_print)), true);

	//seq
	EXPECT_EQ(status_getter(ParseLexeme2("[1]", !("[" >> char_digit >> "]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2("[a]", !("[" >> char_digit >> "]"))), true);

	// seq with list
	EXPECT_EQ(status_getter(ParseLexeme2("[1,2,3]", !("[" >> char_digit % "," >> "]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2("[a,b,c]", !("[" >> char_digit % "," >> "]"))), true);

	// seq with repeate
	EXPECT_EQ(status_getter(ParseLexeme2("[1 2 3]", !("[" >> *char_digit >> "]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2("[a b c]", !("[" >> *char_digit >> "]"))), true);

	//variant
	EXPECT_EQ(status_getter(ParseLexeme2("[a]", !("[" >> (char_digit | char_alpha) >> "]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2("[1]", !("[" >> (char_digit | char_alpha) >> "]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2("[#]", !("[" >> (char_digit | char_alpha) >> "]"))), true);

	auto action_on_quote = [](auto& ctx){return std::basic_string(ctx.Begin(), ctx.End()); };
	auto parser_any_char_symbol = ParserLiteralWithContext{"\\\""} | char_any;
	auto parser_quoted_string = *(!ParserLiteralWithContext{"\""} >> parser_any_char_symbol);
	EXPECT_EQ(result_getter(ParseLexeme2(R"(AS DZ as d \"\" 12 3%# $^T$ " 12 3A SD as d #!@ )", parser_quoted_string[action_on_quote])), R"(AS DZ as d \"\" 12 3%# $^T$)");
}

TEST(TestPredicateNot, TestsWChar)
{
	EXPECT_EQ(status_getter(ParseLexeme2(L"1", !char_alpha)), true);
	EXPECT_EQ(status_getter(ParseLexeme2(L"a", !char_alpha)), false);

	EXPECT_EQ(status_getter(ParseLexeme2(L"1", !char_digit)), false);
	EXPECT_EQ(status_getter(ParseLexeme2(L"a", !char_digit)), true);

	EXPECT_EQ(status_getter(ParseLexeme2(L"#", !char_print)), false);
	EXPECT_EQ(status_getter(ParseLexeme2(L"\t", !char_print)), true);

	//seq
	EXPECT_EQ(status_getter(ParseLexeme2(L"[1]", !(L"[" >> char_digit >> L"]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2(L"[a]", !(L"[" >> char_digit >> L"]"))), true);

	// seq with list
	EXPECT_EQ(status_getter(ParseLexeme2(L"[1,2,3]", !(L"[" >> char_digit % L"," >> L"]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2(L"[a,b,c]", !(L"[" >> char_digit % L"," >> L"]"))), true);

	// seq with repeate
	EXPECT_EQ(status_getter(ParseLexeme2(L"[1 2 3]", !(L"[" >> *char_digit >> L"]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2(L"[a b c]", !(L"[" >> *char_digit >> L"]"))), true);

	//variant
	EXPECT_EQ(status_getter(ParseLexeme2("[a]", !("[" >> (char_digit | char_alpha) >> "]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2("[1]", !("[" >> (char_digit | char_alpha) >> "]"))), false);
	EXPECT_EQ(status_getter(ParseLexeme2("[#]", !("[" >> (char_digit | char_alpha) >> "]"))), true);

	auto action_on_quote = [](auto &ctx)
	{
		return std::basic_string(ctx.Begin(), ctx.End());
	};

	auto parser_quoted_string = *(!ParserLiteralWithContext{"\""} >> ("\\\"" | char_any));
	EXPECT_EQ(result_getter(ParseLexeme2(R"(AS DZ as d \"\" 12 3%# $^T$ " 12 3A SD as d #!@ )", parser_quoted_string[action_on_quote])), R"(AS DZ as d \"\" 12 3%# $^T$)");
}