#pragma once

#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include <cctype>

#include <common/parsing.h>
#include "tests_utils.h"

using namespace aliases;

TEST(ParserWithAction, IntToStringNarrow)
{
    auto l = [](auto& ctx) { return std::to_string(ctx.GetValue()); };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), "42");
    EXPECT_EQ(result_getter(ParseLexeme2("-99", parser)), "-99");
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), "0");
    EXPECT_EQ(result_getter(ParseLexeme2("2147483647", parser)), "2147483647");
}

TEST(ParserWithAction, IntToStringWide)
{
    auto l = [](auto& ctx) { return std::to_wstring(ctx.GetValue()); };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"42", parser)), L"42");
    EXPECT_EQ(result_getter(ParseLexeme2(L"-99", parser)), L"-99");
    EXPECT_EQ(result_getter(ParseLexeme2(L"0", parser)), L"0");
}

TEST(ParserWithAction, DoubleToStringNarrow)
{
    auto l = [](auto& ctx) { return std::to_string(ctx.GetValue()); };
    auto parser = double_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("3.14", parser)), "3.140000");
    EXPECT_EQ(result_getter(ParseLexeme2("0.0", parser)), "0.000000");
    EXPECT_EQ(result_getter(ParseLexeme2("-2.5", parser)), "-2.500000");
}

TEST(ParserWithAction, DoubleToStringWide)
{
    auto l = [](auto& ctx) { return std::to_wstring(ctx.GetValue()); };
    auto parser = double_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"3.14", parser)), L"3.140000");
    EXPECT_EQ(result_getter(ParseLexeme2(L"0.0", parser)), L"0.000000");
}

TEST(ParserWithAction, StringToLengthNarrow)
{
    auto l = [](auto& ctx) { return ctx.GetValue().size(); };
    auto parser = str_alpha[l];

    EXPECT_EQ(result_getter(ParseLexeme2("hello", parser)), 5);
    EXPECT_EQ(result_getter(ParseLexeme2("abc", parser)), 3);
    EXPECT_EQ(result_getter(ParseLexeme2("x", parser)), 1);
}

TEST(ParserWithAction, StringToLengthWide)
{
    auto l = [](auto& ctx) { return ctx.GetValue().size(); };
    auto parser = str_alpha[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"hello", parser)), 5);
    EXPECT_EQ(result_getter(ParseLexeme2(L"abc", parser)), 3);
    EXPECT_EQ(result_getter(ParseLexeme2(L"x", parser)), 1);
}

TEST(ParserWithAction, CharDigitToIntNarrow)
{
    auto l = [](auto& ctx) { return static_cast<int>(ctx.GetValue() - '0'); };
    auto parser = char_digit[l];

    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2("5", parser)), 5);
    EXPECT_EQ(result_getter(ParseLexeme2("9", parser)), 9);
}

TEST(ParserWithAction, CharDigitToIntWide)
{
    auto l = [](auto& ctx) { return static_cast<int>(ctx.GetValue() - L'0'); };
    auto parser = char_digit[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"0", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2(L"5", parser)), 5);
    EXPECT_EQ(result_getter(ParseLexeme2(L"9", parser)), 9);
}

TEST(ParserWithAction, IntToBoolEvenCheckNarrow)
{
    auto l = [](auto& ctx) { return ctx.GetValue() % 2 == 0; };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), true);
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), true);
    EXPECT_EQ(result_getter(ParseLexeme2("-4", parser)), true);
    EXPECT_EQ(result_getter(ParseLexeme2("7", parser)), false);
    EXPECT_EQ(result_getter(ParseLexeme2("-3", parser)), false);
}

TEST(ParserWithAction, IntToBoolEvenCheckWide)
{
    auto l = [](auto& ctx) { return ctx.GetValue() % 2 == 0; };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"42", parser)), true);
    EXPECT_EQ(result_getter(ParseLexeme2(L"0", parser)), true);
    EXPECT_EQ(result_getter(ParseLexeme2(L"7", parser)), false);
}

