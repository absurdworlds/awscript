/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <hrscript/parser/Parser.h>
#include <hrscript/ast/Declaration.h>
#include <hrscript/ast/FuncDeclaration.h>
#include <hrscript/ast/Expression.h>
#include <hrscript/ast/UnaryExpr.h>
#include <hrscript/ast/BinaryExpr.h>
#include <hrscript/ast/CallExpr.h>
#include <hrscript/ast/NumberExpr.h>
#include <hrscript/ast/StringExpr.h>
#include <hrscript/ast/Statement.h>
#include <hrscript/ast/StatementBlock.h>

namespace hrscript {

ast::Declaration* Parser::parseDeclaration()
{
	Declaration* decl = 0;

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
	}

	if (getNextToken() != tok_semicolon)
		return 0;

	return decl;
}

/*
 * variableDecl ::= 'var' id id
 */
ast::Declaration* Parser::parseVariableDeclaration()
{
	return Error("NYI");
#if 0
	// Read variable type
	if (!isTypeName(getNextToken()))
		return 0;

	// Read variable name
	if (!isIdentifier(getNextToken()))
		return 0;
	
	// TODO: symbol table lookup
	std::string name = token.getData();

	// Variable* var = new Variable(/*symbol*/, /*thingy*/)// TODO ;

	return new VariableDeclaration(name);
#endif
}

/*
 * constantDecl ::= 'const' id id '=' expr
 */
ast::Declaration* Parser::parseConstantDeclaration()
{
	return Error("NYI");
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

	Expression* initializer = parseExpression();

	// Constant* constant = new Constant(/*symbol*/, /*thingy*/); // TODO 

	return new ConstantDeclaration(name, initializer);
#endif
}

/*
 * functionDecl ::= 'func' id id '(' args ')'
 *         args ::= arg (',' args)?
 *          arg ::= variableDecl
 */
ast::Declaration* Parser::parseFunctionDeclaration()
{
	// Return type
	if (!isTypeName(tok))
		return 0;

	std::string ret = token.getData();

	// Function name
	if (!isIdentifier(getNextToken()))
		return 0;

	// TODO: symbol table lookup
	std::string name = token.getData();

	if (getNextToken() != tok_l_paren)
		return 0;

	// Argument list
	std::vector<VariableDeclaration*> args;
	while (getNextToken().getType() == tok_kw_var) {
		auto arg = parseVariableDeclaration();
		if (arg == 0)
			return 0;

		args.push_back(arg);

		getNextToken();

		if (token == tok_r_paren)
			break;

		if (token != tok_comma)
			return 0;
	}
	
	if (token != tok_r_paren)
		return 0;

	return new FunctionDeclaration(name, ret, args);
}

/*
 *      classDecl ::= 'class' id
 *          class ::= classDecl '{' classBodyDecls '}'
 * classBodyDecl  ::= functionDecl | varDecl
 * classBodyDecls ::= classBodyDecl ';' classBodyDecls?
 */
ast::Declaration* Parser::parseClassDeclaration()
{
	return Error("NYI");
#if 0
	// Class name
	if (!isIdentifier(getNextToken()))
		return 0;

	std::string name = token.getData();

	if (!getNextToken().getType() != tok_l_brace)
		return new ClassDeclaration(name);

	// Class members
	std::vector<VariableDeclaration*> members;
	while (getNextToken().getType() == tok_kw_var) {
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
ast::Declaration* Parser::parsePrototypeDeclaration()
{
	return Error("NYI");
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

ast::Statement* Parser::parseStatement()
{
	switch (token.getType()) {
	case tok_kw_if:
		return parseBranchStatement();
	case tok_l_brace:
		return parseStatementBlock();
	default:
		return parseExpression();
	}
}

ast::StatementBlock* Parser::parseStatementBlock()
{
	std::vector<Statement*> statements;
	while (true) {
		auto statement = parseStatement();
		if(!statement)
			return 0;

		statements.push_back(statement);

		if (getNextToken().getType() == tok_r_brace)
			break;

		if (getNextToken().getType() != tok_semicolon)
			return 0;
	}

	// TODO: std::move
	return new ast::StatementBlock(statements);
}

ast::Statement* Parser::parseBranchStatement()
{
	if (!getNextToken().getType() != tok_kw_if)
		return 0;
	
	if (!getNextToken().getType() != tok_l_paren)
		return 0;

	ast::Expression ifExpr = parseExpression();
	if (!ifExpr)
		return 0;

	if (!getNextToken.getType() != tok_r_paren)
		return 0;

	ast::Statement* ifBody = parseStatement();
	ast::Statement* elseBody = 0;

	if (getNextToken().getType() == tok_kw_else)
		elseBody = parseStatement();

	return new ast::BranchStatement(ifExpr, ifBody, elseBody);
}

// TODO: separate expression parser
ast::Expression* Parser::parseExpression()
{
	// Parse left hand side
	ast::Expression* LHS = parseUnaryExpr();

	if (!LHS)
		return 0;

	return parseBinaryExpr(LHS, 0);
}

ast::Expression* Parser::parsePrimaryExpr()
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

ast::Expression* Parser::parseParenExpr()
{
	ast::Expression* expr = parseExpression();

	if (!getNextToken().getType() != tok_r_paren)
		return 0; // Expected )

	return expr;
}

ast::Expression* Parser::parseBinaryExpr(Expression* LHS, Precedence minPrec)
{
	while(1) {
		Precedence curPrec = getOperatorPrecedence(getNextToken());
		if (curPrec < minPrec)
			return LHS;

		ast::Expression* RHS = parseUnaryExpr();
		if(!RHS)
			return 0;

		Precedence nextPrec = getOperatorPrecedence(getNextToken());
		if(curPrec < nextPrec) {
			RHS = parseBinaryExpr(RHS, curPrec + 1);
			// I'm tired of seeing if(!ret) return 0,
			// TODO: make proper error checking
			if (!RHS)
				return 0;
		}

		LHS = new ast::BinaryExpr(op, LHS, RHS);
	}
}

ast::Expression* Parser::parseUnaryExpr()
{
	if (!isOperator(getNextToken()))
		return parsePrimaryExpr();

	ast::Expression* operand = parseUnaryExpr();
	if (!operand)
		return 0;

	return new ast::UnaryExpr(token.getType(), LHS);
}

ast::Expression* Parser::parseIdentifierExpr()
{
	std::string name = token.getData();

	if (getNextToken().getType() == tok_l_paren)
		return parseCallExpr(name);
	
	// TODO: postfix operators

	return new ast::IdentifierExpr(name);
}

ast::Expression* Parser::parseCallExpr(std::string func)
{
	std::vector<ast::Expression*> args;
	while (true) {
		ast::Expression* arg = parseExpression();
		if (!arg)
			return 0;

		args.push_back(arg);

		if (getNextToken().getType() == tok_r_paren)
			break;

		if (getNextToken().getType() != tok_comma)
			return 0; // expected ,
	}

	// TODO: std::move
	return new ast::CallExpr(func, args);
}

ast::Expression* Parser::parseStringExpr()
{
	if (!getNextToken().getType() != tok_string_literal)
		return 0;

	return new ast::StringExpr(token.getData());
}

ast::Expression* Parser::parseNumberExpr()
{
	if (!getNextToken().getType() != tok_numeric_constant)
		return 0;

	return new ast::NumberExpr(token.getData());
}

} // namespace hrscript
