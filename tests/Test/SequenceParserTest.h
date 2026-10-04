#pragma once

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <tuple>

#include <common/parsing.h>
#include "tests_utils.h"

using namespace aliases;

TEST(SequenceParser, SimpleSequenceNarrow)
{
    auto parser = int_ >> double_ >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2("42 3.14 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("42 3.14", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2("42 hello", parser)), false);
}

TEST(SequenceParser, SimpleSequenceWide)
{
    auto parser = int_ >> double_ >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 3.14 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 3.14", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 hello", parser)), false);
}

TEST(SequenceParser, SequenceWithRepeatNarrow)
{
    auto parser = int_ >> *double_ >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2("42 1.1 2.2 3.3 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("42 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("42 1.1 hello", parser)), true);
}

TEST(SequenceParser, SequenceWithRepeatWide)
{
    auto parser = int_ >> *double_ >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 1.1 2.2 3.3 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 1.1 hello", parser)), true);
}

TEST(SequenceParser, SequenceWithListNarrow)
{
    auto parser = int_ >> (double_ % ",") >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2("42 1.1, 2.2, 3.3 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("42 1.1 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("42 hello", parser)), false);
}

TEST(SequenceParser, SequenceWithListWide)
{
    auto parser = int_ >> (double_ % L",") >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 1.1, 2.2, 3.3 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 1.1 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 hello", parser)), false);
}

