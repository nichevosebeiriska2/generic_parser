//#include "ParserBase.h"
//#include "ParserOmited.h"
//#include "ParserRepeate.h"
//#include "ParserList.h"
//#include "ParserWithAction.h"
//#include "ParserSequence.h"
//#include "ParserAlternative.h"
//#include "ParseRuleDeclaration.h"
//
//#include "Operators.h"
//
//#include "parser_aliases.h"
//#include "ExampleJson.h"
//
//#include <optional>
//#include <map>

#include "base_parser.h"
#include "alternative.h"
#include "sequence.h"
#include "list.h"
#include "repeate.h"
#include "parser_with_action.h"
#include "rule.h"


#include <fstream>

std::string read_file(std::string strPathToFile)
{
	std::ifstream file(strPathToFile);

	if (file.is_open())
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{} };

	return "";
}




//Context ctx{ Parsers::Skippers::wchar::space };
ParserWithContext<Scanners::scanner_float_ctx> parser;
ParserWithActionNew action_for_rule(parser, [](auto& ctx) { return std::string{}; });
RuleNew<class rule_returns_float, std::string> rule;

IMPLEMENT_RULE_NEW(rule, action_for_rule);

template<ConceptContext TContext>
void fContext(TContext&& ctx)
{

}

//#include "g"
#include "BaseParserTest.h"
#include "RepeateParserTest.h"
#include "ListParserTest.h"
#include "AlternativeParserTest.h"
#include "PredicateNotTest.h"

struct SJsonObject;
struct SJsonArray;

struct SJsonValue
{
	std::variant<std::monostate, bool, int, float, std::string, SJsonArray *, SJsonObject *> m_value;
};

struct SJsonObject
{
	std::map<std::string, SJsonValue> m_values;
};
struct SJsonArray
{
	std::vector<SJsonValue> m_values;
};

RuleNew<class tag_value, SJsonValue> value;
RuleNew<class tag_object, SJsonValue> object;
RuleNew<class tag_array, SJsonValue> array;

auto action_on_null = [](auto& ctx){return std::monostate{};};
auto action_on_bool = [](auto& ctx){return true;};
auto action_on_str = [](auto& ctx){
	return std::string{ctx.Begin(), ctx.End()}; 
};

auto action_on_array = [](auto &ctx)
{
	auto pArray = new SJsonArray;
	for(auto &value : std::get<1>(ctx.GetValue()))
		pArray->m_values.emplace_back(std::move(value));

	return SJsonValue{pArray};
};

auto action_on_object = [](auto &ctx)
{
	auto pObject = new SJsonObject;
	for(auto [name, unused,  value] : std::get<1>(ctx.GetValue()))
		pObject->m_values[std::move(name)] = SJsonValue{std::move(value)};
	
	return SJsonValue{pObject};
};

auto action_on_value = [](auto &ctx)
{
	return std::visit([](auto&& arg){return SJsonValue{arg};}, ctx.GetValue());
};


auto parser_null = (ParserLiteralWithContext{"null"})[action_on_null];
auto parser_bool = (ParserLiteralWithContext{"true"} |  ParserLiteralWithContext{"false"})[action_on_bool];
auto parser_int = int_;
auto parser_float = float_;
auto parser_string = ("\"" >> (*(!ParserLiteralWithContext{ "\"" } >> ("\\\"" | char_any))) >> "\"")[action_on_str];

auto parser_object = "{" >> (*((parser_string >> ":" >> value) % ",")) >> "}";
auto parser_array = "[" >> (*(value % ",")) >> "]";
auto parser_value = object | array | parser_null | parser_bool  | parser_float | parser_int | parser_string;

IMPLEMENT_RULE_NEW(object, parser_object[action_on_object]);
IMPLEMENT_RULE_NEW(array, parser_array[action_on_array]);
IMPLEMENT_RULE_NEW(value, parser_value[action_on_value]);

//#include "large.txt"

