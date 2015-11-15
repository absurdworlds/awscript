/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef hrscript_print_token_h
#define hrscript_print_token_h
#include <string>
#include <hrscript/lexer/Token.h>
namespace hrscript {
/*!
 * Print token name (as in source code)
 */
std::string getTokenName(TokenType type);

/*!
 * Print sequence of characters which would produce the token
 */
std::string getTokenSpelling(TokenType kind);


// some macro magic to print token names
#define STR1(x) #x
#define STR(x)  STR1(x)
#define TOKEN1(x) tok_ ## x
#define KEYWORD1(x) kw_ ## x

std::string getTokenName(TokenType type)
{
	switch (type) {
#define TOKEN(x)   case TOKEN1(x)   : return STR(TOKEN1(x)); 
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) case KEYWORD1(x) : return STR(KEYWORD1(x));
#include <hrscript/lexer/TokenKinds.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	}
}

std::string getTokenSpelling(TokenType kind)
{
	switch (kind) {
#define TOKEN(x)    case TOKEN1(x)   : return STR(x);
#define PUNCT(x, y) case TOKEN1(x)   : return y;
#define KEYWORD(x)  case KEYWORD1(x) : return STR(x);
#include <hrscript/lexer/TokenKinds.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	}
}

#undef TOKEN1
#undef KEYWORD1
} // namespace hrscript
#endif//hrscript_print_token_h
