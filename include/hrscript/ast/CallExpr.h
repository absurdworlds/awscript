/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_CallExpr_
#define _hrscript_AST_CallExpr_
#include <vector>
#include <string>
#include <hrscript/ast/Expression.h>

namespace hrscript {
namespace ast {
/*! AST node for a function call */
class CallExpr : public Expression {
public:
	CallExpr(std::string callee, std::vector<Expression*> arguments)
		: callee(callee), arguments(std::move(arguments))
	{
	}

	virtual std::string getFunction();

	virtual Expression* operator [] (size_t arg);
	virtual Expression* getArgument(size_t arg);

	virtual void accept(ast::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	//Function* callee;
	std::string callee;
	std::vector<Expression*> arguments;
};

} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_CallExpr_
