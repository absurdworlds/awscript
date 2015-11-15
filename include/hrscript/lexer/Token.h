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
enum TokenType {
#define TOKEN(x) tok_ ## x,
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) kw_ ## x,
#include <hrscript/lexer/TokenKinds.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
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
inline bool isIdentifier(Token tok)
{
	return tok.getType() == tok_identifier;
}

/*!
 * Returns true if token is a type name (in other words, can appear
 * after ‘var’)
 */
inline bool isTypeName(Token tok)
{
	return tok.getType() == kw_func ||
	       tok.getType() >= kw_void ||
	       isIdentifier(tok);
}

inline bool operator == (Token tok, TokenType kind)
{
	return tok.getType() == kind;
}
} // namespace hrscript
#endif//_hrscript_token_
