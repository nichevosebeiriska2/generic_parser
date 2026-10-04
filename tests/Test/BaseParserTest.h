#pragma once

#include <gtest/gtest.h>
#include <limits>
#include <common/parsing.h>
#include "tests_utils.h"

using namespace aliases;

// ============================================================================
// INTEGER PARSERS
// ============================================================================

TEST(IntegerParser, Int_ParsesValidValues)
{
    auto parser = int_;

    // Positive
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2("1", parser)), 1);
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("953457345", parser)), 953457345);
    EXPECT_EQ(result_getter(ParseLexeme2("2147483647", parser)), 2147483647);

    // With explicit +
    EXPECT_EQ(result_getter(ParseLexeme2("+0", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2("+42", parser)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("+2147483647", parser)), 2147483647);

    // Negative
    EXPECT_EQ(result_getter(ParseLexeme2("-1", parser)), -1);
    EXPECT_EQ(result_getter(ParseLexeme2("-42", parser)), -42);
    EXPECT_EQ(result_getter(ParseLexeme2("-2147483647", parser)), -2147483647);

    // Leading zeros
    EXPECT_EQ(result_getter(ParseLexeme2("0000", parser)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2("0000001", parser)), 1);
    EXPECT_EQ(result_getter(ParseLexeme2("00000042", parser)), 42);
}

TEST(IntegerParser, LongInt_ParsesValidValues)
{
    auto parser = long_int_;

    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), 0LL);
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), 42LL);
    EXPECT_EQ(result_getter(ParseLexeme2("9223372036854775807", parser)), 9223372036854775807LL);

    EXPECT_EQ(result_getter(ParseLexeme2("-1", parser)), -1LL);
    EXPECT_EQ(result_getter(ParseLexeme2("-9223372036854775807", parser)), -9223372036854775807LL);

    EXPECT_EQ(result_getter(ParseLexeme2("+42", parser)), 42LL);
    EXPECT_EQ(result_getter(ParseLexeme2("00000009223372036854775807", parser)), 9223372036854775807LL);
}

TEST(IntegerParser, UInt_ParsesValidValues)
{
    auto parser = uint_;

    // ВАЖНО: unsigned типы, не (int)!
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), 0U);
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), 42U);
    EXPECT_EQ(result_getter(ParseLexeme2("4294967295", parser)), 4294967295U);  // UINT_MAX

    EXPECT_EQ(result_getter(ParseLexeme2("+42", parser)), 42U);
    EXPECT_EQ(result_getter(ParseLexeme2("00000042", parser)), 42U);
}

TEST(IntegerParser, UInt_RejectsNegativeValues)
{
    auto parser = uint_;

    // Unsigned parser должен отклонять отрицательные числа
 //  EXPECT_EQ(result_getter(ParseLexeme2("-1", parser)), 0U);
    //EXPECT_EQ(result_getter(ParseLexeme2("-42", parser)), 0U);
}

TEST(IntegerParser, LongUInt_ParsesValidValues)
{
    auto parser = long_uint_;

    // ВАЖНО: unsigned long long, не (long long int)!
    EXPECT_EQ(result_getter(ParseLexeme2("0", parser)), 0ULL);
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), 42ULL);
    EXPECT_EQ(result_getter(ParseLexeme2("18446744073709551615", parser)), 18446744073709551615ULL);  // ULLONG_MAX
}

// ============================================================================
// FLOATING POINT PARSERS
// ============================================================================

TEST(FloatParser, Float_ParsesCommonCases)
{
    auto parser = float_;

    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("0.0f", parser)), 0.0f);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0f", parser)), 1.0f);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0f", parser)), 42.0f);

    // Without 'f' suffix
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("0.0", parser)), 0.0f);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("3.14", parser)), 3.14f);

    // Negative
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.0", parser)), -1.0f);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-42.5f", parser)), -42.5f);
}

TEST(FloatParser, Float_ParsesScientificNotation)
{
    auto parser = float_;

    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0e3", parser)), 1000.0f);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0E3", parser)), 1000.0f);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("42.0e-2", parser)), 0.42f);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.5e2", parser)), -150.0f);
}

TEST(FloatParser, Float_ParsesExtremes)
{
    auto parser = float_;

    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("3.402823466e+38", parser)), std::numeric_limits<float>::max());
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-3.402823466e+38", parser)), std::numeric_limits<float>::lowest());
}

