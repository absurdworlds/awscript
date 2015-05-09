/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/parser/Parser.h>

namespace deadalus {

Declaration* Parser::parseDeclaration()
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
	case kw_class:
		decl = parseClassDeclaration();
		break;
	case kw_prototype:
		decl = parsePrototypeDeclaration();
		break;
	case kw_instance:
		decl = parseInstanceDeclaration();
		break;
	}

	if (getNextToken() != tok_semicolon)
		return 0;

	return decl;
}

/*
 * variableDecl ::= 'var' id id
 */
Declaration* Parser::parseVariableDeclaration()
{
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
}

/*
 * constantDecl ::= 'const' id id '=' expr
 */
Declaration* Parser::parseConstantDeclaration()
{
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
}

/*
 * functionDecl ::= 'func' id id '(' args ')'
 *         args ::= arg (',' args)?
 *          arg ::= variableDecl
 */
Declaration* Parser::parseFunctionDeclaration()
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

	if (getNextToken() != tok_lparen)
		return 0;

	// Argument list
	std::vector<VariableDeclaration*> args;
	while (getNextToken().getType() == tok_kw_var) {
		auto arg = parseVariableDeclaration();
		if (arg == 0)
			return 0;

		args.push_back(arg);

		getNextToken();

		if (token == tok_rparen)
			break;

		if (token != tok_comma)
			return 0;
	}
	
	if (token != tok_rparen)
		return 0;

	return new FunctionDeclaration(name, ret, args);
}

/*
 *     classDecl ::= 'class' id
 *         class ::= classDecl '{' variableDecls '}'
 * variableDecls ::= variableDecl ';' variableDecls?
 */
Declaration* Parser::parseClassDeclaration()
{
	// Class name
	if (!isIdentifier(getNextToken()))
		return 0;

	std::string name = token.getData();

	if (!getNextToken().getType() != tok_lbrace)
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

	if (!getNextToken().getType() != tok_rbrace)
		return 0;

	return new ClassDeclaration(name, members);
}

/*
 * prototype ::= 'prototype' id '(' id ')' '{' stmts '}'
 */
Declaration* Parser::parsePrototypeDeclaration()
{
	// Prototype name
	if (!isIdentifier(getNextToken()))
		return 0;

	std::string name = token.getData();

	if (!getNextToken().getType() != tok_lparen)
		return 0;

	if (!isIdentifier(getNextToken()))
		return 0;

	std::string base = token.getData();

	if (!getNextToken().getType() != tok_rparen)
		return 0;
	
	if (!getNextToken().getType() != tok_lbrace)
		return 0;

	StatementBlock* body = parseStatementBlock();

	return new PrototypeDeclaration(name, base, body);
}

/*
 * instanceDecl ::= 'instance' id '(' id ')' 
 *     instance ::= instanceDecl '{' stmts '}'
 */
Declaration* Parser::parseInstanceDeclaration()
{
	// Instance name
	if (!isIdentifier(getNextToken()))
		return 0;

	std::string name = token.getData();

	if (!getNextToken().getType() != tok_lparen)
		return 0;

	if (!isIdentifier(getNextToken()))
		return 0;

	std::string base = token.getData();

	if (!getNextToken().getType() != tok_rparen)
		return 0;

	if (!getNextToken().getType() != tok_lbrace)
		return InstanceDeclaration(name, base);

	StatementBlock* body = parseStatementBlock();

	return new InstanceDeclaration(name, base, body);
}

StatementBlock* Parser::parseStatementBlock()
{
	
}

Statement* Parser::parseVariableStatement()
{

}

Statement* Parser::parseAssignmentStatement()
{
}

Statement* Parser::parseBranchStatement()
{
}

Expression* Parser::parseExpression()
{
	// make sure that it does not get called
	// before parseExpression()
	getNextToken();

	switch(token.getType()) {
	case tok_l_paren:
		parseParenExpr();
		break;
	}
}

Expression* Parser::parseParenExpr()
{
#if 0 // not needed
	if (!getNextToken().getType() != tok_l_paren)
		return 0; // Expected (
#endif

	Expression* expr = parseExpression();

	if (!getNextToken().getType() != tok_r_paren)
		return 0; // Expected )

	return expr;
}

Expression* Parser::parseBinaryExpr()
{
}

Expression* Parser::parseUnaryExpr()
{
}

Expression* Parser::parseIdentifierExpr()
{
	std::string name = token.getData();

	if (!getNextToken().getType() == tok_l_paren)
		return parseCallExpr(name);

	return new IdentifierExpr(name);
}

Expression* Parser::parseCallExpr(std::string func)
{
	if (getNextToken().getType() == tok_r_paren)
		return new CallExpr(func);

	std::vector<Expression*> Args;
	while (token.getType() != tok_r_paren) {
		Expression* arg = parseExpression();
		if (!arg)
			return 0;

		Args.push_back(arg);

		if ()
			break;

		if (getNextToken().getType() != tok_comma)
			return 0; // expected ,
	}

	if (getNextToken().getType() != tok_r_paren)
		return 0;

	// TODO: std::move
	return new CallExpr(func, args);
}

Expression* Parser::parseStringExpr()
{
	if (!getNextToken().getType() != tok_string_literal)
		return 0;

	return new StringExpr(token.getData());
}

Expression* Parser::parseNumberExpr()
{
	if (!getNextToken().getType() != tok_numeric_constant)
		return 0;

	return new NumberExpr(token.getData());

}

} // namespace daedalus
