/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_parser_h
#define aw_script_parser_h
#include <aw/script/common/types.h>
#include <aw/script/lexer/lexer.h>
#include <aw/script/ast/declaration.h>
#include <aw/script/parser/operators.h>

namespace aw {
namespace script {
namespace ast {
class Statement;
class StatementBlock;
class FunctionProto;
class Variable;
class Expression;
}

class diagnostics_engine;

/*!
 * Parser transforms token stream into the Abstract Syntax Stream (AST).
 */
class Parser {
public:
	Parser(Lexer& lexer, diagnostics_engine& diag)
		: lexer(lexer), diag(diag)
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
	/*!
	 * Match token, and consume if matched.
	 */
	bool match(TokenType expected);

	uptr<ast::Declaration> parseClassDeclaration();
	uptr<ast::Declaration> parsePrototypeDeclaration();
	uptr<ast::FunctionProto> parseFunctionPrototype();
	uptr<ast::Declaration> parseFunctionDefinition();
	uptr<ast::Declaration> parse_variable_declaration();
	uptr<ast::Declaration> parseConstantDeclaration();

	uptr<ast::Variable> parse_variable();

	uptr<ast::StatementBlock> parseStatementBlock();
	uptr<ast::Statement> parseStatement();
	uptr<ast::Statement> parseBranchStatement();
	uptr<ast::Statement> parseExprStatement();

	uptr<ast::Expression> parseExpression();
	uptr<ast::Expression> parseUnaryExpr();
	uptr<ast::Expression>
	parseBinaryExpr(uptr<ast::Expression> LHS, prec::Level minPrec);

	uptr<ast::Expression> parsePrimaryExpr();
	uptr<ast::Expression> parseIdentifierExpr();
	uptr<ast::Expression> parseParenExpr();
	uptr<ast::Expression> parseNumberExpr();
	uptr<ast::Expression> parseStringExpr();
	uptr<ast::Expression> parse_call_expr(std::string_view func);

private:
	/*! Current lookahead (peek) token. */
	Token token;

	/*! Lexer which provides the stream of tokens */
	Lexer& lexer;

	/*! Diagnostics engine for error reporting */
	diagnostics_engine& diag;
};

} // namespace script
} // namespace aw
#endif //aw_script_parser_h
