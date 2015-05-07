/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/lexer/Lexer.h>
#include <daedalus/ast/Declaration.h>

namespace daedalus {
class Parser {
public:
	Parser(Lexer& lexer)
		: lexer(lexer)
	{
		token = lexer.getNextToken();
	}

	/*!
	 * Parse next top-level declaration.
	 */
	Declaration* parseDeclaration();

	/*!
	 * Consume peek token and extract next token.
	 */
	Token getNextToken()
	{
		return token = lexer.getNextToken();
	}
private:
	/*! Current lookahead (peek) token. */
	Token token;

	/*! Lexer which provides the stream of tokens */
	Lexer& lexer;

	Declaration* parseClassDeclaration();
	Declaration* parsePrototypeDeclaration();
	Declaration* parseInstanceDeclaration();
	Declaration* parseFunctionDeclaration();
	Declaration* parseVariableDeclaration();
	Declaration* parseVariableDeclaration();
	Expression* parseExpression();
	StatementBlock* parseStatementBlock();
	Statement* parseStatement();
};

} // namespace daedalus
