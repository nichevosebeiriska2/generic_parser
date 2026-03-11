#pragma once

#include <gtest/gtest.h>

#include "base_parser.h"
#include "Operators.h"

using namespace aliases;

constexpr auto result_getter = [](auto pair_parsed_result) {return pair_parsed_result.second; };
constexpr auto status_getter = [](auto pair_parsed_result) {return pair_parsed_result.first; };

auto lambda_identity = [](auto arg) { return arg; };

TEST(TestBaseParser, LongInteger) 
{
	std::string str_input = "123123";

	auto  parser = int_;

	auto result_getter = [](auto pair_parsed_result) {return pair_parsed_result.second; };

	EXPECT_EQ(result_getter(ParseLexeme2("0", parser)),				(int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("1", parser)),				(int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("953457345", parser)),		(int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("42", parser)),			(int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("2147483647", parser)),	(int)2147483647);

	EXPECT_EQ(result_getter(ParseLexeme2("+0", parser)), (int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("+1", parser)), (int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("+42", parser)), (int)+42);
	EXPECT_EQ(result_getter(ParseLexeme2("+953457345", parser)), (int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("+2147483647", parser)), (int)2147483647);

	EXPECT_EQ(result_getter(ParseLexeme2("-0", parser)), (int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("-1", parser)), (int)-1);
	EXPECT_EQ(result_getter(ParseLexeme2("-42", parser)), (int)-42);
	EXPECT_EQ(result_getter(ParseLexeme2("-953457345", parser)), (int)-953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("-2147483647", parser)), (int)-2147483647);

	EXPECT_EQ(result_getter(ParseLexeme2("0000", parser)), (int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("0000001", parser)), (int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("00000042", parser)), (int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("0000953457345", parser)), (int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("00000002147483647", parser)), (int)2147483647);
}

TEST(TestBaseParser, LongLongInteger)
{
	std::string str_input = "123123";

	auto parser = long_int_;

	auto result_getter = [](auto pair_parsed_result) {return pair_parsed_result.second; };

	EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), (long long  int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("1", parser)), (long long  int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), (long long  int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("953457345", parser)), (long long  int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("9223372036854775807", parser)), (long long  int)9223372036854775807);

	EXPECT_EQ(result_getter(ParseLexeme2("-0", parser)), (long long  int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("-1", parser)), (long long  int) - 1);
	EXPECT_EQ(result_getter(ParseLexeme2("-42", parser)), (long long  int) - 42);
	EXPECT_EQ(result_getter(ParseLexeme2("-953457345", parser)), (long long  int) - 953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("-9223372036854775807", parser)), (long long  int) - 9223372036854775807);

	EXPECT_EQ(result_getter(ParseLexeme2("+0", parser)), (long long  int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("+1", parser)), (long long  int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("+42", parser)), (long long  int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("+953457345", parser)), (long long  int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("+9223372036854775807", parser)), (long long  int)9223372036854775807);

	EXPECT_EQ(result_getter(ParseLexeme2("0000", parser)), (long long  int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("0000001", parser)), (long long  int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("00000042", parser)), (long long  int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("0000953457345", parser)), (long long  int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("00000009223372036854775807", parser)), (long long  int)9223372036854775807);
}

TEST(TestBaseParser, LongUnsignedInteger)
{
	std::string str_input = "123123";

	auto parser = uint_;

	auto result_getter = [](auto pair_parsed_result) {return pair_parsed_result.second; };

	EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), (int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("1", parser)), (int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), (int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("953457345", parser)), (int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("2147483647", parser)), (int)2147483647);

	EXPECT_EQ(result_getter(ParseLexeme2("+0", parser)), (int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("+1", parser)), (int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("+42", parser)), (int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("+953457345", parser)), (int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("+2147483647", parser)), (int)2147483647);

	EXPECT_EQ(result_getter(ParseLexeme2("0000", parser)), (int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("0000001", parser)), (int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("00000042", parser)), (int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("0000953457345", parser)), (int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("00000002147483647", parser)), (int)2147483647);
}