TEST(ParserWithAction, IntToDoubledValueNarrow)
{
    auto l = [](auto& ctx) { return ctx.GetValue() * 2; };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("21", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2("-5", parser)), -10);
}

TEST(ParserWithAction, IntToDoubledValueWide)
{
    auto l = [](auto& ctx) { return ctx.GetValue() * 2; };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"21", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2(L"0", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2(L"-5", parser)), -10);
}

TEST(ParserWithAction, StringToUppercaseNarrow)
{
    auto l = [](auto& ctx) {
        std::string result = ctx.GetValue();
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
        };
    auto parser = str_alpha[l];

    EXPECT_EQ(result_getter(ParseLexeme2("hello", parser)), "HELLO");
    EXPECT_EQ(result_getter(ParseLexeme2("World", parser)), "WORLD");
    EXPECT_EQ(result_getter(ParseLexeme2("ABC", parser)), "ABC");
}

TEST(ParserWithAction, StringToUppercaseWide)
{
    auto l = [](auto& ctx) {
        std::wstring result = ctx.GetValue();
        std::transform(result.begin(), result.end(), result.begin(), ::towupper);
        return result;
        };
    auto parser = str_alpha[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"hello", parser)), L"HELLO");
    EXPECT_EQ(result_getter(ParseLexeme2(L"World", parser)), L"WORLD");
    EXPECT_EQ(result_getter(ParseLexeme2(L"ABC", parser)), L"ABC");
}

TEST(ParserWithAction, StringToLowercaseNarrow)
{
    auto l = [](auto& ctx) {
        std::string result = ctx.GetValue();
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
        };
    auto parser = str_alpha[l];

    EXPECT_EQ(result_getter(ParseLexeme2("HELLO", parser)), "hello");
    EXPECT_EQ(result_getter(ParseLexeme2("World", parser)), "world");
    EXPECT_EQ(result_getter(ParseLexeme2("abc", parser)), "abc");
}

TEST(ParserWithAction, UIntToSquareNarrow)
{
    auto l = [](auto& ctx) {
        auto val = ctx.GetValue();
        return val * val;
        };
    auto parser = uint_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("5", parser)), 25U);
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), 0U);
    EXPECT_EQ(result_getter(ParseLexeme2("10", parser)), 100U);
}

TEST(ParserWithAction, UIntToSquareWide)
{
    auto l = [](auto& ctx) {
        auto val = ctx.GetValue();
        return val * val;
        };
    auto parser = uint_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"5", parser)), 25U);
    EXPECT_EQ(result_getter(ParseLexeme2(L"0", parser)), 0U);
    EXPECT_EQ(result_getter(ParseLexeme2(L"10", parser)), 100U);
}

TEST(ParserWithAction, DoubleToRoundedIntNarrow)
{
    auto l = [](auto& ctx) {
        return static_cast<int>(ctx.GetValue() + 0.5);
        };
    auto parser = double_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("3.7", parser)), 4);
    EXPECT_EQ(result_getter(ParseLexeme2("3.2", parser)), 3);
    EXPECT_EQ(result_getter(ParseLexeme2("0.0", parser)), 0);
}

TEST(ParserWithAction, DoubleToRoundedIntWide)
{
    auto l = [](auto& ctx) {
        return static_cast<int>(ctx.GetValue() + 0.5);
        };
    auto parser = double_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"3.7", parser)), 4);
    EXPECT_EQ(result_getter(ParseLexeme2(L"3.2", parser)), 3);
    EXPECT_EQ(result_getter(ParseLexeme2(L"0.0", parser)), 0);
}

TEST(ParserWithAction, IntToPrefixedStringNarrow)
{
    auto l = [](auto& ctx) {
        return "value_" + std::to_string(ctx.GetValue());
        };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), "value_42");
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), "value_0");
    EXPECT_EQ(result_getter(ParseLexeme2("-7", parser)), "value_-7");
}