std::string small_json = R"(
{
  "dataset": {
    "name": "Comprehensive Test Dataset",
    "version": "3.5.0",
    "description": "Large dataset for stress-testing JSON parsers",
    "generated": "2026-03-01T12:00:00Z",
    "records_count": 100,
    "records": [
      {
        "id": 1,
        "type": "user",
        "data": {
          "username": "user_001",
          "email": "user001@example.com",
          "profile": {
            "first_name": "John",
            "last_name": "Doe",
            "age": 28,
            "location": {
              "country": "USA",
              "city": "New York",
              "coordinates": {
                "lat": 40.7128,
                "lng": -74.0060
              }
            },
            "preferences": {
              "theme": "dark",
              "notifications": {
                "email": true,
                "push": true,
                "sms": false
              },
              "languages": ["en", "es", "fr"]
            }
          },
          "stats": {
            "logins": 150,
            "purchases": 23,
            "reviews": 15,
            "rating": 4.7
          },
          "activity": [
            {
              "timestamp": "2026-01-15T08:30:00Z",
              "action": "login",
              "details": {
                "device": "mobile",
                "os": "iOS 16"
              }
            },
            {
              "timestamp": "2026-01-15T09:15:00Z",
              "action": "purchase",
              "details": {
                "product_id": 101,
                "amount": 79.99
              }
            },
            {
              "timestamp": "2026-01-15T10:00:00Z",
              "action": "logout",
              "details": {}
            }
          ]
        }
      },
      {
        "id": 2,
        "type": "user",
        "data": {
          "username": "user_002",
          "email": "user002@example.com",
          "profile": {
            "first_name": "Jane",
            "last_name": "Smith",
            "age": 34,
            "location": {
              "country": "Canada",
              "city": "Toronto",
              "coordinates": {
                "lat": 43.6510,
                "lng": -79.3470
              }
            },
            "preferences": {
              "theme": "light",
              "notifications": {
                "email": true,
                "push": false,
                "sms": true
              },
              "languages": ["en", "fr"]
            }
          },
          "stats": {
            "logins": 200,
            "purchases": 45,
            "reviews": 30,
            "rating": 4.8
          },
          "activity": [
            {
              "timestamp": "2026-01-16T10:00:00Z",
              "action": "login",
              "details": {
                "device": "desktop",
                "os": "Windows 11"
              }
            },
            {
              "timestamp": "2026-01-16T11:30:00Z",
              "action": "purchase",
              "details": {
                "product_id": 201,
                "amount": 45.50
              }
            }
          ]
        }
      }
    ],
    "matrix_data": [
      [1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
      [11, 12, 13, 14, 15, 16, 17, 18, 19, 20],
      [21, 22, 23, 24, 25, 26, 27, 28, 29, 30],
      [31, 32, 33, 34, 35, 36, 37, 38, 39, 40],
      [41, 42, 43, 44, 45, 46, 47, 48, 49, 50],
      [51, 52, 53, 54, 55, 56, 57, 58, 59, 60],
      [61, 62, 63, 64, 65, 66, 67, 68, 69, 70],
      [71, 72, 73, 74, 75, 76, 77, 78, 79, 80],
      [81, 82, 83, 84, 85, 86, 87, 88, 89, 90],
      [91, 92, 93, 94, 95, 96, 97, 98, 99, 100]
    ],
    "nested_arrays": [
      [[[1, 2], [3, 4]], [[5, 6], [7, 8]]],
      [[[9, 10], [11, 12]], [[13, 14], [15, 16]]]
    ],
    "mixed_types": {
      "numbers": [1, 2.5, 3.14159, -10, 0, 1e6, 1e-3],
      "strings": [
        "simple",
        "with spaces",
        "Привет, мир!",
        "🚀🚀🚀",
        "escaped: \"quotes\", \\backslash",
        "multi\nline\ntext",
        "tab\tseparated\tvalues"
      ],
      "booleans": [true, false, true, false],
      "nulls": [null, null, null],
      "mixed": [1, "two", true, null, 3.14]
    },
    "unicode_strings": [
      "English text",
      "Текст на русском",
      "中文文本",
      "日本語のテキスト",
      "한국어 텍스트",
      "النص العربي",
      "😀😃😄😁😆😅🤣😂🙂🙃😉😊😇🥰😍🤩😘😗😚😙🥲😋😛😜🤪😝🤑🤗🤭🤫🤔🤐🤨😐😑😶😏😒🙄😬🤥😌😔😪🤤😴😷🤒🤕🤢🤮🤧🥵🥶🥴😵🤯🤠🥳🥸😎🤓🧐😕😟🙁☹️😮😯😲😳🥺😦😧😨😰😥😢😭😱😖😣😞😓😩😫🥱😤😡😠🤬😈👿💀☠️👻👽👾🤖",
      "❤️🧡💛💚💙💜🖤🤍🤎💖💗💓💞💕💟💌💋💧💦💫👋👋🏻👋🏼👋🏽👋🏾👋🏿🤚🖐✋🖖👌🤌🤏✌️🤞🤟🤘🤙👈👉👆👇☝️👍👎✊👊🤛🤜👏🙌👐🤲🤝🙏✍️💅🤳💪🦾🦿🦵🦶👂🦻👃🧠🫀🫁🦷🦴👀👁👅👄💋🩸"
    ],
    "edge_cases": {
      "empty_string": "",
      "zero": 0,
      "negative_zero": -0,
      "large_number": 9223372036854775807,
      "very_small": 1e-100,
      "scientific": 1.23e+10,
      "true_value": true,
      "false_value": false,
      "null_value": null,
      "empty_array": [],
      "empty_object": {},
      "single_item_array": [42],
      "single_key_object": {"key": "value"}
    },
    "deeply_nested": {
      "level_1": {
        "level_2": {
          "level_3": {
            "level_4": {
              "level_5": {
                "value": "deeply nested"
              }
            }
          }
        }
      }
    },
    "repeated_patterns": [
      {"id": 1, "name": "Item 1", "value": 100},
      {"id": 2, "name": "Item 2", "value": 200},
      {"id": 3, "name": "Item 3", "value": 300},
      {"id": 4, "name": "Item 4", "value": 400},
      {"id": 5, "name": "Item 5", "value": 500},
      {"id": 6, "name": "Item 6", "value": 600},
      {"id": 7, "name": "Item 7", "value": 700},
      {"id": 8, "name": "Item 8", "value": 800},
      {"id": 9, "name": "Item 9", "value": 900},
      {"id": 10, "name": "Item 10", "value": 1000}
    ],
    "metadata": {
      "test_suite": "JSON Parser Stress Test",
      "version": "1.0.0",
      "author": "Automated Generator",
      "purpose": "Comprehensive testing of JSON parser capabilities",
      "features_tested": [
        "basic types",
        "nested objects",
        "arrays",
        "unicode",
        "escaping",
        "scientific notation",
        "edge cases",
        "deep nesting",
        "large datasets",
        "mixed types",
        "null values",
        "boolean values",
        "empty structures"
      ],
      "notes": "This file is intentionally large to test parser performance and memory handling. It includes various edge cases and different data patterns to ensure robustness."
    }
  }
}
)";
int main(int argc, char** argv)
{
	auto [status, res] = ParseLexeme2(small_json, value);
	//::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
    int a = 1;
}