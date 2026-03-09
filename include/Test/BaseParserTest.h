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
}

TEST(TestBaseParser, TestStringParsers)
{
	EXPECT_EQ(result_getter(ParseLexeme2("asd", str_alpha)), std::basic_string("asd"));
	EXPECT_EQ(result_getter(ParseLexeme2("asd asd", str_alpha)), std::basic_string("asd"));
	EXPECT_EQ(result_getter(ParseLexeme2("1", str_alpha)), std::basic_string(""));

}