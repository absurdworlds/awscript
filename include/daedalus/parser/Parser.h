/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/Lexer/Lexer.h>
#include <daedalus/AST/Declaration.h>

namespace daedalus {
class Parser {
public:
	Parser(Lexer& lexer)
		: lexer(lexer)
	{
		token = lexer.getNextToken();
	}

	/*!
	 * Parse one top-level declaration.
	 */
	bool parseTopLevelDeclaration(Declaration& result);

	/*!
	 * Consume last token.
	 */
	void consumeToken()
	{
		token = lexer.getNextToken();
	}
private:

	//! Current extracted token
	Token token;

	//! Lexer which provides the stream of tokens
	Lexer& lexer;
};

} // namespace daedalus
