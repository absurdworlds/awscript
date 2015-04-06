/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_AST_BinaryExpr_
#define _daedalus_AST_BinaryExpr_
#include <vector>
#include <daedalus/ast/Expression.h>

namespace daedalus {
namespace ast {
/*! AST node for a function call */
class CallExpr : public Expression {
public:
	virtual ~BinaryExpr();

	virtual Function* getFunction();

	virtual Expression* operator [] (size_t arg);
	virtual Expression* getArgument(size_t arg);

	virtual accept(ast::Visitor& visitor);
private:
	Function* callee;
	std::vector<Expression*> arguments;
};

} // namespace ast
} // namespace daedalus
#endif//_daedalus_AST_BinaryExpr_