TEST(ParserWithAction, IntToPrefixedStringWide)
{
    auto l = [](auto& ctx) {
        return L"value_" + std::to_wstring(ctx.GetValue());
        };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"42", parser)), L"value_42");
    EXPECT_EQ(result_getter(ParseLexeme2(L"0", parser)), L"value_0");
    EXPECT_EQ(result_getter(ParseLexeme2(L"-7", parser)), L"value_-7");
}

TEST(ParserWithAction, StringToReversedNarrow)
{
    auto l = [](auto& ctx) {
        std::string result = ctx.GetValue();
        std::reverse(result.begin(), result.end());
        return result;
        };
    auto parser = str_alpha[l];

    EXPECT_EQ(result_getter(ParseLexeme2("hello", parser)), "olleh");
    EXPECT_EQ(result_getter(ParseLexeme2("abc", parser)), "cba");
    EXPECT_EQ(result_getter(ParseLexeme2("x", parser)), "x");
}

TEST(ParserWithAction, StringToReversedWide)
{
    auto l = [](auto& ctx) {
        std::wstring result = ctx.GetValue();
        std::reverse(result.begin(), result.end());
        return result;
        };
    auto parser = str_alpha[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"hello", parser)), L"olleh");
    EXPECT_EQ(result_getter(ParseLexeme2(L"abc", parser)), L"cba");
    EXPECT_EQ(result_getter(ParseLexeme2(L"x", parser)), L"x");
}

TEST(ParserWithAction, IntToAbsoluteValueNarrow)
{
    auto l = [](auto& ctx) {
        auto val = ctx.GetValue();
        return val < 0 ? -val : val;
        };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("-42", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), 0);
}

TEST(ParserWithAction, IntToAbsoluteValueWide)
{
    auto l = [](auto& ctx) {
        auto val = ctx.GetValue();
        return val < 0 ? -val : val;
        };
    auto parser = int_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"42", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2(L"-42", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2(L"0", parser)), 0);
}

TEST(ParserWithAction, DoubleToHalvedNarrow)
{
    auto l = [](auto& ctx) {
        return ctx.GetValue() / 2.0;
        };
    auto parser = double_[l];

    EXPECT_EQ(result_getter(ParseLexeme2("10.0", parser)), 5.0);
    EXPECT_EQ(result_getter(ParseLexeme2("3.14", parser)), 1.57);
    EXPECT_EQ(result_getter(ParseLexeme2("-4.0", parser)), -2.0);
}

TEST(ParserWithAction, DoubleToHalvedWide)
{
    auto l = [](auto& ctx) {
        return ctx.GetValue() / 2.0;
        };
    auto parser = double_[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"10.0", parser)), 5.0);
    EXPECT_EQ(result_getter(ParseLexeme2(L"3.14", parser)), 1.57);
    EXPECT_EQ(result_getter(ParseLexeme2(L"-4.0", parser)), -2.0);
}

TEST(ParserWithAction, RepeatIntSumNarrow)
{
    auto l = [](auto& ctx) {
        long sum = 0;
        for (auto v : ctx.GetValue()) sum += v;
        return sum;
        };
    auto parser = (*int_)[l];

    EXPECT_EQ(result_getter(ParseLexeme2("1 2 3 4", parser)), 10);
    EXPECT_EQ(result_getter(ParseLexeme2("10 20 30", parser)), 60);
    EXPECT_EQ(result_getter(ParseLexeme2("-5 5", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("", parser)), 0);
}

TEST(ParserWithAction, RepeatIntSumWide)
{
    auto l = [](auto& ctx) {
        long sum = 0;
        for (auto v : ctx.GetValue()) sum += v;
        return sum;
        };
    auto parser = (*int_)[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"1 2 3 4", parser)), 10);
    EXPECT_EQ(result_getter(ParseLexeme2(L"10 20 30", parser)), 60);
    EXPECT_EQ(result_getter(ParseLexeme2(L"-5 5", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2(L"", parser)), 0);
}

