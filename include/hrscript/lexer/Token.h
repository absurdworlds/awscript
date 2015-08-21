/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_token_
#define _hrscript_token_
#include <string>
namespace hrscript {
#define TOKEN(x) tok_ ## x
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) kw_ ## x
enum TokenType {
	TOKEN(illegal),
	TOKEN(eof),
	TOKEN(identifier),
	TOKEN(numeric_constant),
	TOKEN(string_literal),
	PUNCT(comma,            ","),
	PUNCT(dot,              "."),
	PUNCT(semicolon,        ";"),
	PUNCT(l_brace,          "{"),
	PUNCT(r_brace,          "}"),
	PUNCT(l_square,         "["),
	PUNCT(r_square,         "]"),
	PUNCT(l_paren,          "("),
	PUNCT(r_paren,          ")"),
	PUNCT(amp,              "&"),
	PUNCT(amp_amp,          "&&"),
	PUNCT(pipe,             "|"),
	PUNCT(pipe_pipe,        "||"),
	PUNCT(ast,              "*"),
	PUNCT(ast_equal,        "*="),
	PUNCT(plus,	        "+"),
	PUNCT(plus_equal,       "+="),
	PUNCT(plus_plus,        "++"),
	PUNCT(minus,	        "-"),
	PUNCT(minus_equal,      "-="),
	PUNCT(minus_minus,      "--"),
	PUNCT(slash,	        "/"),
	PUNCT(slash_equal,      "/="),
	PUNCT(percent,	        "%"),
	PUNCT(less,	        "<"),
	PUNCT(less_equal,       "<="),
	PUNCT(less_less,        "<<"),
	PUNCT(greater,          ">"),
	PUNCT(greater_equal,    ">="),
	PUNCT(greater_greater,  ">>"),
	PUNCT(bang,             "!"),
	PUNCT(bang_equal,       "!="),
	PUNCT(tilde,            "~"),
	PUNCT(caret,            "^"),
	PUNCT(equal,            "="),
	PUNCT(equal_equal,      "=="),
	KEYWORD(const),
	KEYWORD(var),
	KEYWORD(func),
	KEYWORD(if),
	KEYWORD(else),
	KEYWORD(class),
	KEYWORD(prototype),
	KEYWORD(instance),
	KEYWORD(return),
	KEYWORD(void),
	KEYWORD(float),
	KEYWORD(int),
	KEYWORD(string),
};

class Token {
public:
	TokenType getType() const
	{
		return type;
	}
	
	void setType(TokenType newType)
	{
		type = newType;
	}

	std::string getData() const
	{
		return data;
	}

	void setData(std::string newData)
	{
		data = newData;
	}
private:
	TokenType type;
	std::string data;
#if 0
	char* dataPtr;
	size_t dataSize;
#endif
};

/*! Returns true if token is an identifier */
bool isIdentifier(Token tok)
{
	return tok.getType() == tok_identifier;
}

/*! Returns true if token is a type name (in other words, can appear
 * after ‘var’)
 */
bool isTypeName(Token tok)
{
	return tok.getType() == tok_kw_func ||
	       tok.getType() >= tok_kw_void ||
	       isIdentifier(tok);
}

bool isOperator(Token tok)
{
	// TODO: make table
	return (tok.getType() == tok_dot) ||
		// IIRC, Daedalus doesn't allow to use '=' in expressions
	       (tok.getType() >= tok_amp && tok.getType() < tok_equal) ||
	       (tok.getType() == tok_equal_equal);
}
} // namespace hrscript
#endif//_hrscript_token_
