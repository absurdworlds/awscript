/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_UnaryExpr_
#define _hrscript_AST_UnaryExpr_
#include <hrscript/ast/Expression.h>

namespace hrscript {
namespace ast {

class UnaryExpr : public Expression {
public:
	UnaryExpr(int op, std::unique_ptr<Expression> operand)
		: op(op), operand(std::move(operand))
	{
	}

	virtual ~UnaryExpr() = default;

	virtual Expression& getOperand()
	{
		return *operand.get();
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
	std::unique_ptr<Expression> operand;
	int op;
};

} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_UnaryExpr_
