/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cstdio>
#include <hrscript/lexer/FileStream.h>
#include <hrscript/lexer/Lexer.h>

namespace hrscript {
std::string getTokenName(TokenType type)
{
	switch (type) {
	case tok_illegal:
		return "tok_illegal";
	case tok_eof:
		return "tok_eof";
	case tok_identifier:
		return "tok_identifier";
	case tok_numeric_constant:
		return "tok_numeric_constant";
	case tok_string_literal:
		return "tok_string_literal";
	case tok_comma:
		return "tok_comma";
	case tok_dot:
		return "tok_dot";
	case tok_semicolon:
		return "tok_semicolon";
	case tok_l_brace:
		return "tok_l_brace";
	case tok_r_brace:
		return "tok_r_brace";
	case tok_l_square:
		return "tok_l_square";
	case tok_r_square:
		return "tok_r_square";
	case tok_l_paren:
		return "tok_l_paren";
	case tok_r_paren:
		return "tok_r_paren";
	case tok_amp:
		return "tok_amp";
	case tok_amp_amp:
		return "tok_amp_amp";
	case tok_pipe:
		return "tok_pipe";
	case tok_pipe_pipe:
		return "tok_pipe_pipe";
	case tok_ast:
		return "tok_ast";
	case tok_ast_equal:
		return "tok_ast_equal";
	case tok_plus:
		return "tok_plus";
	case tok_plus_equal:
		return "tok_plus_equal";
	case tok_plus_plus:
		return "tok_plus_plus";
	case tok_minus:
		return "tok_minus";
	case tok_minus_equal:
		return "tok_minus_equal";
	case tok_minus_minus:
		return "tok_minus_minus";
	case tok_slash:
		return "tok_slash";
	case tok_slash_equal:
		return "tok_slash_equal";
	case tok_percent:
		return "tok_percent";
	case tok_less:
		return "tok_less";
	case tok_less_equal:
		return "tok_less_equal";
	case tok_less_less:
		return "tok_less_less";
	case tok_greater:
		return "tok_greater";
	case tok_greater_equal:
		return "tok_greater_equal";
	case tok_greater_greater:
		return "tok_greater_greater";
	case tok_bang:
		return "tok_bang";
	case tok_bang_equal:
		return "tok_bang_equal";
	case tok_tilde:
		return "tok_tilde";
	case tok_caret:
		return "tok_caret";
	case tok_equal:
		return "tok_equal";
	case tok_equal_equal:
		return "tok_equal_equal";
	case KEYWORD(const):
		return "kw_const";
	case KEYWORD(var):
		return "kw_var";
	case KEYWORD(func):
		return "kw_func";
	case KEYWORD(if):
		return "kw_if";
	case KEYWORD(else):
		return "kw_else";
	case KEYWORD(class):
		return "kw_class";
	case KEYWORD(prototype):
		return "kw_prototype";
	case KEYWORD(instance):
		return "kw_instance";
	case KEYWORD(return):
		return "kw_return";
	case KEYWORD(void):
		return "kw_void";
	case KEYWORD(float):
		return "kw_float";
	case KEYWORD(int):
		return "kw_int";
	case KEYWORD(string):
		return "kw_string";
	}
}

void printTokenInfo(Token& token)
{
	std::string msg;
	switch (token.getType()) {
	case tok_illegal:
	case tok_eof:
	case tok_comma:
	case tok_dot:
	case tok_semicolon:
	case tok_l_brace:
	case tok_r_brace:
	case tok_l_square:
	case tok_r_square:
	case tok_l_paren:
	case tok_r_paren:
	case tok_amp:
	case tok_amp_amp:
	case tok_pipe:
	case tok_pipe_pipe:
	case tok_ast:
	case tok_ast_equal:
	case tok_plus:
	case tok_plus_equal:
	case tok_plus_plus:
	case tok_minus:
	case tok_minus_equal:
	case tok_minus_minus:
	case tok_slash:
	case tok_slash_equal:
	case tok_percent:
	case tok_less:
	case tok_less_equal:
	case tok_less_less:
	case tok_greater:
	case tok_greater_equal:
	case tok_greater_greater:
	case tok_bang:
	case tok_bang_equal:
	case tok_tilde:
	case tok_caret:
	case tok_equal:
	case tok_equal_equal:
	case KEYWORD(const):
	case KEYWORD(var):
	case KEYWORD(func):
	case KEYWORD(if):
	case KEYWORD(else):
	case KEYWORD(class):
	case KEYWORD(prototype):
	case KEYWORD(instance):
	case KEYWORD(return):
	case KEYWORD(void):
	case KEYWORD(float):
	case KEYWORD(int):
	case KEYWORD(string):
		msg += getTokenName(token.getType());
		break;
	case tok_identifier:
	case tok_numeric_constant:
	case tok_string_literal:
		msg += getTokenName(token.getType());
		msg += ", ";
		msg += token.getData();
	}

	printf("〈%s〉 ", msg.c_str());
}

} // namespace hrscript

int main(int argc, char** arg)
{
	using namespace hrscript;
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
