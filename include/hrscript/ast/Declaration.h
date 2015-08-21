/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_Declaration_
#define _hrscript_AST_Declaration_
#include <hrscript/ast/Statement.h>

namespace hrscript {
namespace ast {
class Declaration : public Statement {
public:
	virtual ~Declaration();

	virtual accept(ast::Visitor& visitor);
};

} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_Declaration_