TEST(TestBaseParser, LongLongUnsignedInteger)
{
	std::string str_input = "123123";

	auto parser = long_uint_;

	auto result_getter = [](auto pair_parsed_result) {return pair_parsed_result.second; };

	EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), (long long  int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("1", parser)), (long long  int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), (long long  int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("953457345", parser)), (long long  int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("9223372036854775807", parser)), (long long  int)9223372036854775807);

	EXPECT_EQ(result_getter(ParseLexeme2("+0", parser)), (long long  int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("+1", parser)), (long long  int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("+42", parser)), (long long  int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("+953457345", parser)), (long long  int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("+9223372036854775807", parser)), (long long  int)9223372036854775807);

	EXPECT_EQ(result_getter(ParseLexeme2("0000", parser)), (long long  int)0);
	EXPECT_EQ(result_getter(ParseLexeme2("0000001", parser)), (long long  int)1);
	EXPECT_EQ(result_getter(ParseLexeme2("00000042", parser)), (long long  int)42);
	EXPECT_EQ(result_getter(ParseLexeme2("0000953457345", parser)), (long long  int)953457345);
	EXPECT_EQ(result_getter(ParseLexeme2("00000009223372036854775807", parser)), (long long  int)9223372036854775807);
}


TEST(TestBaseParser, Float)
{
	std::string str_input = "123123.123123123f";

	auto parser = float_;
	auto parser_double = double_;

	auto [parsed, result] = ParseLexeme2(str_input, parser);

	// CHAR //
	// FLOAT
	// common
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("0.0f", parser)), 0.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0f", parser)),1.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0f", parser)), 42.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("953457345.0f", parser)), 953457345.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("9223372036854775807.0f", parser)), 9223372036854775807.0f);

	// no 'f' flag
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("0.0", parser)), 0.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0", parser)), 1.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0", parser)), 42.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("953457345.0", parser)), 953457345.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("9223372036854775807.0", parser)), 9223372036854775807.0f);

	// no 'f' flag negative
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-0.0", parser)), -0.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.0", parser)), -1.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-42.0", parser)), -42.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-953457345.0", parser)), -953457345.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-9223372036854775807.0", parser)), -9223372036854775807.0f);

	// with exp
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0e3", parser)), 1000.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0e7", parser)), 420000000.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("953457345.0e2", parser)), 95345734500.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("9223372036854775807.0e-1", parser)), 922337203685477580.70f);

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0E3", parser)), 1000.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0E7", parser)), 420000000.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("953457345.0E2", parser)), 95345734500.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("9223372036854775807.0E-1", parser)), 922337203685477580.70f);

	// negative with exp
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.0e3", parser)), -1000.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-42.0e7", parser)), -420000000.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-953457345.0e2", parser)), -95345734500.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-9223372036854775807.0e-1", parser)), -922337203685477580.70f);

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.0E3", parser)), -1000.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-42.0E7", parser)), -420000000.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-953457345.0E2", parser)), -95345734500.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-9223372036854775807.0E-1", parser)), -922337203685477580.70f);

	// max/min

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("3.402823466e+38", parser)), std::numeric_limits<float>::max());
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-3.402823466e+38", parser)), std::numeric_limits<float>::lowest());

	// \FLOAT

	// DOUBLE
	// common
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("0.0f", parser_double)), 0.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0f", parser_double)), 1.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0f", parser_double)), 42.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("953457345.0f", parser_double)), 953457345.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("9223372036854775807.0f", parser_double)), 9223372036854775807.0);

	// no 'f' flag
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("0.0", parser_double)), 0.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0", parser_double)), 1.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0", parser_double)), 42.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("953457345.0", parser_double)), 953457345.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("9223372036854775807.0", parser_double)), 9223372036854775807.0);

	// no 'f' flag negative
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-0.0", parser_double)), -0.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.0", parser_double)), -1.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-42.0", parser_double)), -42.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-953457345.0", parser_double)), -953457345.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-9223372036854775807.0", parser_double)), -9223372036854775807.0);

	// with exp
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0e3", parser_double)), 1000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0e7", parser_double)), 420000000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("953457345.0e2", parser_double)), 95345734500.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("9223372036854775807.0e-1", parser_double)), 922337203685477580.70);

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0E3", parser_double)), 1000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0E7", parser_double)), 420000000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("953457345.0E2", parser_double)), 95345734500.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("9223372036854775807.0E-1", parser_double)), 922337203685477580.70);

	// negative with exp
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.0e3", parser_double)), -1000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-42.0e7", parser_double)), -420000000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-953457345.0e2", parser_double)), -95345734500.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-9223372036854775807.0e-1", parser_double)), -922337203685477580.7);

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.0E3", parser_double)), -1000.);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-42.0E7", parser_double)), -420000000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-953457345.0E2", parser_double)), -95345734500.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-9223372036854775807.0E-1", parser_double)), -922337203685477580.7);

	// max/min

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.79769e+308", parser_double)), std::numeric_limits<double>::max());
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.79769e+308", parser_double)), std::numeric_limits<double>::lowest());

	// \DOUBLE

	// WCHAR //

		// FLOAT
	// common
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"0.0f", parser)), 0.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.0f", parser)), 1.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"42.0f", parser)), 42.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"953457345.0f", parser)), 953457345.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"9223372036854775807.0f", parser)), 9223372036854775807.0f);

	// no 'f' flag
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"0.0", parser)), 0.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.0", parser)), 1.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"42.0", parser)), 42.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"953457345.0", parser)), 953457345.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"9223372036854775807.0", parser)), 9223372036854775807.0f);

	// no 'f' flag negative
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-0.0", parser)), -0.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-1.0", parser)), -1.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-42.0", parser)), -42.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-953457345.0", parser)), -953457345.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-9223372036854775807.0", parser)), -9223372036854775807.0f);

	// with exp
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.0e3", parser)), 1000.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"42.0e7", parser)), 420000000.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"953457345.0e2", parser)), 95345734500.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"9223372036854775807.0e-1", parser)), 922337203685477580.70f);

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.0E3", parser)), 1000.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"42.0E7", parser)), 420000000.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"953457345.0E2", parser)), 95345734500.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"9223372036854775807.0E-1", parser)), 922337203685477580.70f);

	// negative with exp
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-1.0e3", parser)), -1000.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-42.0e7", parser)), -420000000.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-953457345.0e2", parser)), -95345734500.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-9223372036854775807.0e-1", parser)), -922337203685477580.70f);

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-1.0E3", parser)), -1000.f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-42.0E7", parser)), -420000000.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-953457345.0E2", parser)), -95345734500.0f);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-9223372036854775807.0E-1", parser)), -922337203685477580.70f);

	// max/min

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"3.402823466e+38", parser)), std::numeric_limits<float>::max());
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-3.402823466e+38", parser)), std::numeric_limits<float>::lowest());

	// \FLOAT

	// DOUBLE
	// common
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"0.0f", parser_double)), 0.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.0f", parser_double)), 1.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"42.0f", parser_double)), 42.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"953457345.0f", parser_double)), 953457345.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"9223372036854775807.0f", parser_double)), 9223372036854775807.0);

	// no 'f' flag
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"0.0", parser_double)), 0.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.0", parser_double)), 1.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"42.0", parser_double)), 42.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"953457345.0", parser_double)), 953457345.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"9223372036854775807.0", parser_double)), 9223372036854775807.0);

	// no 'f' flag negative
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-0.0", parser_double)), -0.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-1.0", parser_double)), -1.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-42.0", parser_double)), -42.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-953457345.0", parser_double)), -953457345.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-9223372036854775807.0", parser_double)), -9223372036854775807.0);

	// with exp
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.0e3", parser_double)), 1000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"42.0e7", parser_double)), 420000000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"953457345.0e2", parser_double)), 95345734500.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"9223372036854775807.0e-1", parser_double)), 922337203685477580.70);

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.0E3", parser_double)), 1000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"42.0E7", parser_double)), 420000000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"953457345.0E2", parser_double)), 95345734500.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"9223372036854775807.0E-1", parser_double)), 922337203685477580.70);

	// negative with exp
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-1.0e3", parser_double)), -1000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-42.0e7", parser_double)), -420000000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-953457345.0e2", parser_double)), -95345734500.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-9223372036854775807.0e-1", parser_double)), -922337203685477580.7);

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-1.0E3", parser_double)), -1000.);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-42.0E7", parser_double)), -420000000.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-953457345.0E2", parser_double)), -95345734500.0);
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-9223372036854775807.0E-1", parser_double)), -922337203685477580.7);

	// max/min

	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"1.79769e+308", parser_double)), std::numeric_limits<double>::max());
	EXPECT_FLOAT_EQ(result_getter(ParseLexeme2(L"-1.79769e+308", parser_double)), std::numeric_limits<double>::lowest());

	// \DOUBLE
}

