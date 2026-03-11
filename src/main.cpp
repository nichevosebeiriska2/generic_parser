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
auto parser_string = ("\"" >> str_alpha[action_on_str] >> "\"")[action_on_str];

auto parser_object = "{" >> ((parser_string >> ":" >> value) % ",") >> "}";
auto parser_array = "[" >> (value % ",") >> "]";
auto parser_value = object | array | parser_null | parser_bool | parser_int | parser_float | parser_string;

IMPLEMENT_RULE_NEW(object, parser_object[action_on_object]);
IMPLEMENT_RULE_NEW(array, parser_array[action_on_array]);
IMPLEMENT_RULE_NEW(value, parser_value[action_on_value]);

//#include "large.txt"

std::string small_json = R"(
{
  "name" : "Test Document"
}
)";
int main(int argc, char** argv)
{
	auto [status, res] = ParseLexeme2(small_json, value);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}