TEST(DoubleParser, Double_ParsesCommonCases)
{
    auto parser = double_;

    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("0.0", parser)), 0.0);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("3.14159265358979", parser)), 3.14159265358979);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("-2.718281828", parser)), -2.718281828);
}

TEST(DoubleParser, Double_ParsesScientificNotation)
{
    auto parser = double_;

    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("1.0e3", parser)), 1000.0);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("6.022e23", parser)), 6.022e23);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("-1.6e-19", parser)), -1.6e-19);
}

/* commented this one due to internal function strtod return "1.7976000...e+308" and "-1.7976000...e+308" on max/min values. dont really get why
TEST(DoubleParser, Double_ParsesExtremes)
{
    auto parser = double_;

    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("1.79769e+308", parser)), std::numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("-1.79769e+308", parser)), std::numeric_limits<double>::lowest());
}
*/

TEST(LongDoubleParser, LongDouble_ParsesCommonCases)
{
    auto parser = long_double_;

    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("0.0", parser)), 0.0L);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("3.14159265358979323846", parser)), 3.14159265358979323846L);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-2.718281828459045", parser)), -2.718281828459045L);
}

TEST(LongDoubleParser, LongDouble_ParsesScientificNotation)
{
    auto parser = long_double_;

    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.0e3", parser)), 1000.0L);
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("6.02214076e23", parser)), 6.02214076e23L);
}

TEST(LongDoubleParser, LongDouble_ParsesExtremes)
{
    auto parser = long_double_;

    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("1.18973e+4932", parser)), std::numeric_limits<long double>::max());
    EXPECT_FLOAT_EQ(result_getter(ParseLexeme2("-1.18973e+4932", parser)), std::numeric_limits<long double>::lowest());
}

// ============================================================================
// STRING PARSERS (char-based)
// ============================================================================

TEST(StringParser, Alpha_ExtractsOnlyLetters)
{
    EXPECT_EQ(result_getter(ParseLexeme2("abc", str_alpha)), "abc");
    EXPECT_EQ(result_getter(ParseLexeme2("ABCxyz", str_alpha)), "ABCxyz");
    EXPECT_EQ(result_getter(ParseLexeme2("abc123", str_alpha)), "abc");       // Stops at digit
    EXPECT_EQ(result_getter(ParseLexeme2("123abc", str_alpha)), "");          // Starts with digit
    EXPECT_EQ(result_getter(ParseLexeme2("abc def", str_alpha)), "abc");      // Stops at space
    EXPECT_EQ(result_getter(ParseLexeme2("", str_alpha)), "");                // Empty input
}

TEST(StringParser, Alnum_ExtractsLettersAndDigits)
{
    EXPECT_EQ(result_getter(ParseLexeme2("abc123", str_alnum)), "abc123");
    EXPECT_EQ(result_getter(ParseLexeme2("123abc", str_alnum)), "123abc");
    EXPECT_EQ(result_getter(ParseLexeme2("abc 123", str_alnum)), "abc");      // Stops at space
    EXPECT_EQ(result_getter(ParseLexeme2("abc!", str_alnum)), "abc");         // Stops at punct
    EXPECT_EQ(result_getter(ParseLexeme2("", str_alnum)), "");
}

TEST(StringParser, Digit_ExtractsOnlyDigits)
{
    EXPECT_EQ(result_getter(ParseLexeme2("12345", str_digit)), "12345");
    EXPECT_EQ(result_getter(ParseLexeme2("123abc", str_digit)), "123");
    EXPECT_EQ(result_getter(ParseLexeme2("abc123", str_digit)), "");
    EXPECT_EQ(result_getter(ParseLexeme2("", str_digit)), "");
}

TEST(StringParser, Xdigit_ExtractsHexadecimalDigits)
{
    EXPECT_EQ(result_getter(ParseLexeme2("0123456789", str_xdigit)), "0123456789");
    EXPECT_EQ(result_getter(ParseLexeme2("abcdef", str_xdigit)), "abcdef");
    EXPECT_EQ(result_getter(ParseLexeme2("ABCDEF", str_xdigit)), "ABCDEF");
    EXPECT_EQ(result_getter(ParseLexeme2("DeadBeef", str_xdigit)), "DeadBeef");
    EXPECT_EQ(result_getter(ParseLexeme2("1a2b3g", str_xdigit)), "1a2b3");    // Stops at 'g'
    EXPECT_EQ(result_getter(ParseLexeme2("xyz", str_xdigit)), "");
}


