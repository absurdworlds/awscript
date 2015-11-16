/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <vector>

#include <hrscript/parser/Parser.h>

#include <hrscript/ast/VariableDeclaration.h>
#include <hrscript/ast/FuncDeclaration.h>

#include <hrscript/ast/Expression.h>
#include <hrscript/ast/UnaryExpr.h>
#include <hrscript/ast/BinaryExpr.h>
#include <hrscript/ast/CallExpr.h>
#include <hrscript/ast/NumberExpr.h>
#include <hrscript/ast/StringExpr.h>
#include <hrscript/ast/IdentifierExpr.h>

#include <hrscript/ast/Statement.h>
#include <hrscript/ast/IfElseStatement.h>
#include <hrscript/ast/StatementBlock.h>

namespace hrscript {
uptr<ast::Declaration>
ErrorDeclaration(std::string msg)
{
	//print(msg);
	return nullptr;
}

uptr<ast::Declaration>
Parser::parseDeclaration()
{
	uptr<ast::Declaration> decl;

	switch(token.getType()) {
	case kw_var:
		decl = parseVariableDeclaration();
		break;
	case kw_const:
		decl = parseConstantDeclaration();
		break;
	case kw_func:
		decl = parseFunctionDeclaration();
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

	if (getNextToken().getType() != tok_semicolon)
		return nullptr;

	return decl;
}

/*
 * variableDecl ::= 'var' id id
 */
uptr<ast::VariableDeclaration>
Parser::parseVariableDeclaration()
{
	// Read variable type
	if (!isTypeName(token))
		return nullptr;

	// Read variable name
	if (!isIdentifier(getNextToken()))
		return nullptr;
	
	// TODO: symbol table lookup
	std::string name = token.getData();

	// Variable* var = new Variable(/*symbol*/, /*thingy*/)// TODO ;

	return std::make_unique<ast::VariableDeclaration>(name);
}

/*
 * constantDecl ::= 'const' id id '=' expr
 */
uptr<ast::Declaration>
Parser::parseConstantDeclaration()
{
	return ErrorDeclaration("NYI");
#if 0
	// Read variable type
	if (!isTypeName(getNextToken()))
		return 0;

	// Read variable name
	if (!isIdentifier(getNextToken()))
		return 0;

	// TODO: symbol table lookup
	std::string name = token.getData();

	// Read constant initializer
	if (getNextToken().getType() != tok_equals)
		return 0;

	ast::Expression* initializer = parseExpression();

	// Constant* constant = new Constant(/*symbol*/, /*thingy*/); // TODO 

	return new ConstantDeclaration(name, initializer);
#endif
}

bool Parser::match(TokenType expected)
{
	if (token.getType() != expected)
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
uptr<ast::Declaration>
Parser::parseFunctionDeclaration()
{
	// Return type
	if (!isTypeName(token))
		return 0;

	std::string ret = token.getData();

	// Function name
	if (!isIdentifier(getNextToken()))
		return 0;

	// TODO: symbol table lookup
	std::string name = token.getData();

	if (getNextToken().getType() != tok_l_paren)
		return 0;

	// Argument list
	std::vector<uptr<ast::VariableDeclaration>> args;
	while (getNextToken().getType() == kw_var) {
		auto arg = parseVariableDeclaration();
		if (!arg)
			return nullptr;

		args.push_back(std::move(arg));

		getNextToken();

		if (token.getType() == tok_r_paren)
			break;

		if (token.getType() != tok_comma)
			return 0;
	}
	
	if (token.getType() != tok_r_paren)
		return 0;

	return std::make_unique<ast::FuncDeclaration>(name, ret, std::move(args));
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
	return ErrorDeclaration("NYI");
#if 0
	// Class name
	if (!isIdentifier(getNextToken()))
		return 0;

	std::string name = token.getData();

	if (!getNextToken().getType() != tok_l_brace)
		return new ClassDeclaration(name);

	// Class members
	std::vector<VariableDeclaration*> members;
	while (getNextToken().getType() == kw_var) {
		auto var = parseVariableDeclaration();
		if (var == 0)
			return 0;

		members.push_back(var);

		getNextToken();

		if (token != tok_semicolon)
			return 0;
	}

	if (!getNextToken().getType() != tok_r_brace)
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
	return ErrorDeclaration("NYI");
#if 0
	// Prototype name
	if (!isIdentifier(getNextToken()))
		return 0;

	std::string name = token.getData();

	if (!getNextToken().getType() != tok_l_paren)
		return 0;

	if (!isIdentifier(getNextToken()))
		return 0;

	std::string base = token.getData();

	if (!getNextToken().getType() != tok_r_paren)
		return 0;
	
	if (!getNextToken().getType() != tok_l_brace)
		return 0;

	StatementBlock* body = parseStatementBlock();

	return new PrototypeDeclaration(name, base, body);
#endif
}

uptr<ast::Statement>
Parser::parseStatement()
{
	switch (token.getType()) {
	case kw_if:
		return parseBranchStatement();
	case tok_l_brace:
		return parseStatementBlock();
	default:
		return parseExpression();
	}
}

uptr<ast::StatementBlock>
Parser::parseStatementBlock()
{
	std::vector<uptr<ast::Statement>> statements;
	while (true) {
		auto statement = parseStatement();
		if(!statement)
			return nullptr;

		statements.push_back(std::move(statement));

		if (getNextToken().getType() == tok_r_brace)
			break;

		if (getNextToken().getType() != tok_semicolon)
			return nullptr;
	}

	return std::make_unique<ast::StatementBlock>(std::move(statements));
}

uptr<ast::Statement>
Parser::parseBranchStatement()
{
	if (getNextToken().getType() != kw_if)
		return nullptr;
	
	if (getNextToken().getType() != tok_l_paren)
		return nullptr;

	uptr<ast::Expression> ifExpr = parseExpression();
	if (!ifExpr)
		return nullptr;

	if (getNextToken().getType() != tok_r_paren)
		return nullptr;

	uptr<ast::Statement> ifBody = parseStatement();
	uptr<ast::Statement> elseBody = nullptr;

	if (getNextToken().getType() == kw_else)
		elseBody = parseStatement();

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
		return 0;

	return parseBinaryExpr(std::move(lhs), prec::Unknown);
}

uptr<ast::Expression>
Parser::parsePrimaryExpr()
{
	// make sure that it does not get called
	// before parsePrimaryExpr()
	getNextToken();

	switch(token.getType()) {
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
	uptr<ast::Expression> expr = parseExpression();

	if (getNextToken().getType() != tok_r_paren)
		return nullptr; // Expected )

	return expr;
}

uptr<ast::Expression>
Parser::parseBinaryExpr(uptr<ast::Expression> LHS,
                        prec::Level minPrec)
{
	while(1) {
		prec::Level curPrec = getOperatorPrecedence(getNextToken());
		if (curPrec < minPrec)
			return LHS;

		uptr<ast::Expression> RHS = parseUnaryExpr();
		if(!RHS)
			return 0;

		prec::Level nextPrec = getOperatorPrecedence(getNextToken());
		if(curPrec < nextPrec) {
			RHS = parseBinaryExpr(
			       std::move(RHS),
			       prec::Level(curPrec + 1));
			if (!RHS)
				return 0;
		}

		LHS = std::make_unique<ast::BinaryExpr>(
		       token.getType(), std::move(LHS), std::move(RHS));
	}
}

uptr<ast::Expression>
Parser::parseUnaryExpr()
{
	if (!isOperator(getNextToken()))
		return parsePrimaryExpr();

	uptr<ast::Expression> operand = parseUnaryExpr();
	if (!operand)
		return nullptr;

	return std::make_unique<ast::UnaryExpr>(
	        token.getType(), std::move(operand));
}

uptr<ast::Expression>
Parser::parseIdentifierExpr()
{
	std::string name = token.getData();

	if (getNextToken().getType() == tok_l_paren)
		return parseCallExpr(name);
	
	// TODO: postfix operators

	return std::make_unique<ast::IdentifierExpr>(name);
}

uptr<ast::Expression>
Parser::parseCallExpr(std::string func)
{
	std::vector<uptr<ast::Expression>> args;
	while (true) {
		auto arg = parseExpression();

		if (!arg)
			return nullptr;

		args.push_back(std::move(arg));

		if (getNextToken().getType() == tok_r_paren)
			break;

		if (getNextToken().getType() != tok_comma)
			return nullptr; // expected ,
	}

	return std::make_unique<ast::CallExpr>(
	        std::move(func), std::move(args));
}

uptr<ast::Expression>
Parser::parseStringExpr()
{
	if (getNextToken().getType() != tok_string_literal)
		return nullptr;

	return std::make_unique<ast::StringExpr>(token.getData());
}

uptr<ast::Expression>
Parser::parseNumberExpr()
{
	if (getNextToken().getType() != tok_numeric_constant)
		return nullptr;

	return std::make_unique<ast::NumberExpr>(token.getData());
}
} // namespace hrscript
