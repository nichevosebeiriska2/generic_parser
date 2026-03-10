#pragma once

#include <gtest/gtest.h>

#include "list.h"
#include "Operators.h"
#include "BaseParserTest.h"

using namespace aliases;

std::string list_zero_ints = "";
std::string list_a_few_ints = "1, 2,  3, 4";
std::string list_a_few_ints_with_spaces = "1    ,     2                ,    3   ,           4";
std::string list_a_few_ints_with_tabs = "1			,2		,	3		,		4";
std::string list_with_another_separator = "1 <::> 2		<::>	3		<::>		4";
std::string list_some_rnd_ints = " -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111, 222, 33, 444, 55, -11, -22, -33, -44, 459387";
std::string list_some_rnd_ints_with_invalid_symbol = " -1 ,231345, -9999, 2139 ,560946456,-3333333, 823749823, 111, a ,222, 33, 444, 55, -11, -22, -33, -44, 459387";

TEST(TestList, TestsListZeroOrMoreInts)
{
	auto parser_list = int_ % ",";
	auto parser_list_another_sep = int_ % "<::>";

	auto parser_list_wchar = int_ % L",";
	auto parser_another_sep_wchar = int_ % L"<::>";

	EXPECT_EQ(result_getter(ParseLexeme2("", parser_list)), lambda_identity(std::vector<long>{}));
	EXPECT_EQ(result_getter(ParseLexeme2("1, 2,  3, 4", parser_list)),						lambda_identity(std::vector<long>{1,2,3,4}));
	EXPECT_EQ(result_getter(ParseLexeme2("1    ,     2                ,    3   ,           4", parser_list)),			lambda_identity(std::vector<long>{1, 2, 3, 4}));
	EXPECT_EQ(result_getter(ParseLexeme2("1			,2		,	3		,		4", parser_list)),				lambda_identity(std::vector<long>{1, 2, 3, 4}));
	EXPECT_EQ(result_getter(ParseLexeme2("1 <::> 2		<::>	3		<::>		4", parser_list_another_sep)),			lambda_identity(std::vector<long>{1, 2, 3, 4}));
	EXPECT_EQ(result_getter(ParseLexeme2(" -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111, 222, 33, 444, 55, -11, -22, -33, -44, 459387", parser_list)),						lambda_identity(std::vector<long>{ -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111, 222, 33, 444, 55, -11, -22, -33, -44, 459387 }));
	EXPECT_EQ(result_getter(ParseLexeme2(" -1 ,231345, -9999, 2139 ,560946456,-3333333, 823749823, 111, a ,222, 33, 444, 55, -11, -22, -33, -44, 459387", parser_list)), lambda_identity(std::vector<long>{ -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111 }));

	EXPECT_EQ(result_getter(ParseLexeme2(L"", parser_list_wchar)), lambda_identity(std::vector<long>{}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"1, 2,  3, 4", parser_list_wchar)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"1    ,     2                ,    3   ,           4", parser_list_wchar)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"1			,2		,	3		,		4", parser_list_wchar)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"1 <::> 2		<::>	3		<::>		4", parser_another_sep_wchar)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
	EXPECT_EQ(result_getter(ParseLexeme2(L" -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111, 222, 33, 444, 55, -11, -22, -33, -44, 459387", parser_list_wchar)), lambda_identity(std::vector<long>{ -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111, 222, 33, 444, 55, -11, -22, -33, -44, 459387 }));
	EXPECT_EQ(result_getter(ParseLexeme2(L" -1 ,231345, -9999, 2139 ,560946456,-3333333, 823749823, 111, a ,222, 33, 444, 55, -11, -22, -33, -44, 459387", parser_list_wchar)), lambda_identity(std::vector<long>{ -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111 }));
};
