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
#include <hrscript/common/types.h>
#include <hrscript/lexer/Lexer.h>
#include <hrscript/ast/Declaration.h>
#include <hrscript/parser/Operators.h>

namespace hrscript {
namespace ast {
class Statement;
class StatementBlock;
class FuncDeclaration;
class VariableDeclaration;
class Expression;
}

class Parser {
public:
	Parser(Lexer& lexer)
		: lexer(lexer)
	{
		token = lexer.getCurrentToken();
	}

	virtual ~Parser() = default;

	/*!
	 * Parse next top-level declaration.
	 */
	uptr<ast::Declaration> parseDeclaration();

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

	/*!
	 * Match token, and consume if matched.
	 */
	bool match(TokenType expected);

	uptr<ast::Declaration> parseClassDeclaration();
	uptr<ast::Declaration> parsePrototypeDeclaration();
	uptr<ast::FuncDeclaration> parseFunctionPrototype();
	uptr<ast::Declaration> parseFunctionDefinition();
	uptr<ast::VariableDeclaration> parseVariableDeclaration();
	uptr<ast::Declaration> parseConstantDeclaration();

	uptr<ast::StatementBlock> parseStatementBlock();
	uptr<ast::Statement> parseStatement();
	uptr<ast::Statement> parseBranchStatement();
	uptr<ast::Statement> parseExprStatement();

	uptr<ast::Expression> parseExpression();
	uptr<ast::Expression> parseIdentifierExpr();
	uptr<ast::Expression> parseParenExpr();
	uptr<ast::Expression> parseNumberExpr();
	uptr<ast::Expression> parseStringExpr();
	uptr<ast::Expression> parseUnaryExpr();
	uptr<ast::Expression> parsePrimaryExpr();

	uptr<ast::Expression>
	parseBinaryExpr(uptr<ast::Expression> LHS, prec::Level minPrec);

	uptr<ast::Expression> parseCallExpr(std::string func);
};

} // namespace hrscript
#endif //_hrscript_parser_
