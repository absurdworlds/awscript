/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_BinaryExpr_
#define _hrscript_AST_BinaryExpr_
#include <hrscript/ast/Expression.h>

namespace aw {
namespace script {
namespace ast {
/*!
 * binary = unary op unary
 */
class BinaryExpr : public Expression {
public:
	BinaryExpr(int op,
	           std::unique_ptr<Expression> lhs,
	           std::unique_ptr<Expression> rhs)
		: op(op), lhs(std::move(lhs)), rhs(std::move(rhs))
	{
	}

	virtual ~BinaryExpr() = default;

	virtual Expression& getLHS()
	{
		return *lhs.get();
	}
	virtual Expression& getRHS()
	{
		return *rhs.get();
	}
	virtual int getOperation()
	{
		return op;
	}

	virtual void accept(ast::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	std::unique_ptr<Expression> lhs;
	std::unique_ptr<Expression> rhs;
	int op;
};

} // namespace ast
} // namespace script
} // namespace aw
#endif//_hrscript_AST_BinaryExpr_
