#pragma once

#include <gtest/gtest.h>
#include <string>

#include <common/parsing.h>
#include "tests_utils.h"

using namespace aliases;

TEST(PredicateNotParser, BasicNegationNarrow)
{
    EXPECT_EQ(status_getter(ParseLexeme2("1", !char_alpha)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("a", !char_alpha)), false);

    EXPECT_EQ(status_getter(ParseLexeme2("1", !char_digit)), false);
    EXPECT_EQ(status_getter(ParseLexeme2("a", !char_digit)), true);

    EXPECT_EQ(status_getter(ParseLexeme2("#", !char_print)), false);
    EXPECT_EQ(status_getter(ParseLexeme2("\t", !char_print)), true);
}

TEST(PredicateNotParser, NegationWithCombinatorsNarrow)
{
    EXPECT_EQ(status_getter(ParseLexeme2("[1]", !("[" >> char_digit >> "]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2("[a]", !("[" >> char_digit >> "]"))), true);

    EXPECT_EQ(status_getter(ParseLexeme2("[1,2,3]", !("[" >> char_digit % "," >> "]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2("[a,b,c]", !("[" >> char_digit % "," >> "]"))), true);

    EXPECT_EQ(status_getter(ParseLexeme2("[1 2 3]", !("[" >> *char_digit >> "]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2("[a b c]", !("[" >> *char_digit >> "]"))), true);

    EXPECT_EQ(status_getter(ParseLexeme2("[a]", !("[" >> (char_digit | char_alpha) >> "]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2("[1]", !("[" >> (char_digit | char_alpha) >> "]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2("[#]", !("[" >> (char_digit | char_alpha) >> "]"))), true);
}


TEST(PredicateNotParser, BasicNegationWide)
{
    EXPECT_EQ(status_getter(ParseLexeme2(L"1", !char_alpha)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"a", !char_alpha)), false);

    EXPECT_EQ(status_getter(ParseLexeme2(L"1", !char_digit)), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"a", !char_digit)), true);

    EXPECT_EQ(status_getter(ParseLexeme2(L"#", !char_print)), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"\t", !char_print)), true);
}

TEST(PredicateNotParser, NegationWithCombinatorsWide)
{
    EXPECT_EQ(status_getter(ParseLexeme2(L"[1]", !(L"[" >> char_digit >> L"]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"[a]", !(L"[" >> char_digit >> L"]"))), true);

    EXPECT_EQ(status_getter(ParseLexeme2(L"[1,2,3]", !(L"[" >> char_digit % L"," >> L"]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"[a,b,c]", !(L"[" >> char_digit % L"," >> L"]"))), true);

    EXPECT_EQ(status_getter(ParseLexeme2(L"[1 2 3]", !(L"[" >> *char_digit >> L"]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"[a b c]", !(L"[" >> *char_digit >> L"]"))), true);

    EXPECT_EQ(status_getter(ParseLexeme2(L"[a]", !(L"[" >> (char_digit | char_alpha) >> L"]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"[1]", !(L"[" >> (char_digit | char_alpha) >> L"]"))), false);
    EXPECT_EQ(status_getter(ParseLexeme2(L"[#]", !(L"[" >> (char_digit | char_alpha) >> L"]"))), true);
}

TEST(PredicateNotParser, EdgeCasesNarrow)
{
    EXPECT_EQ(status_getter(ParseLexeme2("", !char_alpha)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("a", char_alpha >> !char_alpha)), true);
    EXPECT_EQ(status_getter(ParseLexeme2("ab", char_alpha >> !char_alpha)), false);
}

TEST(PredicateNotParser, EdgeCasesWide)
{
    EXPECT_EQ(status_getter(ParseLexeme2(L"", !char_alpha)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"a", char_alpha >> !char_alpha)), true);
    EXPECT_EQ(status_getter(ParseLexeme2(L"ab", char_alpha >> !char_alpha)), false);
}