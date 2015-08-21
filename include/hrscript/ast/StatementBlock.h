/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_StatementBlock_
#define _hrscript_AST_StatementBlock_
#include <hrscript/ast/Statement.h>

namespace hrscript {
namespace ast {
/* TODO: Does Daedalus allow “freestanding” blocks?
 * {
 *    ...
 * }
 *
 * My memory tells me it doesn't. If so, I probably should
 * change base class
 */
class StatementBlock : public Statement {
public:
	StatementBlock(std::vector<Statement*> statements);
	virtual ~StatementBlock();

	virtual accept(ast::Visitor& visitor) = 0;
private:
	std::vector<Statement*> statements;
};
} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_StatementBlock_
