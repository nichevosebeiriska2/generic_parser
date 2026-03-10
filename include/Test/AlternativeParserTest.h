#pragma once

#include "BaseParserTest.h"
#include "alternative.h"
#include "sequence.h"

std::string str_ints_and_alpha_strings = "1 abs 3 asdadad 123";


TEST(TestParserAlternative, TestAltStringInt)
{
	auto parser_alt = int_| str_alpha;
	auto v_int_1 = std::variant<long, std::string>{ 1 };
	auto v_str_asd = std::variant<long, std::string>{ "asd"};

	EXPECT_EQ(result_getter(ParseLexeme2("1", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ "asd"}));
	EXPECT_EQ(result_getter(ParseLexeme2("1 asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("										1 asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("		asdadasdddddddasdad				1 asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ "asdadasdddddddasdad" }));

	EXPECT_EQ(result_getter(ParseLexeme2(L"1", parser_alt)), lambda_identity(std::variant<long, std::wstring>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2(L"asd", parser_alt)), lambda_identity(std::variant<long, std::wstring>{ L"asd"}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"1 asd", parser_alt)), lambda_identity(std::variant<long, std::wstring>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2(L"										1 asd", parser_alt)), lambda_identity(std::variant<long, std::wstring>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2(L"		asdadasdddddddasdad				1 asd", parser_alt)), lambda_identity(std::variant<long, std::wstring>{ L"asdadasdddddddasdad" }));
}

TEST(TestParserAlternative, TestAltStringIntInBrackets)
{
	auto lambda_tup_to_long = [](auto &ctx) { return std::get<1>(ctx.GetValue()); };
	auto parser_long_in_brackets = ("[" >> int_ >> "]")[lambda_tup_to_long];
	auto parser_long_in_brackets_wchar = (L"[" >> int_ >> L"]")[lambda_tup_to_long];
	auto parser_alt = parser_long_in_brackets | str_alpha;

	auto parser_alt_wchar = parser_long_in_brackets_wchar | str_alpha;

	EXPECT_EQ(result_getter(ParseLexeme2("		[1]		", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("	 asd	", parser_alt)), lambda_identity(std::variant<long, std::string>{ "asd"}));
	EXPECT_EQ(result_getter(ParseLexeme2("	[1]		asd		", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("										[1] asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2("		asdadasdddddddasdad				[1] asd", parser_alt)), lambda_identity(std::variant<long, std::string>{ "asdadasdddddddasdad" }));

	EXPECT_EQ(result_getter(ParseLexeme2(L"			  [1]		", parser_alt_wchar)), lambda_identity(std::variant<long, std::wstring>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2(L"		  asd		", parser_alt_wchar)), lambda_identity(std::variant<long, std::wstring>{ L"asd"}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"		 [1]		 asd			", parser_alt_wchar)), lambda_identity(std::variant<long, std::wstring>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2(L"											[1]		asd", parser_alt_wchar)), lambda_identity(std::variant<long, std::wstring>{ 1 }));
	EXPECT_EQ(result_getter(ParseLexeme2(L"				asdadasdddddddasdad				[1] asd", parser_alt_wchar)), lambda_identity(std::variant<long, std::wstring>{ L"asdadasdddddddasdad" }));
}

TEST(TestParserAlternative, TestAltLists)
{
	auto lambda_tup_to_long = [](auto &ctx)
	{
		return std::get<1>(ctx.GetValue());
	};

	auto parser_list_longs_in_brackets = ("[" >> (int_ % ",") >> "]")[lambda_tup_to_long];
	auto parser_list_str_in_brackets = ("[" >> (str_alpha % ",") >> "]")[lambda_tup_to_long];
	auto parser_list_longs_in_brackets_wchar = (L"[" >> (int_ % L",") >> L"]")[lambda_tup_to_long];
	auto parser_list_str_in_brackets_wchar = (L"[" >> (str_alpha % L",") >> L"]")[lambda_tup_to_long];
	auto parser_alt = parser_list_longs_in_brackets | parser_list_str_in_brackets;
	auto parser_alt_wchar = parser_list_longs_in_brackets_wchar | parser_list_str_in_brackets_wchar;
	//
	using tVectorInts = std::vector<long>;
	using tVectorStrings = std::vector<std::string>;
	using tVectorWStrings = std::vector<std::wstring>;
	using tVariant = std::variant<tVectorInts, tVectorStrings>;
	using tVariantW = std::variant<tVectorInts, tVectorWStrings>;

	EXPECT_EQ(result_getter(ParseLexeme2("	 [9]	", parser_alt)), lambda_identity(tVariant{tVectorInts{9}}));
	EXPECT_EQ(result_getter(ParseLexeme2("	 [9,8,7,	6,5		,4		,	 3	, 2,1]	", parser_alt)), lambda_identity(tVariant{tVectorInts{9,8,7,6,5,4,3,2,1}}));
	EXPECT_EQ(result_getter(ParseLexeme2("	 [asd]	", parser_alt)), lambda_identity(tVariant{tVectorStrings{"asd"}}));
	EXPECT_EQ(result_getter(ParseLexeme2("	 [a,b,c ,asd, asdadadad, ASDASDSD]	", parser_alt)), lambda_identity(tVariant{tVectorStrings{"a","b","c" ,"asd", "asdadadad", "ASDASDSD"}}));
	EXPECT_EQ(result_getter(ParseLexeme2("		[1,2,3,4]		[a, b, asd, asdll]", parser_alt)), lambda_identity(tVariant{tVectorInts{1,2,3,4}}));
	EXPECT_EQ(result_getter(ParseLexeme2("		[a, b, asd, asdll]	[1,2,3,4]", parser_alt)), lambda_identity(tVariant{tVectorStrings{"a", "b", "asd", "asdll"}}));

	EXPECT_EQ(result_getter(ParseLexeme2(L"	 [9]	", parser_alt_wchar)), lambda_identity(tVariantW{tVectorInts{9}}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"	 [9,	8, 7,		6,		 5,4	,3		,2		,1]	", parser_alt_wchar)), lambda_identity(tVariantW{tVectorInts{9,8,7,6,5,4,3,2,1}}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"	 [asd]	", parser_alt_wchar)), lambda_identity(tVariantW{tVectorWStrings{L"asd"}}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"	 [a,b,c ,asd, asdadadad, ASDASDSD]	", parser_alt_wchar)), lambda_identity(tVariantW{tVectorWStrings{L"a",L"b",L"c" ,L"asd", L"asdadadad", L"ASDASDSD"}}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"		[1,2,3,4]		[a, b, asd, asdll]", parser_alt_wchar)), lambda_identity(tVariantW{tVectorInts{1,2,3,4}}));
	EXPECT_EQ(result_getter(ParseLexeme2(L"		[a, b, asd, asdll]	[1,2,3,4]", parser_alt_wchar)), lambda_identity(tVariantW{tVectorWStrings{L"a", L"b", L"asd", L"asdll"}}));
}


