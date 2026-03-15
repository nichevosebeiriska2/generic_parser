#pragma once

#include "parsing.h"


struct SFactor{ float m_value;};
struct STerm : public SFactor {};
struct SExpression : public SFactor {};

RuleNew<class tag_factor, SFactor>			factor;
RuleNew<class tag_term, STerm>				term;
RuleNew<class tag_term_tail, STerm>			term_tail;
RuleNew<class tag_expr, SExpression>		expression;
RuleNew<class tag_expr_tail, SExpression>	expression_tail;

auto parser_number = aliases::float_;
auto action_on_term_multiplied_with_factor	= [](auto& ctx){
	const auto &term_value = std::get<0>(ctx.GetValue()).m_value;
	const auto &factor_value = std::get<2>(ctx.GetValue()).m_value;
	return STerm{term_value * factor_value};
};

auto action_on_term_divided_with_factor			= [](auto& ctx){
	
	const auto &term_value = std::get<0>(ctx.GetValue()).m_value;
	const auto &factor_value = std::get<2>(ctx.GetValue()).m_value;
	return STerm{term_value / factor_value};
};
auto action_factor_to_term									= [](auto ctx){
	return STerm{ctx.GetValue()};
};

auto action_expr_parser_result_to_term			= [](auto& ctx)->SExpression 
{
	return SExpression{ std::visit([](auto && arg){return arg.m_value;}, ctx.GetValue())}; 
};
auto action_term_parser_result_to_term			= [](auto& ctx)->STerm {
	return STerm{ std::visit([](auto && arg){return arg.m_value;}, ctx.GetValue())};
};

auto action_on_factor_in_brackets	= [](auto& ctx){return SFactor{std::get<1>(ctx.GetValue()).m_value}; }; // "(" factor ")" -> factor
auto action_of_factor = [](auto& ctx){
	return std::visit([](auto&& f){return SFactor{f};}, ctx.GetValue()); 
}; //

auto parser_factor = (
	aliases::float_  | ("(" >> expression >> ")")[action_on_factor_in_brackets]
)[action_of_factor];

auto on_end_of_string = [](auto& ctx) {return SExpression{ 0 }; };
auto term_plus_tail = [](auto& ctx) {
	return SExpression{ std::get<1>(ctx.GetValue()).m_value + std::get<2>(ctx.GetValue()).m_value };
};
auto term_minus_tail = [](auto& ctx) {
	return SExpression{ -std::get<1>(ctx.GetValue()).m_value + std::get<2>(ctx.GetValue()).m_value };
	};

auto term_mult_termtail = [](auto& ctx) {
	return SExpression{ std::get<1>(ctx.GetValue()).m_value * std::get<2>(ctx.GetValue()).m_value };
	};

auto term_div_termtail = [](auto& ctx) {
	return SExpression{ 1 / (std::get<1>(ctx.GetValue()).m_value * std::get<2>(ctx.GetValue()).m_value) };
	};
// zero or move +term / -term

auto var_expr_to_expr = [](auto& ctx)
	{
		return std::visit([](auto&& arg) {return SExpression{ arg }; }, ctx.GetValue());
	};
auto var_term_to_term = [](auto& ctx)
	{
		return std::visit([](auto&& arg) {return STerm{ arg }; }, ctx.GetValue());
	};
auto expression_tail_parser = 
(
	("+" >> term >> expression_tail) [term_plus_tail]
 | ("-" >> term >> expression_tail)[term_minus_tail]
 | (!aliases::char_any)[on_end_of_string]

)[var_expr_to_expr];

auto term_tail_parser = 
(
	  ("*" >> factor >> term_tail)[term_mult_termtail]
	| ("/" >> factor >> term_tail)[term_div_termtail]
	| (!aliases::char_any)[
		([](auto& ctx) {
			return STerm{ 1 }; }
			)
	]
)[var_term_to_term];

auto action_on_expression = [](auto& ctx) { return SExpression{ std::get<0>(ctx.GetValue()).m_value + std::get<1>(ctx.GetValue()).m_value }; };
auto action_on_term = [](auto& ctx) { 
	return STerm{ std::get<0>(ctx.GetValue()).m_value * std::get<1>(ctx.GetValue()).m_value }; };

//auto parser_term = (factor >> term_tail)[];
auto parser_expr = (term >> expression_tail)[action_on_expression];
auto parser_term	= (factor >> term_tail)[action_on_term];
IMPLEMENT_RULE(factor,			parser_factor);
IMPLEMENT_RULE(term,			parser_term)
IMPLEMENT_RULE(term_tail,		term_tail_parser);
IMPLEMENT_RULE(expression,		parser_expr);
IMPLEMENT_RULE(expression_tail, expression_tail_parser);

/*

factor	-> number | ( expr) 
term	-> term * factor | term / factor
expr	-> term | term + expr | term - expr

| 
*/