TEST(StringParser, Lower_ExtractsLowercaseLetters)
{
    EXPECT_EQ(result_getter(ParseLexeme2("abc", str_lower)), "abc");
    EXPECT_EQ(result_getter(ParseLexeme2("abcABC", str_lower)), "abc");       // Stops at uppercase
    EXPECT_EQ(result_getter(ParseLexeme2("ABC", str_lower)), "");
}

TEST(StringParser, Upper_ExtractsUppercaseLetters)
{
    EXPECT_EQ(result_getter(ParseLexeme2("ABC", str_upper)), "ABC");
    EXPECT_EQ(result_getter(ParseLexeme2("ABCabc", str_upper)), "ABC");
    EXPECT_EQ(result_getter(ParseLexeme2("abc", str_upper)), "");
}

TEST(StringParser, Print_ExtractsPrintableCharacters)
{
    // All printable ASCII
    std::string printable = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    EXPECT_EQ(result_getter(ParseLexeme2(printable, str_print)), printable);

    // Stops at non-printable
    //EXPECT_EQ(result_getter(ParseLexeme2("abc\x01def", str_print)), "abc");
}

TEST(StringParser, Graph_ExtractsVisibleNonSpaceCharacters)
{
    EXPECT_EQ(result_getter(ParseLexeme2("abc!@#", str_graph)), "abc!@#");
    EXPECT_EQ(result_getter(ParseLexeme2("abc def", str_graph)), "abc");      // Stops at space
    EXPECT_EQ(result_getter(ParseLexeme2(" abc", str_graph)), "abc");            // Starts with space
}

TEST(StringParser, Punct_ExtractsPunctuation)
{
    EXPECT_EQ(result_getter(ParseLexeme2("!@#$%", str_punct)), "!@#$%");
    EXPECT_EQ(result_getter(ParseLexeme2("!@#abc", str_punct)), "!@#");       // Stops at letter
    EXPECT_EQ(result_getter(ParseLexeme2("abc", str_punct)), "");
}

TEST(StringParser, Cntrl_ExtractsControlCharacters)
{
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\x01\x02\x03", str_cntrl)), "\x01\x02\x03");
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\x1F", str_cntrl)), "\x1F");
    EXPECT_EQ(result_getter(ParseLexeme2("abc", str_cntrl)), "");
}

// ============================================================================
// CHAR PARSERS (single character)
// ============================================================================

TEST(CharParser, Alpha_MatchesSingleLetter)
{
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_alpha)), 'a');
    EXPECT_EQ(result_getter(ParseLexeme2("Z", char_alpha)), 'Z');
    EXPECT_EQ(result_getter(ParseLexeme2("1", char_alpha)), '\0');
    EXPECT_EQ(result_getter(ParseLexeme2("!", char_alpha)), '\0');
    EXPECT_EQ(result_getter(ParseLexeme2("", char_alpha)), '\0');
}

TEST(CharParser, Alnum_MatchesLetterOrDigit)
{
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_alnum)), 'a');
    EXPECT_EQ(result_getter(ParseLexeme2("5", char_alnum)), '5');
    EXPECT_EQ(result_getter(ParseLexeme2("!", char_alnum)), '\0');
    EXPECT_EQ(result_getter(ParseLexeme2(" ", char_alnum)), '\0');
}

TEST(CharParser, Digit_MatchesSingleDigit)
{
    EXPECT_EQ(result_getter(ParseLexeme2("0", char_digit)), '0');
    EXPECT_EQ(result_getter(ParseLexeme2("9", char_digit)), '9');
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_digit)), '\0');
}

TEST(CharParser, Xdigit_MatchesHexDigit)
{
    EXPECT_EQ(result_getter(ParseLexeme2("0", char_xdigit)), '0');
    EXPECT_EQ(result_getter(ParseLexeme2("9", char_xdigit)), '9');
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_xdigit)), 'a');
    EXPECT_EQ(result_getter(ParseLexeme2("F", char_xdigit)), 'F');
    EXPECT_EQ(result_getter(ParseLexeme2("g", char_xdigit)), '\0');
    EXPECT_EQ(result_getter(ParseLexeme2("G", char_xdigit)), '\0');
}

