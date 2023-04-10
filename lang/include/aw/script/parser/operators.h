/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_parser_operators_h
#define aw_script_parser_operators_h
#include <aw/script/lexer/token.h>

namespace aw::script {

enum class precedence {
	none           = -1, // Not an operator
	unknown        = 0,
	assignment     = 1,  // =, any= (except relational)
	logical_or     = 2,  // ||
	logical_and    = 3,  // &&
	bitwise_or     = 4,  // |
	bitwise_xor    = 5,  // ^
	bitwise_and    = 6,  // &
	equality       = 7,  // ==, !=
	relational     = 8,  // >=, <=, <, >
	shift          = 9,  // <<, >>
	additive       = 10, // +, -
	multiplicative = 11, // *, /, %
	field_access   = 12, // .
	range          = 13, // ..
	max
};

inline precedence token_precedence(token tok)
{
	switch(tok.kind) {
	default:
		return precedence::none;
	case token_kind::equal:
	case token_kind::ast_equal:
	case token_kind::plus_equal:
	case token_kind::minus_equal:
	case token_kind::slash_equal:
	case token_kind::percent_equal:
	case token_kind::amp_equal:
	case token_kind::pipe_equal:
	case token_kind::caret_equal:
	case token_kind::less_less_equal:
	case token_kind::greater_greater_equal:
		return precedence::assignment;
	case token_kind::pipe_pipe:
		return precedence::logical_or;
	case token_kind::amp_amp:
		return precedence::logical_and;
	case token_kind::pipe:
		return precedence::bitwise_or;
	case token_kind::caret:
		return precedence::bitwise_xor;
	case token_kind::amp:
		return precedence::bitwise_and;
	case token_kind::equal_equal:
	case token_kind::bang_equal:
		return precedence::equality;
	case token_kind::less:
	case token_kind::less_equal:
	case token_kind::greater:
	case token_kind::greater_equal:
		return precedence::relational;
	case token_kind::less_less:
	case token_kind::greater_greater:
		return precedence::shift;
	case token_kind::plus:
	case token_kind::minus:
		return precedence::additive;
	case token_kind::ast:
	case token_kind::slash:
	case token_kind::percent:
		return precedence::multiplicative;
	case token_kind::dot:
		return precedence::field_access;
	case token_kind::dot_dot:
		return precedence::range;
	}
}

inline bool is_right_associative(token tok)
{
	return token_precedence(tok) == precedence::assignment;
}

inline bool is_binary_operator(token tok)
{
	return token_precedence(tok) > precedence::unknown;
}

} // namespace aw::script
#endif //aw_script_parser_operators_h
