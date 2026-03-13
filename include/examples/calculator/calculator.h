#pragma once

#include "parsing.h"


struct SFactor
{
	float m_value;
};

struct STerm
{
	float m_value;
};

struct SExpression
{
	float m_value;
};

RuleNew<class tag_factor, SFactor>	factor;
RuleNew<class tag_term, STerm>		term;
RuleNew<class tag_expr, SExpression>		expression;

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
//auto action_term_to_expression							= [](auto sTerm){return SExpression{sTerm.m_value}; };
auto action_expr_parser_result_to_term			= [](auto& ctx)->SExpression 
{
	return SExpression{ std::visit([](auto && arg){return arg.m_value;}, ctx.GetValue())}; 
};
auto action_term_parser_result_to_term			= [](auto& ctx)->STerm {
	return STerm{ std::visit([](auto && arg){return arg.m_value;}, ctx.GetValue())};
};
auto action_num_to_factor										= [](auto& ctx){return SFactor{ctx.GetValue()}; };
auto action_on_factor_in_brackets						= [](auto& ctx){return SFactor{std::get<1>(ctx.GetValue()).m_value}; }; // "(" factor ")" -> factor
auto action_of_factor												= [](auto& ctx){
	return std::visit([](auto&& f){return SFactor{f};}, ctx.GetValue()); 
}; //

auto parser_factor = (parser_number[action_num_to_factor]
											| ("(" >> expression >> ")")[action_on_factor_in_brackets]
											)[action_of_factor];

auto parser_term = (factor
								| (term >> "*" >> factor)[action_on_term_multiplied_with_factor]
								| (term >> "*" >> factor)[action_on_term_divided_with_factor]
										)[action_term_parser_result_to_term];

auto action_expr_plus_term = [](auto &ctx)
{

	const auto &term_value = std::get<0>(ctx.GetValue()).m_value;
	const auto &factor_value = std::get<2>(ctx.GetValue()).m_value;
	return STerm{term_value + factor_value};
};

auto action_expr_minus_term = [](auto &ctx)
{
	const auto &term_value = std::get<0>(ctx.GetValue()).m_value;
	const auto &factor_value = std::get<2>(ctx.GetValue()).m_value;
	return STerm{term_value + factor_value};
};

auto action_term_to_expr = [](auto&ctx){return SExpression{ctx.GetValue().m_value}; };

auto parser_expr = (
											(expression >> "+" >> term)[action_expr_plus_term]
										| (expression >> "-" >> term)[action_expr_minus_term]
										| term
										)[action_expr_parser_result_to_term];

IMPLEMENT_RULE(factor,			parser_factor);
IMPLEMENT_RULE(term,				parser_term);
IMPLEMENT_RULE(expression,	parser_expr);