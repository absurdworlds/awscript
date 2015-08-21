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
	IdentifierExpr(std::string id);
	virtual ~IdentifierExpr();

	virtual accept(ast::Visitor& visitor) = 0;
private:
	std::string identifier;
};

} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_IdentifierExpr_
