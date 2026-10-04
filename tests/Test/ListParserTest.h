#pragma once

#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <vector>

#include <common/parsing.h>
#include "tests_utils.h"

using namespace aliases;

// ============================================================================
// ТЕСТОВЫЕ ДАННЫЕ (inline constexpr предотвращает ODR-нарушения в .h файлах)
// ============================================================================
std::string kEmptyList = "";
std::string kSimpleList = "1, 2, 3, 4";
std::string kMessySpaces = "1    ,     2                ,    3   ,           4";
std::string kTabsAndSpaces = "1\t\t,2\t\t,\t3\t\t,\t\t4";
std::string kCustomSep = "1 <::> 2\t\t<::>\t3\t\t<::>\t\t4";
std::string kLongRandomList = " -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111, 222, 33, 444, 55, -11, -22, -33, -44, 459387";
std::string kListWithInvalidMid = " -1 ,231345, -9999, 2139 ,560946456,-3333333, 823749823, 111, a ,222, 33, 444, 55, -11, -22, -33, -44, 459387";

// ============================================================================
// ОСНОВНЫЕ СЦЕНАРИИ (Basic Scenarios)
// ============================================================================

TEST(ListParser, ParsesValidCommaSeparatedInts)
{
    auto parser = int_ % ",";

    EXPECT_EQ(result_getter(ParseLexeme2(kEmptyList, parser)), lambda_identity(std::vector<long>{}));
    EXPECT_EQ(result_getter(ParseLexeme2(kSimpleList, parser)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
    EXPECT_EQ(result_getter(ParseLexeme2(kLongRandomList, parser)), lambda_identity(std::vector<long>{
        -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111, 222, 33, 444, 55, -11, -22, -33, -44, 459387
    }));
}

TEST(ListParser, HandlesExcessiveWhitespace)
{
    auto parser = int_ % ",";

    EXPECT_EQ(result_getter(ParseLexeme2(kMessySpaces, parser)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
    EXPECT_EQ(result_getter(ParseLexeme2(kTabsAndSpaces, parser)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
}

TEST(ListParser, SupportsCustomSeparators)
{
    auto parser = int_ % "<::>";

    EXPECT_EQ(result_getter(ParseLexeme2(kCustomSep, parser)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
}

// ============================================================================
// ЧАСТИЧНЫЙ УСПЕХ И ОШИБКИ (Partial Success & Errors)
// ============================================================================

TEST(ListParser, StopsParsingOnFirstInvalidElement)
{
    auto parser = int_ % ",";

    // Парсер должен успешно распарсить начало и остановиться на 'a'
    EXPECT_EQ(result_getter(ParseLexeme2(kListWithInvalidMid, parser)), lambda_identity(std::vector<long>{
        -1, 231345, -9999, 2139, 560946456, -3333333, 823749823, 111
    }));
}

TEST(ListParser, FailsGracefullyIfFirstElementIsInvalid)
{
    auto parser = int_ % ",";

    // Если первый элемент невалиден, список должен быть пустым (или вернуть ошибку, в зависимости от твоей логики)
    EXPECT_EQ(result_getter(ParseLexeme2("a, 1, 2, 3", parser)), lambda_identity(std::vector<long>{}));
    EXPECT_EQ(result_getter(ParseLexeme2(" , 1, 2", parser)), lambda_identity(std::vector<long>{}));
}

// ============================================================================
// ГРАНИЧНЫЕ СЛУЧАИ (Edge Cases)
// ============================================================================

TEST(ListParser, HandlesSingleElementList)
{
    auto parser = int_ % ",";

    // Список без разделителей вообще
    EXPECT_EQ(result_getter(ParseLexeme2("42", parser)), lambda_identity(std::vector<long>{42}));
    EXPECT_EQ(result_getter(ParseLexeme2(" -99 ", parser)), lambda_identity(std::vector<long>{-99}));
}

TEST(ListParser, HandlesTrailingSeparator)
{
    auto parser = int_ % ",";

    // Поведение зависит от твоего парсера: он может вернуть {1, 2, 3} или {1, 2, 3, 0} или ошибку.
    // Проверь, что он делает, и подставь правильный ожидаемый результат:
    EXPECT_EQ(result_getter(ParseLexeme2("1, 2, 3, ", parser)), lambda_identity(std::vector<long>{1, 2, 3}));
}

TEST(ListParser, HandlesLeadingSeparator)
{
    auto parser = int_ % ",";

    // Аналогично: проверяем, как парсер реагирует на разделитель в начале
    EXPECT_EQ(result_getter(ParseLexeme2(", 1, 2", parser)), lambda_identity(std::vector<long>{})); // Или {1, 2}, если он игнорирует начальные сепараторы
}

TEST(ListParser, HandlesConsecutiveSeparators)
{
    auto parser = int_ % ",";

    // Два разделителя подряд означают пустой элемент между ними
    EXPECT_EQ(result_getter(ParseLexeme2("1, , 2", parser)), lambda_identity(std::vector<long>{1})); // Или {1, 0, 2}, зависит от логики
    EXPECT_EQ(result_getter(ParseLexeme2("1,,2", parser)), lambda_identity(std::vector<long>{1}));
}

// ============================================================================
// ШИРОКИЕ СИМВОЛЫ (WCHAR_T SUPPORT)
// ============================================================================

TEST(ListParser, SupportsWideCharacters)
{
    auto parser_char = int_ % L",";
    auto parser_custom = int_ % L"<::>";

    EXPECT_EQ(result_getter(ParseLexeme2(L"", parser_char)), lambda_identity(std::vector<long>{}));
    EXPECT_EQ(result_getter(ParseLexeme2(L"1, 2, 3, 4", parser_char)), lambda_identity(std::vector<long>{1, 2, 3, 4}));
    EXPECT_EQ(result_getter(ParseLexeme2(L"1\t\t,2\t\t,\t3", parser_char)), lambda_identity(std::vector<long>{1, 2, 3}));
    EXPECT_EQ(result_getter(ParseLexeme2(L"1 <::> 2 <::> 3", parser_custom)), lambda_identity(std::vector<long>{1, 2, 3}));

    // Проверка частичного успеха для wchar
    EXPECT_EQ(result_getter(ParseLexeme2(L"1, 2, a, 3", parser_char)), lambda_identity(std::vector<long>{1, 2}));
}

// ============================================================================
// ДРУГИЕ ТИПЫ ДАННЫХ (GENERIC BEHAVIOR)
// ============================================================================

TEST(ListParser, WorksWithStringParser)
{
    auto parser = str_alpha % ",";

    EXPECT_EQ(result_getter(ParseLexeme2("apple, banana, cherry", parser)), lambda_identity(std::vector<std::string>{"apple", "banana", "cherry"}));
    EXPECT_EQ(result_getter(ParseLexeme2("hello, 123, world", parser)), lambda_identity(std::vector<std::string>{"hello"})); // Останавливается на " 123"
}

TEST(ListParser, WorksWithDoubleParser)
{
    auto parser = double_ % ";";

    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("1.1; 2.2; 3.3", parser))[0], 1.1);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("1.1; 2.2; 3.3", parser))[1], 2.2);
    EXPECT_DOUBLE_EQ(result_getter(ParseLexeme2("1.1; 2.2; 3.3", parser))[2], 3.3);
}