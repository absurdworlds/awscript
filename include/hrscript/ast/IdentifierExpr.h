/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_IdentifierExpr_
#define _hrscript_AST_IdentifierExpr_
#include <hrscript/ast/Expression.h>

namespace hrscript {
namespace ast {
/*! Expression for identifier referencing */
class IdentifierExpr : public Expression {
public:
	IdentifierExpr(std::string id)
		: id(id)
	{
	}

	virtual ~IdentifierExpr() = default;

	virtual void accept(ast::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	std::string id;
};

} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_IdentifierExpr_
