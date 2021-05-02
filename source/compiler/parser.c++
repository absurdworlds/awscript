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
	uptr<ast::Declaration> decl;

	switch(token.type()) {
	case kw_var:
		decl = parse_variable_declaration();
		break;
	case kw_const:
		decl = parseConstantDeclaration();
		break;
	case kw_func:
		decl = parseFunctionDefinition();
		break;
	case kw_prototype:
		decl = parsePrototypeDeclaration();
		break;
	case kw_class:
		decl = parseClassDeclaration();
		break;
	default:
		return nullptr;
	}

	/* TODO: do not forget about global variables*/
	while (match(tok_semicolon));

	return decl;
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

	if (match(tok_equal))
	{
		auto initializer = parseExpression();
		var->set_initialier(std::move(initializer));
	}

	if (!match(tok_semicolon))
		return error_unexpected_token(diag, token, tok_semicolon);

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

	if (!match(tok_equal))
		return error(diag, Diagnostic::expected_initializer, token);

	auto initializer = parseExpression();
	var->set_initialier(std::move(initializer));

	if (!match(tok_semicolon))
		return error_unexpected_token(diag, token, tok_semicolon);

	return var;
}

uptr<ast::Variable>
Parser::parse_variable()
{
	// Read variable type
	if (!is_type_name(token))
		return nullptr;

	// Read variable name
	if (!is_identifier(getNextToken()))
		return nullptr;
	
	// TODO: symbol table lookup
	std::string_view name = token.data();

	getNextToken(); // consume identifier

	// Variable* var = new Variable(/*symbol*/, /*thingy*/)// TODO ;

	return ast::Variable::create(name);
}

bool Parser::match(TokenType expected)
{
	if (token.type() != expected)
		return false;

	// consume token
	getNextToken();
	return true;
}

/*
 * functionDecl ::= 'func' id id '(' args ')'
 *         args ::= arg (',' args)?
 *          arg ::= variableDecl
 */
uptr<ast::FunctionProto>
Parser::parseFunctionPrototype()
{
	// Return type
	if (!is_type_name(token))
		return nullptr;

	std::string_view ret = token.data();

	// Function name
	if (!is_identifier(getNextToken()))
		return nullptr;

	// TODO: symbol table lookup
	std::string_view name = token.data();

	// consume identifier
	getNextToken();
	if (!match(tok_l_paren))
		return nullptr;

	// Argument list
	std::vector<uptr<ast::Variable>> args;

	// TODO: 'var int a'  or  'int a'
	// while ((token == tok_identifier) || (token == kw_var)) {
	while (match(kw_var)) {
		auto arg = parse_variable();
		if (!arg)
			return nullptr;

		args.push_back(std::move(arg));

		if (token.type() == tok_r_paren)
			break;

		if (!match(tok_comma))
			return error_unexpected_token(diag, token, tok_comma);
	}
	
	if (!match(tok_r_paren))
		return error(diag, Diagnostic::ExpectedVariableDecl, token, tok_r_paren);

	return ast::FunctionProto::create(name, ret, std::move(args));
}
/*
std::nullptr_t Parser::error(Diagnostic::ID id)
{
	return nullptr;
}*/

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

	if (token.type() == tok_semicolon)
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
	return error_not_implemented_yet(diag, token);
#if 0
	// Class name
	if (!is_identifier(getNextToken()))
		return 0;

	std::string_view name = token.data();

	if (!getNextToken().type() != tok_l_brace)
		return new ClassDeclaration(name);

	// Class members
	std::vector<Variable*> members;
	while (getNextToken().type() == kw_var) {
		auto var = parse_variable();
		if (var == 0)
			return 0;

		members.push_back(var);

		getNextToken();

		if (token != tok_semicolon)
			return 0;
	}

	if (!getNextToken().type() != tok_r_brace)
		return 0;

	return new ClassDeclaration(name, members);
#endif
}

/*
 * prototype ::= 'prototype' id '(' id ')' '{' stmts '}'
 */
uptr<ast::Declaration>
Parser::parsePrototypeDeclaration()
{
	return error_not_implemented_yet(diag, token);
#if 0
	// Prototype name
	if (!is_identifier(getNextToken()))
		return 0;

	std::string_view name = token.data();

	if (!getNextToken().type() != tok_l_paren)
		return 0;

	if (!is_identifier(getNextToken()))
		return 0;

	std::string base = token.data();

	if (!getNextToken().type() != tok_r_paren)
		return 0;
	
	if (!getNextToken().type() != tok_l_brace)
		return 0;

	StatementBlock* body = parseStatementBlock();

	return new PrototypeDeclaration(name, base, body);
#endif
}

