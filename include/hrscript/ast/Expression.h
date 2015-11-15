/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_Expression_
#define _hrscript_AST_Expression_
#include <hrscript/ast/Statement.h>

namespace hrscript {
namespace ast {
class Expression : public Statement {
public:
	virtual ~Expression() = default;

	virtual void accept(ast::Visitor& visitor) = 0;
};

} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_Expression_
