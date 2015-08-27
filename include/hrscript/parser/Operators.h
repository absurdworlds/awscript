/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_OperatorPrecedence_
#define _hrscript_OperatorPrecedence_
#include <hrscript/lexer/Token.h>

namespace hrscript {
enum class Precedence {
	Unknown        = 0,
	Assignment     = 1,  // =, any= (except relational)
	LogicalOr      = 2,
	LogicalAnd     = 3,
	BitwiseOr      = 4,
	BitwiseXor     = 5,
	BitwiseAnd     = 6,
	Equality       = 7,  // ==, !=
	Relational     = 8,  // >=, <=, <, >
	Shift          = 9,  // <<, >>
	Additive       = 10, // +, -
	Multiplicative = 11  // *, *, %
}

Precedence getOperatorPrecedence(Token tok)
{
	switch(tok.getType()) {
	default:
		// Not an operator
		return Precedence::Unknown;
	case tok_ast_equal:
	case tok_plus_equal:
	case tok_minus_equal:
	case tok_slash_equal:
		return Precedence::Assignment;
	case tok_tok_pipe_pipe:
		return Precedence::LogicalOr;
	case tok_tok_amp_amp:
		return Precedence::LogicalAnd;
	case tok_tok_pipe:
		return Precedence::BitwiseOr;
	case tok_tok_caret:
		return Precedence::BitwiseXor;
	case tok_tok_amp:
		return Precedence::BitwiseAnd;
	case tok_tok_equal_equal:
	case tok_tok_bang_equal:
		return Precedence::Equality;
	case tok_less:
	case tok_less_equal:
	case tok_greater:
	case tok_greater_equal:
		return Precedence::Relational;
	case tok_plus:
	case tok_minus:
		return Precedence::Additive;
	case tok_ast:
	case tok_slash:
		return Precedence::Multiplicative;	
	}
}

bool isOperator(Token tok)
{
	return getOperatorPrecedence(tok) > Precedence::Unknown;
}

} // namespace hrscript
#endif //_hrscript_OperatorPrecedence_