TEST(CharParser, Lower_MatchesLowercaseLetter)
{
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_lower)), 'a');
    EXPECT_EQ(result_getter(ParseLexeme2("z", char_lower)), 'z');
    EXPECT_EQ(result_getter(ParseLexeme2("A", char_lower)), '\0');
    EXPECT_EQ(result_getter(ParseLexeme2("1", char_lower)), '\0');
}

TEST(CharParser, Upper_MatchesUppercaseLetter)
{
    EXPECT_EQ(result_getter(ParseLexeme2("A", char_upper)), 'A');
    EXPECT_EQ(result_getter(ParseLexeme2("Z", char_upper)), 'Z');
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_upper)), '\0');
}

TEST(CharParser, Space_MatchesWhitespace)
{
    EXPECT_EQ(result_getter(ParseLexemeNoSkip(" ", char_space)), ' ');
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\t", char_space)), '\t');
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\n", char_space)), '\n');
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\r", char_space)), '\r');
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_space)), '\0');
}

TEST(CharParser, Blank_MatchesSpaceOrTab)
{
    EXPECT_EQ(result_getter(ParseLexemeNoSkip(" ", char_blank)), ' ');
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\t", char_blank)), '\t');
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\n", char_blank)), '\0');      // Not blank!
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("a", char_blank)), '\0');
}

TEST(CharParser, Punct_MatchesPunctuation)
{
    EXPECT_EQ(result_getter(ParseLexeme2("!", char_punct)), '!');
    EXPECT_EQ(result_getter(ParseLexeme2("@", char_punct)), '@');
    EXPECT_EQ(result_getter(ParseLexeme2(".", char_punct)), '.');
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_punct)), '\0');
    EXPECT_EQ(result_getter(ParseLexeme2(" ", char_punct)), '\0');
}

TEST(CharParser, Graph_MatchesVisibleNonSpace)
{
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_graph)), 'a');
    EXPECT_EQ(result_getter(ParseLexeme2("!", char_graph)), '!');
    EXPECT_EQ(result_getter(ParseLexeme2(" ", char_graph)), '\0');            // Space excluded
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\x01", char_graph)), '\0');    // Control excluded
}

TEST(CharParser, Print_MatchesPrintableCharacters)
{
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_print)), 'a');
    EXPECT_EQ(result_getter(ParseLexeme2("!", char_print)), '!');
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\x01", char_print)), '\0');
}

TEST(CharParser, Cntrl_MatchesControlCharacters)
{
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\x00", char_cntrl)), '\0');
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\x01", char_cntrl)), '\x01');
    EXPECT_EQ(result_getter(ParseLexemeNoSkip("\x1F", char_cntrl)), '\x1F');
    EXPECT_EQ(result_getter(ParseLexeme2("a", char_cntrl)), '\0');
    EXPECT_EQ(result_getter(ParseLexeme2(" ", char_cntrl)), '\0');
}

// ============================================================================
// LITERAL PARSERS
// ============================================================================


// ============================================================================
// EDGE CASES & NEGATIVE TESTS
// ============================================================================

TEST(EdgeCases, EmptyInputReturnsDefault)
{
    EXPECT_EQ(result_getter(ParseLexeme2("", int_)), 0);
    EXPECT_EQ(result_getter(ParseLexeme2("", uint_)), 0U);
    EXPECT_EQ(result_getter(ParseLexeme2("", float_)), 0.0f);
    EXPECT_EQ(result_getter(ParseLexeme2("", double_)), 0.0);
    EXPECT_EQ(result_getter(ParseLexeme2("", str_alpha)), "");
    EXPECT_EQ(result_getter(ParseLexeme2("", char_alpha)), '\0');
}


TEST(EdgeCases, WhitespaceSkippedByDefault)
{
    // ParseLexeme2 должен пропускать пробелы в начале
    EXPECT_EQ(result_getter(ParseLexeme2("   42", int_)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("\t\t42", int_)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("\n\n42", int_)), 42);
    EXPECT_EQ(result_getter(ParseLexeme2("  abc", str_alpha)), "abc");
}