#pragma once

#include "Operators.h"
#include "ParseRuleDeclaration.h"

using namespace Parsers;

struct SFactor
{
	float m_value;
};

struct STerm
{

};

struct SExpression
{

};

ParseRule<class tag_factor, SFactor>	factor;
ParseRule<class tag_term, SFactor>		term;
ParseRule<class tag_expr, SFactor>		expression;

auto parser_number = (_float{} | _int{}) ([](auto&& arg_number){return arg_number;});

auto parser_factor = (parser_number 
											| ("-" >> parser_number) ([](auto &&tup)
																								{
																									return -(std::visit([](auto&& some_number){return some_number;}, std::get<0>(tup)));
																								})
											| ("(" >> expression >> ")")([](auto && arg){return arg;})
);

//auto parser_term = parser_number | ;
//
//auto parser_minus = _string_lit{"-"}([](){return 1;});
//auto parser_plus = _string_lit{"+"}([](){return 1;});
//auto parser_div = _string_lit{"/"}([](){return 1;});
//auto parser_mult = _string_lit{"*"}([](){return 1;});