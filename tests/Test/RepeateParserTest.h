#pragma once

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include <common/parsing.h>
#include "tests_utils.h"

using namespace aliases;

std::string kEmpty = "";
std::string kDoubles = "1.5 2.5 3.14159";
std::string kWords = "apple banana cherry";
std::string kDigits = "12345";
std::string kUInts = "100 200 300";
std::string kInvalidStart = "abc 1 2 3";

std::wstring kEmptyW = L"";
std::wstring kDoublesW = L"1.5 2.5 3.14159";
std::wstring kWordsW = L"apple banana cherry";
std::wstring kDigitsW = L"12345";
std::wstring kUIntsW = L"100 200 300";
std::wstring kInvalidStartW = L"abc 1 2 3";

TEST(RepeatParser, ParsesDoublesNarrow)
{
    auto parser = *double_;
    EXPECT_EQ(result_getter(ParseLexeme2(kEmpty, parser)), std::vector<double>({}));
    EXPECT_EQ(result_getter(ParseLexeme2(kDoubles, parser)), std::vector<double>({ 1.5, 2.5, 3.14159 }));
}

TEST(RepeatParser, ParsesStringAlphaNarrow)
{
    auto parser = *str_alpha;
    EXPECT_EQ(result_getter(ParseLexeme2(kEmpty, parser)), std::vector<std::string>({}));
    EXPECT_EQ(result_getter(ParseLexeme2(kWords, parser)), std::vector<std::string>({ "apple", "banana", "cherry" }));
    EXPECT_EQ(result_getter(ParseLexeme2("123", parser)), std::vector<std::string>({}));
}

TEST(RepeatParser, ParsesCharDigitNarrow)
{
    auto parser = *char_digit;
    EXPECT_EQ(result_getter(ParseLexeme2(kEmpty, parser)), std::vector<char>({}));
    EXPECT_EQ(result_getter(ParseLexeme2(kDigits, parser)), std::vector<char>({ '1', '2', '3', '4', '5' }));
    EXPECT_EQ(result_getter(ParseLexeme2("abc", parser)), std::vector<char>({}));
}

TEST(RepeatParser, ParsesUnsignedIntsNarrow)
{
    auto parser = *uint_;
    EXPECT_EQ(result_getter(ParseLexeme2(kEmpty, parser)), std::vector<unsigned long>({}));
    EXPECT_EQ(result_getter(ParseLexeme2(kUInts, parser)), std::vector<unsigned long>({ 100, 200, 300 }));
}

TEST(RepeatParser, StopsOnInvalidFirstElementNarrow)
{
    auto parser = *int_;
    EXPECT_EQ(result_getter(ParseLexeme2(kInvalidStart, parser)), std::vector<long>({}));
}

TEST(RepeatParser, ParsesDoublesWide)
{
    auto parser = *double_;
    EXPECT_EQ(result_getter(ParseLexeme2(kEmptyW, parser)), std::vector<double>({}));
    EXPECT_EQ(result_getter(ParseLexeme2(kDoublesW, parser)), std::vector<double>({ 1.5, 2.5, 3.14159 }));
}

TEST(RepeatParser, ParsesStringAlphaWide)
{
    auto parser = *str_alpha;
    EXPECT_EQ(result_getter(ParseLexeme2(kEmptyW, parser)), std::vector<std::wstring>({}));
    EXPECT_EQ(result_getter(ParseLexeme2(kWordsW, parser)), std::vector<std::wstring>({ L"apple", L"banana", L"cherry" }));
    EXPECT_EQ(result_getter(ParseLexeme2(L"123", parser)), std::vector<std::wstring>({}));
}

TEST(RepeatParser, ParsesCharDigitWide)
{
    auto parser = *char_digit;
    EXPECT_EQ(result_getter(ParseLexeme2(kEmptyW, parser)), std::vector<wchar_t>({}));
    EXPECT_EQ(result_getter(ParseLexeme2(kDigitsW, parser)), std::vector<wchar_t>({ L'1', L'2', L'3', L'4', L'5' }));
    EXPECT_EQ(result_getter(ParseLexeme2(L"abc", parser)), std::vector<wchar_t>({}));
}

TEST(RepeatParser, ParsesUnsignedIntsWide)
{
    auto parser = *uint_;
    EXPECT_EQ(result_getter(ParseLexeme2(kEmptyW, parser)), std::vector<unsigned long>({}));
    EXPECT_EQ(result_getter(ParseLexeme2(kUIntsW, parser)), std::vector<unsigned long>({ 100U, 200U, 300U }));
}

TEST(RepeatParser, StopsOnInvalidFirstElementWide)
{
    auto parser = *int_;
    EXPECT_EQ(result_getter(ParseLexeme2(kInvalidStartW, parser)), std::vector<long>({}));
}