// each string parser is wrapper of std:: function from 'cctype' so the behave pretty much the same
// few tests for alpha/digit/alnum parsers
TEST(TestBaseParser, TestParseStringAlpha)
{
	EXPECT_EQ(result_getter(ParseLexeme2("asd", str_alpha)), "asd");
	EXPECT_EQ(result_getter(ParseLexeme2("asd asd", str_alpha)), "asd");
	EXPECT_EQ(result_getter(ParseLexeme2("asdasdasd#asd", str_alpha)), "asdasdasd");
	EXPECT_EQ(result_getter(ParseLexeme2("123asdasdasd#asd", str_alpha)), "");
	EXPECT_EQ(result_getter(ParseLexeme2("1", str_alpha)), "");

	EXPECT_EQ(result_getter(ParseLexeme2(L"asd", str_alpha)), L"asd");
	EXPECT_EQ(result_getter(ParseLexeme2(L"asd asd", str_alpha)), L"asd");
	EXPECT_EQ(result_getter(ParseLexeme2(L"asdasdasd#asd", str_alpha)), L"asdasdasd");
	EXPECT_EQ(result_getter(ParseLexeme2(L"123asdasdasd#asd", str_alpha)), L"");
	EXPECT_EQ(result_getter(ParseLexeme2(L"1", str_alpha)), L"");
}

