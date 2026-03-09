#pragma once

#include "BaseParserTest.h"
#include "alternative.h"

std::string str_ints_and_alpha_strings = "1 abs 3 asdadad 123";


TEST(TestParserAlternative, TestAlt)
{
	auto parser_alt = int_| str_alpha;
	auto v_int_1 = std::variant<long, std::string>{ 1 };
	auto v_str_asd = std::variant<long, std::string>{ "asd"};

	EXPECT_EQ(result_getter(ParseLexeme2("1", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ "asd"}));
	EXPECT_EQ(result_getter(ParseLexeme2("1 asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("										1 asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("		asdadasdddddddasdad				1 asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ "asdadasdddddddasdad" }));
}

