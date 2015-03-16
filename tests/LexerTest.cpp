/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cstdio>
#include <daedalus/lexer/Lexer.h>

namespace daedalus {
void printTokenInfo(Token& token)
{
	std::string msg;
	switch (token.getType()) {
	case tok_illegal:
		msg += "tok_illegal(";
		break;
	case tok_eof:
		msg += "tok_eof(";
		break;
	case tok_identifier:
		msg += "tok_identifier(";
		goto identifier_info;
	case tok_numeric_constant:
		msg += "tok_numeric_constant(";
		goto identifier_info;
	case tok_string_literal:
		msg += "tok_string_literal(";
		goto identifier_info;
	case tok_comma:
		msg += "tok_comma";
		break;
	case tok_dot:
		msg += "tok_dot";
		break;
	case tok_l_brace:
		msg += "tok_l_brace";
		break;
	case tok_r_brace:
		msg += "tok_r_brace";
		break;
	case tok_l_square:
		msg += "tok_l_square";
		break;
	case tok_r_square:
		msg += "tok_r_square";
		break;
	case tok_l_paren:
		msg += "tok_l_paren";
		break;
	case tok_r_paren:
		msg += "tok_r_paren";
		break;
	case tok_amp:
		msg += "tok_amp";
		break;
	case tok_amp_amp:
		msg += "tok_amp_amp";
		break;
	case tok_pipe:
		msg += "tok_pipe";
		break;
	case tok_pipe_pipe:
		msg += "tok_pipe_pipe";
		break;
	case tok_ast:
		msg += "tok_ast";
		break;
	case tok_ast_equal:
		msg += "tok_ast_equal";
		break;
	case tok_plus:
		msg += "tok_plus";
		break;
	case tok_plus_equal:
		msg += "tok_plus_equal";
		break;
	case tok_plus_plus:
		msg += "tok_plus_plus";
		break;
	case tok_minus:
		msg += "tok_minus";
		break;
	case tok_minus_equal:
		msg += "tok_minus_equal";
		break;
	case tok_minus_minus:
		msg += "tok_minus_minus";
		break;
	case tok_slash:
		msg += "tok_slash";
		break;
	case tok_slash_equal:
		msg += "tok_slash_equal";
		break;
	case tok_percent:
		msg += "tok_percent";
		break;
	case tok_less:
		msg += "tok_less";
		break;
	case tok_less_equal:
		msg += "tok_less_equal";
		break;
	case tok_less_less:
		msg += "tok_less_less";
		break;
	case tok_greater:
		msg += "tok_greater";
		break;
	case tok_greater_equal:
		msg += "tok_greater_equal";
		break;
	case tok_greater_greater:
		msg += "tok_greater_greater";
		break;
	case tok_bang:
		msg += "tok_bang";
		break;
	case tok_bang_equal:
		msg += "tok_bang_equal";
		break;
	case tok_tilde:
		msg += "tok_tilde";
		break;
	case tok_caret:
		msg += "tok_caret";
		break;
	case tok_equal:
		msg += "tok_equal";
		break;
	case tok_equal_equal:
		msg += "tok_equal_equal";
		break;
	case KEYWORD(const):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(var):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(func):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(if):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(else):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(class):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(prototype):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(instance):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(return):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(void):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(float):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(int):
		msg += "tok_identifier(";
		goto identifier_info;
	case KEYWORD(string):
		msg += "tok_identifier(";
identifier_info:
		s += token.getData();
	default:
		break;
	}
	printf("%s), ", s.c_str());
}
} // namespace daedalus

int main(int argc, char** arg)
{
	using namespace daedalus;
	if (argc < 2)
		return 2;

	FileStream fstream(arg[1]);
	Lexer lexer(fstream);

	Token token;
	do {
		token = lexer.getNextToken();	

		printTokenInfo(token);
	} while (token.getType() != tok_eof);
}