uptr<ast::Statement>
Parser::parseStatement()
{
	switch (token.type()) {
	case kw_if:
		getNextToken(); // consume 'if'
		return parseBranchStatement();
	case tok_l_brace:
		return parseStatementBlock();
	default:
		return parseExprStatement();
	}
}

uptr<ast::Statement>
Parser::parseExprStatement()
{
	auto expr = parseExpression();

	if (!match(tok_semicolon))
		return error(diag, Diagnostic::ExpectedSemicolonAfterExpression, token);

	return std::move(expr);
}

uptr<ast::StatementBlock>
Parser::parseStatementBlock()
{
	if (!match(tok_l_brace))
		return nullptr; // expected '{'

	std::vector<uptr<ast::Statement>> statements;
	while (!match(tok_r_brace)) {
		auto statement = parseStatement();
		if(!statement)
			return nullptr;

		statements.push_back(std::move(statement));
	}

	return std::make_unique<ast::StatementBlock>(std::move(statements));
}

uptr<ast::Statement>
Parser::parseBranchStatement()
{
	if (!match(tok_l_paren))
		return error_unexpected_token(diag, token, tok_l_paren);

	uptr<ast::Expression> ifExpr = parseExpression();
	if (!ifExpr)
		return nullptr;

	if (!match(tok_r_paren))
		return error_unexpected_token(diag, token, tok_r_paren);

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

/********************** Expressions **********************/
uptr<ast::Expression>
Parser::parseExpression()
{
	// Parse left hand side
	uptr<ast::Expression> lhs = parseUnaryExpr();

	if (!lhs)
		return nullptr;

	return parseBinaryExpr(std::move(lhs), prec::Unknown);
}

uptr<ast::Expression>
Parser::parsePrimaryExpr()
{
	switch(token.type()) {
	case tok_l_paren:
		return parseParenExpr();
	case tok_identifier:
		return parseIdentifierExpr();
	case tok_numeric_constant:
		return parseNumberExpr();
	case tok_string_literal:
		return parseStringExpr();
	default:
		return 0; // expected expression
	}
}

uptr<ast::Expression>
Parser::parseParenExpr()
{
	if (!match(tok_l_paren))
		return error_unexpected_token(diag, token, tok_l_paren); // Expected (

	uptr<ast::Expression> expr = parseExpression();

	if (!match(tok_r_paren))
		return error_unexpected_token(diag, token, tok_r_paren);

	return expr;
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

uptr<ast::Expression>
Parser::parseUnaryExpr()
{
	if (!isOperator(token))
		return parsePrimaryExpr();

	Token opcode = token;
	getNextToken(); // consume operator

	uptr<ast::Expression> operand = parseUnaryExpr();
	if (!operand)
		return nullptr;

	return std::make_unique<ast::UnaryExpr>(
	        opcode.type(), std::move(operand));
}

uptr<ast::Expression>
Parser::parseIdentifierExpr()
{
	std::string_view name = token.data();

	getNextToken(); // consume identifier
	if (match(tok_l_paren))
		return parse_call_expr(name);
	
	// TODO: postfix operators

	return std::make_unique<ast::IdentifierExpr>(name);
}

uptr<ast::Expression>
Parser::parse_call_expr(std::string_view func)
{
	std::vector<uptr<ast::Expression>> args;

	if (!match(tok_r_paren)) {
		while (true) {
			auto arg = parseExpression();

			if (!arg)
				return nullptr;

			args.push_back(std::move(arg));

			if (match(tok_r_paren))
				break;

			if (!match(tok_comma))
				return error_unexpected_token(diag, token, tok_comma); // expected ,
		}
	}

	return std::make_unique<ast::CallExpr>(
	        std::move(func), std::move(args));
}

uptr<ast::Expression>
Parser::parseStringExpr()
{
	assert(token == tok_string_literal && "parseStringExpr called when there's no string literal!");

	Token tok = token;
	// Consume string
	getNextToken();

	return std::make_unique<ast::StringExpr>(tok.data());
}

uptr<ast::Expression>
Parser::parseNumberExpr()
{
	assert(token == tok_numeric_constant && "parseNumberExpr called when there's no number!");

	// store token, because we need to consume it
	Token tok = token;
	// consume number
	getNextToken();

	return std::make_unique<ast::NumberExpr>(tok.data());
}
} // namespace script
} // namespace aw
