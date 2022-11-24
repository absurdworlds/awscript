/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <vector>

#include <aw/script/ast/decl/variable.h>
#include <aw/script/ast/decl/function.h>
#include <aw/script/ast/decl/class.h>

#include <aw/script/ast/expression.h>
#include <aw/script/ast/expr/unary.h>
#include <aw/script/ast/expr/binary.h>
#include <aw/script/ast/expr/call.h>
#include <aw/script/ast/expr/number.h>
#include <aw/script/ast/expr/string.h>
#include <aw/script/ast/expr/identifier.h>

#include <aw/script/ast/statement.h>
#include <aw/script/ast/statement_block.h>
#include <aw/script/ast/if_else_statement.h>

#include <aw/script/utility/print_token.h>
#include <aw/script/parser/parser.h>

#include "errors.h"

namespace aw {
namespace script {
uptr<ast::Declaration>
Parser::parseDeclaration()
{
}

/*
 * variableDecl ::= 'var' id id '=' expr
 */
uptr<ast::Declaration>
Parser::parse_variable_declaration()
{
	getNextToken(); // consume 'var';

	auto var = parse_variable();
	if (!var)
		return nullptr;

	if (match(token_kind::equal))
	{
		auto initializer = parseExpression();
		var->set_initialier(std::move(initializer));
	}

	if (!match(token_kind::semicolon))
		return error_unexpected_token(diag, token, token_kind::semicolon);

	return var;
}


/*
 * constantDecl ::= 'const' id id '=' expr
 */
uptr<ast::Declaration>
Parser::parseConstantDeclaration()
{
	getNextToken(); // consume 'const';

	auto var = parse_variable();
	if (!var)
		return nullptr;

	var->set_const(true);

	if (!match(token_kind::equal))
		return error(diag, Diagnostic::expected_initializer, token);

	auto initializer = parseExpression();
	var->set_initialier(std::move(initializer));

	if (!match(token_kind::semicolon))
		return error_unexpected_token(diag, token, token_kind::semicolon);

	return var;
}

uptr<ast::Variable>
Parser::parse_variable()
{
	// Read variable type
	if (!is_type_name(token))
		return error(diag, Diagnostic::expected_type_name, token);

	// Read variable name
	if (!is_identifier(getNextToken()))
		return error(diag, Diagnostic::expected_identifier, token);
	
	// TODO: symbol table lookup
	std::string_view name = token.data();

	getNextToken(); // consume identifier

	// Variable* var = new Variable(/*symbol*/, /*thingy*/)// TODO ;

	return ast::Variable::create(name);
}


/*
 * functionDef  ::= functionDecl '{' stmts '}'
 */
uptr<ast::Declaration>
Parser::parseFunctionDefinition()
{
	getNextToken(); // consume 'func'

	auto proto = parseFunctionPrototype();

	if (!proto)
		return nullptr;

	if (token.type() == token_kind::semicolon)
		return std::move(proto);

	auto body = parseStatementBlock();

	if (!body)
		return nullptr;

	return ast::Function::create(
	        std::move(proto), std::move(body));
}

/*
 *      classDecl ::= 'class' id
 *          class ::= classDecl '{' classBodyDecls '}'
 * classBodyDecl  ::= functionDecl | varDecl
 * classBodyDecls ::= classBodyDecl ';' classBodyDecls?
 */
uptr<ast::Declaration>
Parser::parseClassDeclaration()
{
	// Class name
	if (!is_identifier(getNextToken()))
		return error(diag, Diagnostic::expected_identifier, token);

	std::string_view name = token.data();

	getNextToken(); // consume name;

	if (!match(token_kind::l_brace))
	{
		if (!match(token_kind::semicolon))
			return error_unexpected_token(diag, token, token_kind::semicolon);
		return std::make_unique<ast::ClassDeclaration>(name);
	}

	// Class members
	std::vector<uptr<ast::Variable>> members;
	while (match(kw_var)) {
		auto var = parse_variable();
		if (!var)
			return nullptr;

		members.emplace_back(std::move(var));

		if (!match(token_kind::semicolon))
			return error_unexpected_token(diag, token, token_kind::semicolon);
	}

	if (!match(token_kind::r_brace))
		return error_unexpected_token(diag, token, token_kind::r_brace);

	return std::make_unique<ast::ClassDeclaration>(name, std::move(members));
}


uptr<ast::Statement>
Parser::parseStatement()
{
	switch (token.type()) {
	case kw_if:
		getNextToken(); // consume 'if'
		return parseBranchStatement();
	case token_kind::l_brace:
		return parseStatementBlock();
	default:
		return parseExprStatement();
	}
}

uptr<ast::Statement>
Parser::parseExprStatement()
{
	auto expr = parseExpression();

	if (!match(token_kind::semicolon))
		return error(diag, Diagnostic::expected_semicolon_after_expression, token);

	return std::move(expr);
}

uptr<ast::Statement>
Parser::parseBranchStatement()
{
	if (!match(token_kind::l_paren))
		return error_unexpected_token(diag, token, token_kind::l_paren);

	uptr<ast::Expression> ifExpr = parseExpression();
	if (!ifExpr)
		return nullptr;

	if (!match(token_kind::r_paren))
		return error_unexpected_token(diag, token, token_kind::r_paren);

	uptr<ast::Statement> ifBody = parseStatement();
	if (!ifBody)
		return nullptr;

	uptr<ast::Statement> elseBody = nullptr;

	if (match(kw_else)) {
		elseBody = parseStatement();

		if (!elseBody)
			return nullptr;
	}

	return std::make_unique<ast::IfElseStatement>(
	        std::move(ifExpr), std::move(ifBody), std::move(elseBody));
}

uptr<ast::Expression>
Parser::parseBinaryExpr(uptr<ast::Expression> LHS,
                        prec::Level minPrec)
{
	while (1) {
		prec::Level curPrec = getOperatorPrecedence(token);
		if (curPrec < minPrec)
			return LHS;

		Token opcode = token;
		getNextToken(); // consume operator

		uptr<ast::Expression> RHS = parseUnaryExpr();
		if(!RHS)
			return nullptr;


		prec::Level nextPrec = getOperatorPrecedence(token);
		bool isRightAssoc = isRightAssociative(token);

		if (curPrec < nextPrec ||
		   (curPrec == nextPrec && isRightAssoc)) {
			RHS = parseBinaryExpr(
			       std::move(RHS),
			       prec::Level(curPrec + !isRightAssoc));
			if (!RHS)
				return nullptr;
		}

		LHS = std::make_unique<ast::BinaryExpr>(
		       opcode.type(), std::move(LHS), std::move(RHS));
	}
}

} // namespace script
} // namespace aw
