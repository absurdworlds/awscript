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
#include <daedalus/ast/Expression.h>

namespace daedalus {
namespace ast {
enum class Operator {

};

class BinaryExpr : public Expression {
public:
	BinaryExpr(int op, Expression* LHS, Expression* RHS);
	virtual ~BinaryExpr();

	virtual Expression* getLHS();
	virtual Expression* getRHS();
	virtual Operator getOperation();

	virtual accept(ast::Visitor& visitor);
private:
	Expression* LHS;
	Expression* RHS;
	Operator op;
};

} // namespace ast
} // namespace daedalus
#endif//_daedalus_AST_BinaryExpr_
