#pragma once

#include <gtest/gtest.h>

#include "list.h"
#include "Operators.h"
#include "BaseParserTest.h"

using namespace aliases;

std::string zero_ints = "";
std::string a_few_ints = "1 2 3 4";
std::string a_few_ints_with_spaces = "1         2                    3               4";
std::string a_few_ints_with_tabs = "1			2		3		4";
std::string some_rnd_ints = " -1 231345 -9999 2139 560946456 -3333333 823749823 111 222 33 444 55 -11 -22 -33 -44 459387";
std::string some_rnd_ints_with_invalid_symbol = " -1 231345 -9999 2139 560946456 -3333333 823749823 111 a 222 33 444 55 -11 -22 -33 -44 459387";

TEST(TestRepeate, TestRepeateZeroOrMoreInts)
{
	auto parser_list = *int_;

	auto v_empty = std::vector<long>{};
	auto v = std::vector<long>{ 1, 2, 3, 4 };
	auto v_rnd = std::vector<long>{ -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111, 222, 33, 444, 55, -11, -22, -33, -44, 459387 };
	auto v_rnd_with_invalid = std::vector<long>{ -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111 };

	EXPECT_EQ(result_getter(ParseLexeme2("", parser_list)), v_empty);
	EXPECT_EQ(result_getter(ParseLexeme2("1 2 3 4", parser_list)), v);
	EXPECT_EQ(result_getter(ParseLexeme2("   1         2                    3               4", parser_list)), v); // spaces
	EXPECT_EQ(result_getter(ParseLexeme2("		1			2		3		4", parser_list)), v); // tabs
	EXPECT_EQ(result_getter(ParseLexeme2(" -1 231345 -9999 2139 560946456 -3333333 823749823 111 222 33 444 55 -11 -22 -33 -44 459387", parser_list)), v_rnd);
	EXPECT_EQ(result_getter(ParseLexeme2(" -1 231345 -9999 2139 560946456 -3333333 823749823 111 a 222 33 444 55 -11 -22 -33 -44 459387", parser_list)), v_rnd_with_invalid);

	EXPECT_EQ(result_getter(ParseLexeme2(L"", parser_list)), v_empty);
	EXPECT_EQ(result_getter(ParseLexeme2(L"1 2 3 4", parser_list)), v);
	EXPECT_EQ(result_getter(ParseLexeme2(L"   1         2                    3               4", parser_list)), v); // spaces
	EXPECT_EQ(result_getter(ParseLexeme2(L"		1			2		3		4", parser_list)), v); // tabs
	EXPECT_EQ(result_getter(ParseLexeme2(L" -1 231345 -9999 2139 560946456 -3333333 823749823 111 222 33 444 55 -11 -22 -33 -44 459387", parser_list)), v_rnd);
	EXPECT_EQ(result_getter(ParseLexeme2(L" -1 231345 -9999 2139 560946456 -3333333 823749823 111 a 222 33 444 55 -11 -22 -33 -44 459387", parser_list)), v_rnd_with_invalid);
};