TEST(TestBaseParser, TestParseStringAlnum)
{
	EXPECT_EQ(result_getter(ParseLexeme2("1", str_alnum)), "1");
	EXPECT_EQ(result_getter(ParseLexeme2("a", str_alnum)), "a");
	EXPECT_EQ(result_getter(ParseLexeme2("asd123", str_alnum)), "asd123");
	EXPECT_EQ(result_getter(ParseLexeme2("123asd", str_alnum)), "123asd");
	EXPECT_EQ(result_getter(ParseLexeme2("123asd erwtrwer123123", str_alnum)), "123asd");
	EXPECT_EQ(result_getter(ParseLexeme2("asdasdasd#asd", str_alnum)), "asdasdasd");

	EXPECT_EQ(result_getter(ParseLexeme2(L"1", str_alnum)), L"1");
	EXPECT_EQ(result_getter(ParseLexeme2(L"a", str_alnum)), L"a");
	EXPECT_EQ(result_getter(ParseLexeme2(L"asd123", str_alnum)), L"asd123");
	EXPECT_EQ(result_getter(ParseLexeme2(L"123asd", str_alnum)), L"123asd");
	EXPECT_EQ(result_getter(ParseLexeme2(L"123asd erwtrwer123123", str_alnum)), L"123asd");
	EXPECT_EQ(result_getter(ParseLexeme2(L"asdasdasd#asd", str_alnum)), L"asdasdasd");
}

TEST(TestBaseParser, TestParseStringDigit)
{
	EXPECT_EQ(result_getter(ParseLexeme2("123", str_digit)), "123");
	EXPECT_EQ(result_getter(ParseLexeme2("123asd", str_digit)), "123");
	EXPECT_EQ(result_getter(ParseLexeme2("12312312312#asd", str_digit)), "12312312312");
	EXPECT_EQ(result_getter(ParseLexeme2("t12312312312#asd", str_digit)), "");
	EXPECT_EQ(result_getter(ParseLexeme2("a", str_digit)), "");

	EXPECT_EQ(result_getter(ParseLexeme2(L"123", str_digit)), L"123");
	EXPECT_EQ(result_getter(ParseLexeme2(L"123asd", str_digit)), L"123");
	EXPECT_EQ(result_getter(ParseLexeme2(L"12312312312#asd", str_digit)), L"12312312312");
	EXPECT_EQ(result_getter(ParseLexeme2(L"t12312312312#asd", str_digit)), L"");
	EXPECT_EQ(result_getter(ParseLexeme2(L"a", str_digit)), L"");
}