TEST(ParserWithAction, ListIntSumNarrow)
{
    auto l = [](auto& ctx) {
        long sum = 0;
        for (auto v : ctx.GetValue()) sum += v;
        return sum;
        };
    auto parser = (int_ % ",")[l];

    EXPECT_EQ(result_getter(ParseLexeme2("1, 2, 3, 4", parser)), 10);
    EXPECT_EQ(result_getter(ParseLexeme2("100, -50, 25", parser)), 75);
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("", parser)), 0);
}

TEST(ParserWithAction, ListIntSumWide)
{
    auto l = [](auto& ctx) {
        long sum = 0;
        for (auto v : ctx.GetValue()) sum += v;
        return sum;
        };
    auto parser = (int_ % L",")[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"1, 2, 3, 4", parser)), 10);
    EXPECT_EQ(result_getter(ParseLexeme2(L"100, -50, 25", parser)), 75);
    EXPECT_EQ(result_getter(ParseLexeme2(L"42", parser)), 42);
}

TEST(ParserWithAction, ListIntCountNarrow)
{
    auto l = [](auto& ctx) { return ctx.GetValue().size(); };
    auto parser = (int_ % ",")[l];

    EXPECT_EQ(result_getter(ParseLexeme2("1, 2, 3, 4", parser)), 4);
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), 1);
    EXPECT_EQ(result_getter(ParseLexeme2("", parser)), 0);
}

TEST(ParserWithAction, ListIntCountWide)
{
    auto l = [](auto& ctx) { return ctx.GetValue().size(); };
    auto parser = (int_ % L",")[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"1, 2, 3, 4", parser)), 4);
    EXPECT_EQ(result_getter(ParseLexeme2(L"42", parser)), 1);
    EXPECT_EQ(result_getter(ParseLexeme2(L"", parser)), 0);
}

TEST(ParserWithAction, ListDoubleProductNarrow)
{
    auto l = [](auto& ctx) {
        double product = 1.0;
        for (auto v : ctx.GetValue()) product *= v;
        return product;
        };
    auto parser = (double_ % ",")[l];

    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("2.0, 3.0, 4.0", parser)), 24.0);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("1.5, 2.0", parser)), 3.0);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("5.0", parser)), 5.0);
}

TEST(ParserWithAction, ListDoubleProductWide)
{
    auto l = [](auto& ctx) {
        double product = 1.0;
        for (auto v : ctx.GetValue()) product *= v;
        return product;
        };
    auto parser = (double_ % L",")[l];

    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2(L"2.0, 3.0, 4.0", parser)), 24.0);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2(L"1.5, 2.0", parser)), 3.0);
}

TEST(ParserWithAction, RepeatStringConcatNarrow)
{
    auto l = [](auto& ctx) {
        std::string result;
        for (const auto& s : ctx.GetValue()) result += s;
        return result;
        };
    auto parser = (*str_alpha)[l];

    EXPECT_EQ(result_getter(ParseLexeme2("hello world foo", parser)), "helloworldfoo");
    EXPECT_EQ(result_getter(ParseLexeme2("abc", parser)), "abc");
    EXPECT_EQ(result_getter(ParseLexeme2("", parser)), "");
}

TEST(ParserWithAction, RepeatStringConcatWide)
{
    auto l = [](auto& ctx) {
        std::wstring result;
        for (const auto& s : ctx.GetValue()) result += s;
        return result;
        };
    auto parser = (*str_alpha)[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"hello world foo", parser)), L"helloworldfoo");
    EXPECT_EQ(result_getter(ParseLexeme2(L"abc", parser)), L"abc");
    EXPECT_EQ(result_getter(ParseLexeme2(L"", parser)), L"");
}

TEST(ParserWithAction, RepeatCharDigitToStringNarrow)
{
    auto l = [](auto& ctx) {
        std::string result;
        for (auto c : ctx.GetValue()) result += c;
        return result;
        };
    auto parser = (*char_digit)[l];

    EXPECT_EQ(result_getter(ParseLexeme2("12345", parser)), "12345");
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), "0");
    EXPECT_EQ(result_getter(ParseLexeme2("", parser)), "");
}

