/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_FuncDeclaration_
#define _hrscript_AST_FuncDeclaration_
#include <hrscript/ast/Declaration.h>
#include <hrscript/ast/StatementBlock.h>
namespace hrscript {
namespace ast {
class FuncDeclaration : public Declaration {
public:
	FuncDeclaration(std::string id, /* TODO */ std::string returnType,
	                std::vector<VariableDeclaration*> args);
	virtual ~FuncDeclaration();

	virtual bool accept(ast::Visitor& visitor) = 0;
private:
	std::string name;
	std::string returnType;
	std::vector<VariableDeclaration*> args;
};

class FuncDefinition : public Declaration {
public:
	FuncDefinition(FuncDeclaration* proto);
	virtual ~FuncDefinition();

	virtual bool accept(ast::Visitor& visitor) = 0;
private:
	FuncDeclaration* prototype;
	StatementBlock* body;
};
} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_FuncDeclaration_