TEST(TestBaseParser, TestParserStringPrintable)
{
	EXPECT_EQ(result_getter(ParseLexeme2(R"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)", str_print)), R"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)");
	EXPECT_EQ(result_getter(ParseLexeme2(R"(!"#$%&	'()*+,-./:;<=>?@[\]^_`{|}~)", str_print)), R"(!"#$%&)"); //tab
	EXPECT_EQ(result_getter(ParseLexeme2(R"(!"#$%&
'()*+,-./:;<=>?@[\]^_`{|}~)", str_print)), R"(!"#$%&)"); //newline

	// with prefix spaces
	EXPECT_EQ(result_getter(ParseLexeme2(LR"(  !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)", str_print)), LR"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)");
	EXPECT_EQ(result_getter(ParseLexeme2(LR"(  !"#$%&	'()*+,-./:;<=>?@[\]^_`{|}~)", str_print)), LR"(!"#$%&)"); //tab
	EXPECT_EQ(result_getter(ParseLexeme2(LR"(  !"#$%&
'()*+,-./:;<=>?@[\]^_`{|}~)", str_print)), LR"(!"#$%&)"); //newline
}

// and some char parsers

TEST(TestBaseParser, TestParseCharlpha)
{
	EXPECT_EQ(result_getter(ParseLexeme2("asd", char_alpha)), 'a');
	EXPECT_EQ(result_getter(ParseLexeme2("asd asd", char_alpha)), 'a');
	EXPECT_EQ(result_getter(ParseLexeme2("asdasdasd#asd", char_alpha)), 'a');
	EXPECT_EQ(result_getter(ParseLexeme2("123asdasdasd#asd", char_alpha)), '\0');
	EXPECT_EQ(result_getter(ParseLexeme2("#123", char_alpha)), '\0');
	EXPECT_EQ(result_getter(ParseLexeme2("1asdasd1123", char_alpha)), '\0');

	EXPECT_EQ(result_getter(ParseLexeme2(L"asd", char_alpha)), L'a');
	EXPECT_EQ(result_getter(ParseLexeme2(L"asd asd", char_alpha)), L'a');
	EXPECT_EQ(result_getter(ParseLexeme2(L"asdasdasd#asd", char_alpha)), L'a');
	EXPECT_EQ(result_getter(ParseLexeme2(L"123asdasdasd#asd", char_alpha)), L'\0');
	EXPECT_EQ(result_getter(ParseLexeme2(L"#123", char_alpha)), L'\0');
	EXPECT_EQ(result_getter(ParseLexeme2(L"1asdasd1123", char_alpha)), L'\0');
}

TEST(TestBaseParser, TestParseCharAlnum)
{
	EXPECT_EQ(result_getter(ParseLexeme2("1", char_alnum)), '1');
	EXPECT_EQ(result_getter(ParseLexeme2("a", char_alnum)), 'a');
	EXPECT_EQ(result_getter(ParseLexeme2("asd123", char_alnum)), 'a');
	EXPECT_EQ(result_getter(ParseLexeme2("123asd", char_alnum)), '1');
	EXPECT_EQ(result_getter(ParseLexeme2("asdasdasd#asd", char_alnum)), 'a');

	EXPECT_EQ(result_getter(ParseLexeme2(L"1", char_alnum)), L'1');
	EXPECT_EQ(result_getter(ParseLexeme2(L"a", char_alnum)), L'a');
	EXPECT_EQ(result_getter(ParseLexeme2(L"asd123", char_alnum)), L'a');
	EXPECT_EQ(result_getter(ParseLexeme2(L"123asd", char_alnum)), L'1');
	EXPECT_EQ(result_getter(ParseLexeme2(L"asdasdasd#asd", char_alnum)), L'a');
	EXPECT_EQ(result_getter(ParseLexeme2(L"ф", char_alnum)), L'ф');

}

TEST(TestBaseParser, TestParseCharDigit)
{
	EXPECT_EQ(result_getter(ParseLexeme2("123", char_digit)), '1');
	EXPECT_EQ(result_getter(ParseLexeme2("123asd", char_digit)), '1');
	EXPECT_EQ(result_getter(ParseLexeme2("12312312312#asd", char_digit)), '1');
	EXPECT_EQ(result_getter(ParseLexeme2("t12312312312#asd", char_digit)), '\0');
	EXPECT_EQ(result_getter(ParseLexeme2("a", char_digit)), '\0');


	EXPECT_EQ(result_getter(ParseLexeme2(L"123", char_digit)), L'1');
	EXPECT_EQ(result_getter(ParseLexeme2(L"123asd", char_digit)), L'1');
	EXPECT_EQ(result_getter(ParseLexeme2(L"12312312312#asd", char_digit)), L'1');
	EXPECT_EQ(result_getter(ParseLexeme2(L"t12312312312#asd", char_digit)), L'\0');
	EXPECT_EQ(result_getter(ParseLexeme2(L"a", char_digit)), L'\0');
}

TEST(TestBaseParser, TestCharAny)
{
	EXPECT_EQ(result_getter(ParseLexeme2("0", char_any)), '0');
	EXPECT_EQ(result_getter(ParseLexeme2("1", char_any)), '1');
	EXPECT_EQ(result_getter(ParseLexeme2("2", char_any)), '2');
	EXPECT_EQ(result_getter(ParseLexeme2("3", char_any)), '3');
	EXPECT_EQ(result_getter(ParseLexeme2("4", char_any)), '4');
	EXPECT_EQ(result_getter(ParseLexeme2("5", char_any)), '5');
	EXPECT_EQ(result_getter(ParseLexeme2("6", char_any)), '6');
	EXPECT_EQ(result_getter(ParseLexeme2("7", char_any)), '7');
	EXPECT_EQ(result_getter(ParseLexeme2("8", char_any)), '8');
	EXPECT_EQ(result_getter(ParseLexeme2("9", char_any)), '9');

	EXPECT_EQ(result_getter(ParseLexeme2("a", char_any)), 'a');
	EXPECT_EQ(result_getter(ParseLexeme2("b", char_any)), 'b');
	EXPECT_EQ(result_getter(ParseLexeme2("c", char_any)), 'c');
	EXPECT_EQ(result_getter(ParseLexeme2("d", char_any)), 'd');
	EXPECT_EQ(result_getter(ParseLexeme2("z", char_any)), 'z');

	EXPECT_EQ(result_getter(ParseLexeme2("A", char_any)), 'A');
	EXPECT_EQ(result_getter(ParseLexeme2("B", char_any)), 'B');
	EXPECT_EQ(result_getter(ParseLexeme2("C", char_any)), 'C');
	EXPECT_EQ(result_getter(ParseLexeme2("D", char_any)), 'D');
	EXPECT_EQ(result_getter(ParseLexeme2("Z", char_any)), 'Z');

	EXPECT_EQ(result_getter(ParseLexeme2("!", char_any)), '!');
	EXPECT_EQ(result_getter(ParseLexeme2("\"", char_any)), '\"');
	EXPECT_EQ(result_getter(ParseLexeme2("#", char_any)), '#');
	EXPECT_EQ(result_getter(ParseLexeme2("$", char_any)), '$');
	EXPECT_EQ(result_getter(ParseLexeme2("%", char_any)), '%');
	EXPECT_EQ(result_getter(ParseLexeme2("&", char_any)), '&');
	EXPECT_EQ(result_getter(ParseLexeme2("'", char_any)), '\'');
	EXPECT_EQ(result_getter(ParseLexeme2("(", char_any)), '(');
	EXPECT_EQ(result_getter(ParseLexeme2(")", char_any)), ')');
	EXPECT_EQ(result_getter(ParseLexeme2("*", char_any)), '*');
	EXPECT_EQ(result_getter(ParseLexeme2("+", char_any)), '+');
	EXPECT_EQ(result_getter(ParseLexeme2(",", char_any)), ',');
	EXPECT_EQ(result_getter(ParseLexeme2("-", char_any)), '-');
	EXPECT_EQ(result_getter(ParseLexeme2(".", char_any)), '.');
	EXPECT_EQ(result_getter(ParseLexeme2("/", char_any)), '/');
	EXPECT_EQ(result_getter(ParseLexeme2(":", char_any)), ':');
	EXPECT_EQ(result_getter(ParseLexeme2(";", char_any)), ';');
	EXPECT_EQ(result_getter(ParseLexeme2("<", char_any)), '<');
	EXPECT_EQ(result_getter(ParseLexeme2("=", char_any)), '=');
	EXPECT_EQ(result_getter(ParseLexeme2(">", char_any)), '>');
	EXPECT_EQ(result_getter(ParseLexeme2("?", char_any)), '?');
	EXPECT_EQ(result_getter(ParseLexeme2("@", char_any)), '@');
	EXPECT_EQ(result_getter(ParseLexeme2("[", char_any)), '[');
	EXPECT_EQ(result_getter(ParseLexeme2("\\", char_any)), '\\');
	EXPECT_EQ(result_getter(ParseLexeme2("]", char_any)), ']');
	EXPECT_EQ(result_getter(ParseLexeme2("^", char_any)), '^');
	EXPECT_EQ(result_getter(ParseLexeme2("_", char_any)), '_');
	EXPECT_EQ(result_getter(ParseLexeme2("`", char_any)), '`');
	EXPECT_EQ(result_getter(ParseLexeme2("{", char_any)), '{');
	EXPECT_EQ(result_getter(ParseLexeme2("|", char_any)), '|');
	EXPECT_EQ(result_getter(ParseLexeme2("}", char_any)), '}');
	EXPECT_EQ(result_getter(ParseLexeme2("~", char_any)), '~');


	EXPECT_EQ(result_getter(ParseLexemeNoSkip("\t", char_any)), '\t');
	EXPECT_EQ(result_getter(ParseLexemeNoSkip("\r", char_any)), '\r');
	EXPECT_EQ(result_getter(ParseLexemeNoSkip("\n", char_any)), '\n');
	EXPECT_EQ(result_getter(ParseLexemeNoSkip(" ", char_any)), ' ');
}

TEST(TestBaseParser, TestCharAnyWChar)
{
	EXPECT_EQ(result_getter(ParseLexeme2(L"0", char_any)), L'0');
	EXPECT_EQ(result_getter(ParseLexeme2(L"1", char_any)), L'1');
	EXPECT_EQ(result_getter(ParseLexeme2(L"2", char_any)), L'2');
	EXPECT_EQ(result_getter(ParseLexeme2(L"3", char_any)), L'3');
	EXPECT_EQ(result_getter(ParseLexeme2(L"4", char_any)), L'4');
	EXPECT_EQ(result_getter(ParseLexeme2(L"5", char_any)), L'5');
	EXPECT_EQ(result_getter(ParseLexeme2(L"6", char_any)), L'6');
	EXPECT_EQ(result_getter(ParseLexeme2(L"7", char_any)), L'7');
	EXPECT_EQ(result_getter(ParseLexeme2(L"8", char_any)), L'8');
	EXPECT_EQ(result_getter(ParseLexeme2(L"9", char_any)), L'9');

	EXPECT_EQ(result_getter(ParseLexeme2(L"a", char_any)), L'a');
	EXPECT_EQ(result_getter(ParseLexeme2(L"b", char_any)), L'b');
	EXPECT_EQ(result_getter(ParseLexeme2(L"c", char_any)), L'c');
	EXPECT_EQ(result_getter(ParseLexeme2(L"d", char_any)), L'd');
	EXPECT_EQ(result_getter(ParseLexeme2(L"z", char_any)), L'z');

	EXPECT_EQ(result_getter(ParseLexeme2(L"A", char_any)), L'A');
	EXPECT_EQ(result_getter(ParseLexeme2(L"B", char_any)), L'B');
	EXPECT_EQ(result_getter(ParseLexeme2(L"C", char_any)), L'C');
	EXPECT_EQ(result_getter(ParseLexeme2(L"D", char_any)), L'D');
	EXPECT_EQ(result_getter(ParseLexeme2(L"Z", char_any)), L'Z');

	EXPECT_EQ(result_getter(ParseLexeme2(L"!", char_any)), L'!');
	EXPECT_EQ(result_getter(ParseLexeme2(L"\"", char_any)), L'\"');
	EXPECT_EQ(result_getter(ParseLexeme2(L"#", char_any)), L'#');
	EXPECT_EQ(result_getter(ParseLexeme2(L"$", char_any)), L'$');
	EXPECT_EQ(result_getter(ParseLexeme2(L"%", char_any)), L'%');
	EXPECT_EQ(result_getter(ParseLexeme2(L"&", char_any)), L'&');
	EXPECT_EQ(result_getter(ParseLexeme2(L"'", char_any)), L'\'');
	EXPECT_EQ(result_getter(ParseLexeme2(L"(", char_any)), L'(');
	EXPECT_EQ(result_getter(ParseLexeme2(L")", char_any)), L')');
	EXPECT_EQ(result_getter(ParseLexeme2(L"*", char_any)), L'*');
	EXPECT_EQ(result_getter(ParseLexeme2(L"+", char_any)), L'+');
	EXPECT_EQ(result_getter(ParseLexeme2(L",", char_any)), L',');
	EXPECT_EQ(result_getter(ParseLexeme2(L"-", char_any)), L'-');
	EXPECT_EQ(result_getter(ParseLexeme2(L".", char_any)), L'.');
	EXPECT_EQ(result_getter(ParseLexeme2(L"/", char_any)), L'/');
	EXPECT_EQ(result_getter(ParseLexeme2(L":", char_any)), L':');
	EXPECT_EQ(result_getter(ParseLexeme2(L";", char_any)), L';');
	EXPECT_EQ(result_getter(ParseLexeme2(L"<", char_any)), L'<');
	EXPECT_EQ(result_getter(ParseLexeme2(L"=", char_any)), L'=');
	EXPECT_EQ(result_getter(ParseLexeme2(L">", char_any)), L'>');
	EXPECT_EQ(result_getter(ParseLexeme2(L"?", char_any)), L'?');
	EXPECT_EQ(result_getter(ParseLexeme2(L"@", char_any)), L'@');
	EXPECT_EQ(result_getter(ParseLexeme2(L"[", char_any)), L'[');
	EXPECT_EQ(result_getter(ParseLexeme2(L"\\", char_any)), L'\\');
	EXPECT_EQ(result_getter(ParseLexeme2(L"]", char_any)), L']');
	EXPECT_EQ(result_getter(ParseLexeme2(L"^", char_any)), L'^');
	EXPECT_EQ(result_getter(ParseLexeme2(L"_", char_any)), L'_');
	EXPECT_EQ(result_getter(ParseLexeme2(L"`", char_any)), L'`');
	EXPECT_EQ(result_getter(ParseLexeme2(L"{", char_any)), L'{');
	EXPECT_EQ(result_getter(ParseLexeme2(L"|", char_any)), L'|');
	EXPECT_EQ(result_getter(ParseLexeme2(L"}", char_any)), L'}');
	EXPECT_EQ(result_getter(ParseLexeme2(L"~", char_any)), L'~');

	EXPECT_EQ(result_getter(ParseLexemeNoSkip(L"\t", char_any)), L'\t');
	EXPECT_EQ(result_getter(ParseLexemeNoSkip(L"\r", char_any)), L'\r');
	EXPECT_EQ(result_getter(ParseLexemeNoSkip(L"\n", char_any)), L'\n');
	EXPECT_EQ(result_getter(ParseLexemeNoSkip(L" ", char_any)), L' ');
}