TEST(ParserWithAction, RepeatCharDigitToStringWide)
{
    auto l = [](auto& ctx) {
        std::wstring result;
        for (auto c : ctx.GetValue()) result += c;
        return result;
        };
    auto parser = (*char_digit)[l];

    EXPECT_EQ(result_getter(ParseLexeme2(L"12345", parser)), L"12345");
    EXPECT_EQ(result_getter(ParseLexeme2(L"0", parser)), L"0");
    EXPECT_EQ(result_getter(ParseLexeme2(L"", parser)), L"");
}

TEST(ParserWithAction, ActionInsideRepeatNarrow)
{
    auto l = [](auto& ctx) { return ctx.GetValue() * 2; };
    auto parser = *(int_[l]);

    EXPECT_EQ(result_getter(ParseLexeme2("1 2 3", parser)), std::vector<long>({ 2, 4, 6 }));
    EXPECT_EQ(result_getter(ParseLexeme2("10 20", parser)), std::vector<long>({ 20, 40 }));
    EXPECT_EQ(result_getter(ParseLexeme2("", parser)), std::vector<long>({}));
}

TEST(ParserWithAction, ActionInsideRepeatWide)
{
    auto l = [](auto& ctx) { return ctx.GetValue() * 2; };
    auto parser = *(int_[l]);

    EXPECT_EQ(result_getter(ParseLexeme2(L"1 2 3", parser)), std::vector<long>({ 2, 4, 6 }));
    EXPECT_EQ(result_getter(ParseLexeme2(L"10 20", parser)), std::vector<long>({ 20, 40 }));
    EXPECT_EQ(result_getter(ParseLexeme2(L"", parser)), std::vector<long>({}));
}

TEST(ParserWithAction, ActionInsideListNarrow)
{
    auto l = [](auto& ctx) { return std::to_string(ctx.GetValue()); };
    auto parser = int_[l] % ",";

    EXPECT_EQ(result_getter(ParseLexeme2("1, 2, 3", parser)), std::vector<std::string>({ "1", "2", "3" }));
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), std::vector<std::string>({ "42" }));
    EXPECT_EQ(result_getter(ParseLexeme2("", parser)), std::vector<std::string>({}));
}

TEST(ParserWithAction, ActionInsideListWide)
{
    auto l = [](auto& ctx) { return std::to_wstring(ctx.GetValue()); };
    auto parser = int_[l] % L",";

    EXPECT_EQ(result_getter(ParseLexeme2(L"1, 2, 3", parser)), std::vector<std::wstring>({ L"1", L"2", L"3" }));
    EXPECT_EQ(result_getter(ParseLexeme2(L"42", parser)), std::vector<std::wstring>({ L"42" }));
    EXPECT_EQ(result_getter(ParseLexeme2(L"", parser)), std::vector<std::wstring>({}));
}

TEST(ParserWithAction, ActionInsideRepeatStringToUpperNarrow)
{
    auto l = [](auto& ctx) {
        std::string result = ctx.GetValue();
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
        };
    auto parser = *(str_alpha[l]);

    EXPECT_EQ(result_getter(ParseLexeme2("hello world", parser)), std::vector<std::string>({ "HELLO", "WORLD" }));
    EXPECT_EQ(result_getter(ParseLexeme2("abc", parser)), std::vector<std::string>({ "ABC" }));
}

TEST(ParserWithAction, ActionInsideRepeatStringToUpperWide)
{
    auto l = [](auto& ctx) {
        std::wstring result = ctx.GetValue();
        std::transform(result.begin(), result.end(), result.begin(), ::towupper);
        return result;
        };
    auto parser = *(str_alpha[l]);

    EXPECT_EQ(result_getter(ParseLexeme2(L"hello world", parser)), std::vector<std::wstring>({ L"HELLO", L"WORLD" }));
    EXPECT_EQ(result_getter(ParseLexeme2(L"abc", parser)), std::vector<std::wstring>({ L"ABC" }));
}
