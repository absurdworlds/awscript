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
	switch(token.getType()) {
	case kw_var:
		return parseVariableDeclaration();
	case kw_const:
		return parseConstantDeclaration();
	case kw_func:
		return parseFunctionDeclaration();
	case kw_class:
		return parseClassDeclaration();
	case kw_prototype:
		return parsePrototypeDeclaration();
	case kw_instance:
		return parseInstanceDeclaration();
	}
}

/*
 * variableDecl ::= 'var' id id
 */
Declaration* parseVariableDeclaration()
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
Declaration* parseConstantDeclaration()
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
Declaration* parseFunctionDeclaration()
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
	getNextToken();

	std::vector<VariableDeclaration*> args;
	while (token.getType() == tok_kw_var) {
		VariableDeclaration* arg = parseVariableDeclaration();
		args.push_back(arg);

		getNextToken();
	}

	if (getNextToken() != tok_rparen)
		return 0;

	getNextToken();

	return new FunctionDeclaration(name, ret, args);
}
} // namespace daedalus
