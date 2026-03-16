#pragma once

#include "parsing.h"

/*

 Grammar : 

	factor	-> <number> | "(" <expr> ")"
	term		-> <term> "*" <factor> | <term> "/" <factor>
	expr		-> <term> | <term> "+" <expr> | <term> "-" <expr>

	Have to be replaced with equivalent one to exclude left reqursion: 

	factor		-> <number> | "("  <expr> ")"
	term_tail -> { "*" | "/" } <term_tail> | {} // list of numbers with mult/divide or nothing
	expr_tail -> { "+" | "-" } <expr_tail> | {} // list of numbers with plus/minus or nothing
	term			-> <factor> <term_tail>						// term with optional list of multipliers
	expr			-> <term> <expr_tail>							// expression with optional list of terms  

*/


RuleNew<class tag_factor,			float>	factor;
RuleNew<class tag_term,				float>	term;
RuleNew<class tag_term_tail,	float>	term_tail;
RuleNew<class tag_expr,				float>	expression;
RuleNew<class tag_expr_tail,	float>	expression_tail;

auto action_var_float_to_float = [](auto &ctx) { return std::visit([](auto&& f){return f;}, ctx.GetValue()); };
auto action_on_factor_in_brackets	= [](auto& ctx){return std::get<1>(ctx.GetValue()); }; // "(" factor ")" -> factor

auto parser_factor = ( aliases::float_  | ("(" >> expression >> ")")[action_on_factor_in_brackets])[action_var_float_to_float];

auto return_default_expr = [](auto& ctx) { return 0.0f; };
auto return_default_term = [](auto& ctx) { return 1.0f; };
auto action_minus_term_and_tail =		[](auto& ctx) { const auto &[op, term, tail] = ctx.GetValue();		return (- term + tail); };
auto action_plus_term_and_tail =		[](auto& ctx) { const auto &[op, term, tail] = ctx.GetValue();		return (  term + tail); };
auto action_mult_factor_and_tail =	[](auto& ctx) { const auto &[op, factor, tail] = ctx.GetValue();	return       factor * tail; };
auto action_div_factor_and_tail =		[](auto& ctx) { const auto &[op, factor, tail] = ctx.GetValue();	return 1.0f/(factor * tail); };

auto expression_tail_parser = 
(
		("+" >> term >> expression_tail)[action_plus_term_and_tail]
	| ("-" >> term >> expression_tail)[action_minus_term_and_tail]
	| ((!ParserLiteralWithContext{"-"}) | (!ParserLiteralWithContext{"="}))[return_default_expr] // if there is no more "+" or "-" return term = 0
)[action_var_float_to_float];

auto term_tail_parser = 
(
	  ("*" >> factor >> term_tail)[action_mult_factor_and_tail]
	| ("/" >> factor >> term_tail)[action_div_factor_and_tail]
	| ((!ParserLiteralWithContext{"*"}) | (!ParserLiteralWithContext{"/"}))[return_default_term] // if there is no more "/" or "*" return multiplier = 1
)[action_var_float_to_float];

auto action_on_expression = [](auto& ctx) { const auto& [term, expr_tail] = ctx.GetValue(); return term + expr_tail; };
auto action_on_term				= [](auto& ctx) { const auto& [fact, term_tail] = ctx.GetValue(); return fact * term_tail; };

auto parser_expr = (term >> expression_tail)[action_on_expression];
auto parser_term = (factor >> term_tail)[action_on_term];

IMPLEMENT_RULE(factor,					parser_factor);
IMPLEMENT_RULE(term,						parser_term)
IMPLEMENT_RULE(term_tail,				term_tail_parser);
IMPLEMENT_RULE(expression,			parser_expr);
IMPLEMENT_RULE(expression_tail, expression_tail_parser);
 
