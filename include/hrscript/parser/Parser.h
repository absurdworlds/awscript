/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_parser_
#define _hrscript_parser_
#include <hrscript/lexer/Lexer.h>
#include <hrscript/parser/Operators.h>

namespace hrscript {
namespace ast {
class Statement;
class Declaration;
class Expression;
}

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

	ast::Declaration* parseClassDeclaration();
	ast::Declaration* parsePrototypeDeclaration();
	ast::Declaration* parseFunctionDeclaration();
	ast::Declaration* parseVariableDeclaration();
	ast::Declaration* parseConstantDeclaration();

	ast::StatementBlock* parseStatementBlock();
	ast::Statement* parseStatement();
	ast::Statement* parseBranchStatement();

	ast::Expression* parseExpression();
	ast::Expression* parseIdentifierExpr();
	ast::Expression* parseParenExpr();
	ast::Expression* parseNumberExpr();
	ast::Expression* parseStringExpr();
	ast::Expression* parseUnaryExpr();
	ast::Expression* parsePrimaryExpr();
	ast::Expression* parseBinaryExpr(ast::Expression* LHS, prec::Level minPrec);
	ast::Expression* parseCallExpr(std::string func);
};

} // namespace hrscript
#endif //_hrscript_parser_