TEST(SequenceParser, KeyValueStructureNarrow)
{
    auto parser = str_alpha >> "=" >> int_;
    EXPECT_EQ(status_getter(ParseLexeme2("count=42", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("count = 42", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("count 42", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2("42=count", parser)), false);
}

TEST(SequenceParser, KeyValueStructureWide)
{
    auto parser = str_alpha >> L"=" >> int_;
    EXPECT_EQ(status_getter(ParseLexeme2(L"count=42", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"count = 42", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"count 42", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42=count", parser)), false);
}

TEST(SequenceParser, ArithmeticExpressionNarrow)
{
    auto parser = int_ >> char_any >> int_;
    EXPECT_EQ(status_getter(ParseLexeme2("5+3", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("10-7", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("2*4", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("5 + 3", parser)), true);
}

TEST(SequenceParser, ArithmeticExpressionWide)
{
    auto parser = int_ >> char_any >> int_;
    EXPECT_EQ(status_getter(ParseLexeme2(L"5+3", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"10-7", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"2*4", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"5 + 3", parser)), true);
}

TEST(SequenceParser, CoordinateStructureNarrow)
{
    auto parser = "(" >> double_ >> "," >> double_ >> ")";
    EXPECT_EQ(status_getter(ParseLexeme2("(3.14, 2.71)", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("( 3.14 , 2.71 )", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("(3.14 2.71)", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2("3.14, 2.71", parser)), false);
}

TEST(SequenceParser, CoordinateStructureWide)
{
    auto parser = L"(" >> double_ >> L"," >> double_ >> L")";
    EXPECT_EQ(status_getter(ParseLexeme2(L"(3.14, 2.71)", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"( 3.14 , 2.71 )", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"(3.14 2.71)", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"3.14, 2.71", parser)), false);
}

TEST(SequenceParser, MultipleListsNarrow)
{
    auto parser = (int_ % ",") >> ";" >> (double_ % ",");
    EXPECT_EQ(status_getter(ParseLexeme2("1, 2, 3; 1.1, 2.2, 3.3", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("1; 1.1", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("1, 2; 1.1, 2.2", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("1, 2, 3 1.1, 2.2", parser)), false);
}

TEST(SequenceParser, MultipleListsWide)
{
    auto parser = (int_ % L",") >> L";" >> (double_ % L",");
    EXPECT_EQ(status_getter(ParseLexeme2(L"1, 2, 3; 1.1, 2.2, 3.3", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"1; 1.1", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"1, 2; 1.1, 2.2", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"1, 2, 3 1.1, 2.2", parser)), false);
}

TEST(SequenceParser, NestedBracketsNarrow)
{
    auto parser = "[" >> int_ >> "," >> "[" >> double_ >> "," >> double_ >> "]" >> "]";
    EXPECT_EQ(status_getter(ParseLexeme2("[42, [1.1, 2.2]]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("[ 42 , [ 1.1 , 2.2 ] ]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("[42, 1.1, 2.2]", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2("[[42], [1.1, 2.2]]", parser)), false);
}

TEST(SequenceParser, NestedBracketsWide)
{
    auto parser = L"[" >> int_ >> L"," >> L"[" >> double_ >> L"," >> double_ >> L"]" >> L"]";
    EXPECT_EQ(status_getter(ParseLexeme2(L"[42, [1.1, 2.2]]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"[ 42 , [ 1.1 , 2.2 ] ]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"[42, 1.1, 2.2]", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"[[42], [1.1, 2.2]]", parser)), false);
}

TEST(SequenceParser, IdentifierWithNumbersNarrow)
{
    auto parser = str_alpha >> *char_digit;
    EXPECT_EQ(status_getter(ParseLexeme2("var123", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("x", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("test456", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("123var", parser)), false);
}

TEST(SequenceParser, IdentifierWithNumbersWide)
{
    auto parser = str_alpha >> *char_digit;
    EXPECT_EQ(status_getter(ParseLexeme2(L"var123", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"x", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"test456", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"123var", parser)), false);
}

TEST(SequenceParser, DateLikeStructureNarrow)
{
    auto parser = int_ >> "/" >> int_ >> "/" >> int_;
    EXPECT_EQ(status_getter(ParseLexeme2("2024/01/15", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("2024 / 01 / 15", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("2024-01-15", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2("2024/01", parser)), false);
}

TEST(SequenceParser, DateLikeStructureWide)
{
    auto parser = int_ >> L"/" >> int_ >> L"/" >> int_;
    EXPECT_EQ(status_getter(ParseLexeme2(L"2024/01/15", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"2024 / 01 / 15", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"2024-01-15", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"2024/01", parser)), false);
}

TEST(SequenceParser, MixedTypesSequenceNarrow)
{
    auto parser = char_alpha >> int_ >> double_ >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2("a42 3.14 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("Z 100 2.718 world", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("42 3.14 hello", parser)), false);
}

TEST(SequenceParser, MixedTypesSequenceWide)
{
    auto parser = char_alpha >> int_ >> double_ >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2(L"a42 3.14 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"Z 100 2.718 world", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 3.14 hello", parser)), false);
}

TEST(SequenceParser, RepeatThenListNarrow)
{
    auto parser = *int_ >> (str_alpha % ",");
    EXPECT_EQ(status_getter(ParseLexeme2("1 2 3 apple, banana, cherry", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("apple, banana", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("1 2 3", parser)), false);
}

TEST(SequenceParser, RepeatThenListWide)
{
    auto parser = *int_ >> (str_alpha % L",");
    EXPECT_EQ(status_getter(ParseLexeme2(L"1 2 3 apple, banana, cherry", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"apple, banana", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"1 2 3", parser)), false);
}

TEST(SequenceParser, ListThenRepeatNarrow)
{
    auto parser = (int_ % ",") >> *double_;
    EXPECT_EQ(status_getter(ParseLexeme2("1, 2, 3 1.1 2.2 3.3", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("1, 2, 3", parser)), true);
}

TEST(SequenceParser, ListThenRepeatWide)
{
    auto parser = (int_ % L",") >> *double_;
    EXPECT_EQ(status_getter(ParseLexeme2(L"1, 2, 3 1.1 2.2 3.3", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"1, 2, 3", parser)), true);
}

TEST(SequenceParser, ComplexNestedStructureNarrow)
{
    auto parser = str_alpha >> "=" >> "[" >> (int_ % ",") >> "]";
    EXPECT_EQ(status_getter(ParseLexeme2("data=[1, 2, 3]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("data = [ 1 , 2 , 3 ]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("data=[1]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("data=1, 2, 3", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2("data=[1, 2, 3", parser)), false);
}

TEST(SequenceParser, ComplexNestedStructureWide)
{
    auto parser = str_alpha >> L"=" >> L"[" >> (int_ % L",") >> L"]";
    EXPECT_EQ(status_getter(ParseLexeme2(L"data=[1, 2, 3]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"data = [ 1 , 2 , 3 ]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"data=[1]", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"data=1, 2, 3", parser)), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"data=[1, 2, 3", parser)), false);
}

TEST(SequenceParser, EdgeCaseEmptyRepeatNarrow)
{
    auto parser = int_ >> *double_ >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2("42 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("42  hello", parser)), true);
}

TEST(SequenceParser, EdgeCaseEmptyRepeatWide)
{
    auto parser = int_ >> *double_ >> str_alpha;
    EXPECT_EQ(status_getter(ParseLexeme2(L"42 hello", parser)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"42  hello", parser)), true);
}