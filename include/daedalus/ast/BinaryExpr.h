/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_AST_AssignmentExpr_
#define _daedalus_AST_AssignmentExpr_
#include <daedalus/ast/Expression.h>

namespace daedalus {
namespace ast {
class BinaryExpr : public Expression {
public:
	virtual ~BinaryExpr();

	virtual Expression* getLHS();
	virtual Expression* getRHS();

	virtual accept(ast::Visitor& visitor);
private:
	Expression* LHS;
	Expression* RHS;
};

} // namespace ast
} // namespace daedalus
#endif//_daedalus_AST_AssignmentExpr_
