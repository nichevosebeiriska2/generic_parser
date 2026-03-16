#pragma once

#include "base_parser.h"
#include "alternative.h"
#include "sequence.h"
#include "list.h"
#include "repeate.h"
#include "parser_with_action.h"
#include "parser_predicate_not.h"
#include "rule.h"

namespace aliases
{
	constexpr auto EOF_ = !(char_any); // end 
	constexpr auto ALWAYS_TRUE = (EOF_ | char_any); // returns true in each case
};